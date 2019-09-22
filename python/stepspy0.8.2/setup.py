import setuptools

with open("README.md", "r") as fh:
    long_description = fh.read()

setuptools.setup(
    name="stepspy",
    version="0.8.2",
    author="Changgang Li",
    author_email="lichgang@sdu.edu.cn",
    description="Python module of Simulation Toolkit for Electrical Power Systems",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/changgang/steps",
    packages=setuptools.find_packages(),
    classifiers=[
        "Programming Language :: Python :: 2",
        "Programming Language :: Python :: 3",
        "Programming Language :: C++",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
)