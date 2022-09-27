# Daqromancy
Daqromancy is a data aquisition application designed for use with CAEN DPP digitizers using the CAENDigitizer library. Daqromancy is focused on providing a clean, powerful interface to the digitizers in an open source,
(hopefully) friendly environment.

Daqromancy does not provide online data analysis/visulization short of a simple osciloscope and data rates. Daqromancy does provide a data server which can be used to pipe data to an external analysis program.

## Installation
Currently, Daqromancy only supports installing from source.

To download the Daqromancy source retrieve the repository using `git clone --recursive https://github.com/gwm17/Daqromancy.git`. Move into the repostiory and follow standard CMake build instructions. For example,
on a Linux system use the following commands:

```
mkdir build
cd build
cmake ..
make -j 4
```

This will build and install the Daqromancy executable in the `bin` directory of the repository.

## About
This project started as an extension of the [BoxScore](https://github.com/goluckyryan/BoxScore) application framework, developed by @goluckyryan. As it grew, it became clear that the extension had become it's own
monster. This monster is known as Daqromancy!

First and foremost, Daqromancy was an opprotunity for me to learn more about how a DAQ works and how to design a project of this scope and complexity. So it should be noted that Daqromancy is a learning process,
and will most likely undergo many changes and refactors before it is ready to do anything serious.

Daqromancy aims to provide a cross-platform DAQ that is both readable and transparent without sacrificing performance.

## Dependencies
Daqromancy requires the [CAENDigitizer](https://www.caen.it) library to be installed (this is the middelware that allows for digitizer communication).
The CAENDigitizer library only supports Linux and Windows at this time; as such Daqromancy only targets Windows and Linux.

On Windows, it is recommended to use the Visual Studio environment to build the project.

On Linux, Daqromancy needs the development tools for the X system. On Ubuntu this can be installed using: 
`sudo apt install libgl1 libgl1-mesa-dev libglu1-mesa libglu1-mesa-dev xorg-dev mesa-utils`

## Warning
Daqromancy is under very heavy development still, and as such should not be used in any production capacity at this time
