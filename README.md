# Philux - AVR
Philux is a high-power, trailing edge dimmer remotely controlled on wired/wireless network or serial port

## Contents
* [Build](#build)
* [Upload](#upload)
* [Simulation](#simulation)

## Build

1. Configure
```sh
cmake --preset=philux-avr
```

2. Build
After configuring the project run following command in root directory:
```sh
cmake --build build/philux-avr
```

## Upload
On success build, machine code is available under *cmake-build-debug* directory as *philuxavr-atmega32.hex*. You can upload it to an ATMega32 microcontroller using any programmer.
I use **avrdude** to upload the program onto the microcontroller. I'm using my own simple **[Ponyser][ponyser]** programmer with a RS232/USB cable
```sh
avrdude -p m32 -c ponyser -P /dev/ttyUSB0 -i 1000 -u -V -F -D -B.5 -U flash:w:cmake-build-debug/philuxavr-atmega32.hex:i
```

## Simulation
Simulation environment has been set up in [Proteus][labcenter] using .hex file available under *cmake-build-debug* directory

* Linux
```bash
sudo setcap cap_net_raw,cap_net_admin=epi `which wine-preloader`
```

* Windows

[labcenter]: https://www.labcenter.com/
[ponyser]: https://github.com/pouyanh/ponyser-pcb
