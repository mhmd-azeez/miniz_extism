# miniz for Extism

1. Fetch all submodules
2. Download WASI SDK
3. Build
```
cd plugin
mkdir build
cd build
cmake cmake -D CMAKE_C_COMPILER=/mnt/d/x/wasm/miniz_extism/dependencies/extism-pdk/wasi-sdk/bin/clang ..
make
```