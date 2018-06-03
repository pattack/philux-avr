* Configure
```sh
cmake -DCMAKE_TOOLCHAIN_FILE=avr.cmake --build cmake-build-debug
```

* Build
```sh
cmake --build cmake-build-debug --target philuxavr -- -j 2
```
