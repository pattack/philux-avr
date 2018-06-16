* Run simulation
* Linux
```bash
sudo setcap cap_net_raw,cap_net_admin=epi `which wine-preloader`
```

* Windows

* Configure
```sh
mkdir cmake-build-debug
cd cmake-build-debug
cmake -DCMAKE_TOOLCHAIN_FILE=avr.cmake ../
```

* Build
```sh
cmake --build cmake-build-debug --target philuxavr -- -j 2
```
