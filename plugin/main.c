#define EXTISM_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include "miniz.h"
#include "extism-pdk.h"

#define CHUNK_SIZE 16384

int main() {
  printf("Miniz version: %s\n", MZ_VERSION);
  return 0;
}

int32_t EXTISM_EXPORTED_FUNCTION(compress_stream) {
  uint64_t inputLen = extism_input_length();

  // Load input
  uint8_t *inputData = malloc(inputLen);
  extism_load_input(0, inputData, inputLen);

  // Calculate the maximum compressed size
  uint64_t maxCompressedSize = mz_compressBound(inputLen);
  uint8_t *compressedData = malloc(maxCompressedSize);

  // Initialize deflate stream
  mz_stream stream;
  stream.next_in = inputData;
  stream.avail_in = inputLen;
  stream.next_out = compressedData;
  stream.avail_out = maxCompressedSize;

  mz_deflateInit(&stream, MZ_BEST_COMPRESSION);

  // Compress input data in chunks
  int status;
  do {
    status = mz_deflate(&stream, MZ_FINISH);
  } while (status == MZ_OK);

  if (status != MZ_STREAM_END) {
    extism_error_set_buf_from_sz("Failed to compress data\0");
    return status;
  }

  mz_deflateEnd(&stream);

  // Set output
  extism_output_buf(compressedData, stream.total_out);

  free(inputData);
  free(compressedData);

  return 0;
}

int32_t EXTISM_EXPORTED_FUNCTION(decompress_stream) {
  uint64_t inputLen = extism_input_length();

  // Load input
  uint8_t *inputData = malloc(inputLen);
  extism_load_input(0, inputData, inputLen);

  // Initialize inflate stream
  mz_stream stream;
  stream.next_in = inputData;
  stream.avail_in = inputLen;
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;

  mz_inflateInit(&stream);

  // Decompress input data in chunks
  int status;
  uint64_t decompressedSize = CHUNK_SIZE;
  uint8_t *decompressedData = malloc(decompressedSize);

  do {
    if (stream.total_out >= decompressedSize) {
      decompressedSize += CHUNK_SIZE;
      decompressedData = realloc(decompressedData, decompressedSize);
      if (decompressedData == NULL) {
        return MZ_MEM_ERROR;
      }
    }
    stream.next_out = decompressedData + stream.total_out;
    stream.avail_out = decompressedSize - stream.total_out;

    status = mz_inflate(&stream, MZ_NO_FLUSH);
  } while (status == MZ_OK);

  if (status != MZ_STREAM_END) {
    extism_error_set_buf_from_sz("Failed to decompress data\0");
    return status;
  }

  mz_inflateEnd(&stream);

  // Set output
  extism_output_buf(decompressedData, stream.total_out);

  free(inputData);
  free(decompressedData);

  return 0;
}