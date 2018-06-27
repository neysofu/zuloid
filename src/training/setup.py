from setuptools import setup
from setuptools import find_packages

install_requires = {
    'numpy',
    'scipy',
    'tqdm'
}

setup(
    name="Z64C-dev",
    version="0.1.0",
    packages=find_packages(),
    include_package_data=True,
    license="GNU GPLv3",
    install_requires=install_requires,
    description="CapsLayer: An advanced library for capsule theory",
    keywords="capsule, capsNet, deep learning, tensorflow",
    platform=['any']
)
