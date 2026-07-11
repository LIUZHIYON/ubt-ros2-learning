from setuptools import find_packages, setup

package_name = 'rqt_bag_diagnostics_demo'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name, ['plugins.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='ubt',
    maintainer_email='2950317571@qq.com',
    description='rqt_bag plugin for diagnostics_msgs',
    license='Apache-2.0',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'diagnostic_pub = rqt_bag_diagnostics_demo.diagnostic_pub:main',
        ],
    },
)
