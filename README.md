# miniz for Extism

This repo shows how you can convert a normal C library to an Extism Plugin. [miniz](https://github.com/richgel999/miniz) is a minimal compression library.

## Try it out

1. Fetch all submodules

```
git submodule update --init --recursive
```

2. Download WASI SDK

```
cd dependencies/extism-pdk
./install-wasi-sdk.sh
```

This downloads WASI SDK to `dependencies/extism-pdk/wasi-sdk`.

3. Build
```
cd plugin
mkdir build
cd build
cmake cmake -D CMAKE_C_COMPILER=$(pwd)/../../dependencies/extism-pdk/wasi-sdk/bin/clang ..
make
```

Now you should be able to find a `miniz_extism.wasm` in `plugin/build`.

4. Test
```
cd host
go run .
```

You should get an output similar to this:
```
PS D:\x\wasm\miniz_extism\host> go run .
Original data: Hello, World! Hello, World! Hello, World! Hello, World! Hello, World!
Original data len: 69
Compressed data len: 39
Decompressed data: Hello, World! Hello, World! Hello, World! Hello, World! Hello, World!
```