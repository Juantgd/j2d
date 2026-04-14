```shell
# build
cmake -S . -B build
cd build && make -j$(nproc)

# run
./Release/sdl3_test
```
