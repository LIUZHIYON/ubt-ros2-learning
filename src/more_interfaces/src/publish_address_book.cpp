#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
// 这就是 rosidl_generate_interfaces 自动生成的头文件！
// 路径规则：包名/msg/消息名.hpp（蛇形命名）
#include "more_interfaces/msg/address_book.hpp"

using namespace std::chrono_literals;

class AddressBookPublisher : public rclcpp::Node
{
public:
  AddressBookPublisher()
  : Node("address_book_publisher")
  {
    // 创建发布者：话题名 = "address_book"，队列长度 = 10
    address_book_publisher_ =
      this->create_publisher<more_interfaces::msg::AddressBook>("address_book", 10);

    // 每 1 秒调一次 publish_message()
    timer_ = this->create_wall_timer(
      1s, std::bind(&AddressBookPublisher::publish_message, this));
  }

private:
  void publish_message()
  {
    auto message = more_interfaces::msg::AddressBook();

    // 填写地址簿信息
    message.first_name = "小";
    message.last_name = "夏";
    message.phone_number = "13800138000";
    message.phone_type = message.PHONE_TYPE_MOBILE;

    RCLCPP_INFO(this->get_logger(),
      "发布联系人: '%s %s' 电话: '%s' 类型: '%d'",
      message.first_name.c_str(), message.last_name.c_str(),
      message.phone_number.c_str(), message.phone_type);

    address_book_publisher_->publish(message);
  }

  rclcpp::Publisher<more_interfaces::msg::AddressBook>::SharedPtr address_book_publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<AddressBookPublisher>());
  rclcpp::shutdown();
  return 0;
}
