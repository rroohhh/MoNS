# MoNS, a Modular nbody solver

MoNS is a modular libary for nbody simulations targeted at high performance computing.

Currently it is under heavy development.

## Features

* None yet completed

## Building

For now there is just a simple toplevel CMakeLists.txt, until this is replaced by a proper cmake building system,
a simple
```shell
cmake CMakeLists.txt
```
should do the trick. If building fails make sure you meet the building requirements!

### Requirements:
* a c++14 compilant compiler (g++5.x or clang3.8 should do)

## ToDo

* Target needs get and set functions
* Get the basic framework going (adding modules/targets and executing simulations)
* Make simulationtypes configurable
* Get better system for output (disable-able, different output targets)
* write some basic modules (bhut-tree, gravity, collisions,...)
* support for more target (MPI, GPU,...)
* write some programs, that do simulations with this library
* library for automatic cpu/gpu/cluster programming