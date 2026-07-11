/**
 * 自定义内存分配器演示
 * 
 * 来源于 ROS2 Humble Allocator Template Tutorial
 * 对比普通模式和 intra-process 模式下，系统堆分配 vs 自定义分配器的调用次数
 * 
 * 运行方式：
 *   普通模式：   ros2 run allocator_demo allocator_demo
 *   intra 模式： ros2 run allocator_demo allocator_demo intra
 */

#include <chrono>
#include <list>
#include <memory>
#include <string>
#include <utility>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp/allocator/allocator_common.hpp"
#include "rclcpp/strategies/allocator_memory_strategy.hpp"
#include "std_msgs/msg/u_int32.hpp"

using namespace std::chrono_literals;

// ============================================================
// 自定义分配器：统计 allocate/deallocate 调用次数
// ============================================================
static uint32_t num_allocs = 0;
static uint32_t num_deallocs = 0;

template<typename T = void>
struct MyAllocator
{
public:
  using value_type = T;
  using size_type = std::size_t;
  using pointer = T *;
  using const_pointer = const T *;
  using difference_type = typename std::pointer_traits<pointer>::difference_type;

  MyAllocator() noexcept {}
  ~MyAllocator() noexcept {}

  template<typename U>
  MyAllocator(const MyAllocator<U> &) noexcept {}

  T * allocate(size_t size, const void * = 0)
  {
    if (size == 0) { return nullptr; }
    num_allocs++;                              // ← 计数
    return static_cast<T *>(std::malloc(size * sizeof(T)));
  }

  void deallocate(T * ptr, size_t size)
  {
    (void)size;
    if (!ptr) { return; }
    num_deallocs++;                            // ← 计数
    std::free(ptr);
  }

  template<typename U>
  struct rebind
  {
    typedef MyAllocator<U> other;
  };
};

template<typename T, typename U>
constexpr bool operator==(const MyAllocator<T> &, const MyAllocator<U> &) noexcept
{
  return true;
}

template<typename T, typename U>
constexpr bool operator!=(const MyAllocator<T> &, const MyAllocator<U> &) noexcept
{
  return false;
}

// ============================================================
// 重载全局 new/delete，统计系统堆分配次数
// ============================================================
static bool is_running = false;
static uint32_t global_runtime_allocs = 0;
static uint32_t global_runtime_deallocs = 0;

void * operator new(std::size_t size)
{
  if (is_running) { global_runtime_allocs++; }
  return std::malloc(size);
}

void operator delete(void * ptr, size_t size) noexcept
{
  (void)size;
  if (ptr != nullptr) {
    if (is_running) { global_runtime_deallocs++; }
    std::free(ptr);
  }
}

void operator delete(void * ptr) noexcept
{
  if (ptr != nullptr) {
    if (is_running) { global_runtime_deallocs++; }
    std::free(ptr);
  }
}

