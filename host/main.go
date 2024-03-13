package main

import (
	"context"
	"fmt"
	"os"

	extism "github.com/extism/go-sdk"
)

func main() {
	manifest := extism.Manifest{
		Wasm: []extism.Wasm{
			extism.WasmFile{
				Path: "../plugin/build/miniz_extism.wasm",
			},
		},
	}

	ctx := context.Background()
	config := extism.PluginConfig{
		EnableWasi: true,
	}
	plugin, err := extism.NewPlugin(ctx, manifest, config, []extism.HostFunction{})

	if err != nil {
		fmt.Printf("Failed to initialize plugin: %v\n", err)
		os.Exit(1)
	}

	defer plugin.Close()

	data := []byte("Hello, World! Hello, World! Hello, World! Hello, World! Hello, World!")
	fmt.Printf("Original data: %s\n", string(data))
	fmt.Printf("Original data len: %v\n", len(data))

	_, out, err := plugin.Call("compress_stream", data)
	if err != nil {
		fmt.Printf("Failed to call compress_stream: %v\n", err)
		os.Exit(1)
	}

	fmt.Printf("Compressed data len: %v\n", len(out))

	_, out, err = plugin.Call("decompress_stream", out)
	if err != nil {
		fmt.Printf("Failed to call decompress_stream: %v\n", err)
		os.Exit(1)
	}

	fmt.Printf("Decompressed data: %s\n", string(out))
}
