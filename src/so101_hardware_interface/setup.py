from setuptools import find_packages, setup

package_name = 'so101_hardware_interface'

setup(
    name=package_name,
    version='0.0.1',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/config', ['config/hardware_params.yaml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='your_name',
    maintainer_email='you@example.com',
    description=(
        'Bridges the real SO-101 Feetech servo bus to ROS 2 via '
        "LeRobot's SOFollower driver."
    ),
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'hardware_interface_node = '
            'so101_hardware_interface.hardware_interface_node:main',
        ],
    },
)