// ============================================================
// main
// ============================================================
int main(int argc, char ** argv)
{
  using rclcpp::memory_strategies::allocator_memory_strategy::AllocatorMemoryStrategy;
  using Alloc = MyAllocator<void>;
  using MessageAllocTraits =
    rclcpp::allocator::AllocRebind<std_msgs::msg::UInt32, Alloc>;
  using MessageAlloc = MessageAllocTraits::allocator_type;
  using MessageDeleter = rclcpp::allocator::Deleter<MessageAlloc, std_msgs::msg::UInt32>;
  using MessageUniquePtr = std::unique_ptr<std_msgs::msg::UInt32, MessageDeleter>;

  rclcpp::init(argc, argv);

  rclcpp::Node::SharedPtr node;
  std::list<std::string> keys = {"intra", "intraprocess", "intra-process", "intra_process"};
  bool intra_process = false;

  printf(
    "================================================================\n"
    "  自定义内存分配器演示\n"
    "================================================================\n"
    "  普通模式：  ros2 run allocator_demo allocator_demo\n"
    "  Intra 模式： ros2 run allocator_demo allocator_demo intra\n"
    "\n"
    "  每 10ms 发布一条消息到 /allocator_tutorial 话题，Ctrl-C 停止\n"
    "  停止后会打印系统堆分配 vs 自定义分配器的调用次数\n"
    "================================================================\n\n");

  if (argc > 1) {
    for (auto & key : keys) {
      if (std::string(argv[1]) == key) {
        intra_process = true;
        break;
      }
    }
  }

  if (intra_process) {
    printf("Intra-process pipeline is ON.\n");
    node = rclcpp::Node::make_shared("allocator_demo",
      rclcpp::NodeOptions().use_intra_process_comms(true));
  } else {
    printf("Intra-process pipeline is OFF.\n");
    node = rclcpp::Node::make_shared("allocator_demo",
      rclcpp::NodeOptions().use_intra_process_comms(false));
  }

  uint32_t counter = 0;
  auto callback = [&counter](std_msgs::msg::UInt32::ConstSharedPtr msg) -> void
  {
    (void)msg;
    ++counter;
  };

  // ---- 关键：创建自定义分配器，传给 publisher 和 subscriber ----
  auto alloc = std::make_shared<Alloc>();

  rclcpp::PublisherOptionsWithAllocator<Alloc> publisher_options;
  publisher_options.allocator = alloc;                      // ← 注入分配器
  auto publisher = node->create_publisher<std_msgs::msg::UInt32>(
    "allocator_tutorial", 10, publisher_options);

  rclcpp::SubscriptionOptionsWithAllocator<Alloc> subscription_options;
  subscription_options.allocator = alloc;                   // ← 注入分配器
  auto msg_mem_strat = std::make_shared<
    rclcpp::message_memory_strategy::MessageMemoryStrategy<
      std_msgs::msg::UInt32, Alloc>>(alloc);
  auto subscriber = node->create_subscription<std_msgs::msg::UInt32>(
    "allocator_tutorial", 10, callback, subscription_options, msg_mem_strat);

  // Executor 也注入分配器
  std::shared_ptr<rclcpp::memory_strategy::MemoryStrategy> memory_strategy =
    std::make_shared<AllocatorMemoryStrategy<Alloc>>(alloc);

  rclcpp::ExecutorOptions options;
  options.memory_strategy = memory_strategy;                // ← 注入分配器
  rclcpp::executors::SingleThreadedExecutor executor(options);
  executor.add_node(node);

  // 消息的分配/释放也走自定义分配器
  MessageDeleter message_deleter;
  MessageAlloc message_alloc = *alloc;
  rclcpp::allocator::set_allocator_for_deleter(&message_deleter, &message_alloc);

  rclcpp::sleep_for(1ms);
  is_running = true;

  uint32_t i = 0;
  while (rclcpp::ok()) {
    auto ptr = MessageAllocTraits::allocate(message_alloc, 1);
    MessageAllocTraits::construct(message_alloc, ptr);
    MessageUniquePtr msg(ptr, message_deleter);
    msg->data = i;
    ++i;
    publisher->publish(std::move(msg));
    rclcpp::sleep_for(10ms);
    executor.spin_some();
  }
  is_running = false;

  // ---- 输出统计结果 ----
  printf("\n========== 统计结果 ==========\n");
  printf("Global new      调用 %u 次\n", global_runtime_allocs);
  printf("Global delete   调用 %u 次\n", global_runtime_deallocs);
  printf("Allocator new   调用 %u 次\n", num_allocs);
  printf("Allocator delete 调用 %u 次\n", num_deallocs);
  printf("==============================\n");

  // 如果是 intra-process，Global new 应该接近 0
  if (intra_process && global_runtime_allocs == 0) {
    printf("✅ Intra-process 模式：系统堆分配为 0，全部走自定义分配器！\n");
  }

  rclcpp::shutdown();
  return 0;
}
