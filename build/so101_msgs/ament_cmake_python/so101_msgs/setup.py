from setuptools import find_packages
from setuptools import setup

setup(
    name='so101_msgs',
    version='0.0.1',
    packages=find_packages(
        include=('so101_msgs', 'so101_msgs.*')),
)
