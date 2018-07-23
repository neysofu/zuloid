from setuptools import setup
from setuptools import find_packages

DEPENDENCIES = [
    "python-chess==0.23.7",
    "numpy==1.14.5",
    "tensorflow==1.1.0",
    "keras==2.1.2"]

setup(
    name="Z64C",
    version="0.1.0",
    license="GNU GPLv3",
    install_requires=DEPENDENCIES,
    platform=["any"],
)
