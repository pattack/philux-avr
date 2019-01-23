# Philux - AVR
Philux is a high-power, trailing edge dimmer remotely controlled on wired/wireless network or serial port

## Contents
* [Build](#build)
* [Upload](#upload)
* [Simulation](#simulation)

## Build

1. Configure
```sh
mkdir cmake-build-debug
cd cmake-build-debug
cmake -DCMAKE_TOOLCHAIN_FILE=avr.cmake ../
```

2. Build
```sh
cmake --build cmake-build-debug --target philuxavr -- -j 2
```

## Upload

## Simulation
Simulation environment has been set up in [Proteus][labcenter]

* Linux
```bash
sudo setcap cap_net_raw,cap_net_admin=epi `which wine-preloader`
```

* Windows

[labcenter]: https://www.labcenter.com/
