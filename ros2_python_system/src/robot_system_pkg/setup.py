from setuptools import find_packages, setup

package_name = 'robot_system_pkg'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='weixiong0404',
    maintainer_email='weixiong8489@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            "comm_node = robot_system_pkg.comm_node:main",
            "brain_node = robot_system_pkg.brain_node:main",
            "inference_node = robot_system_pkg.inference_node:main"
        ],
    },
)
