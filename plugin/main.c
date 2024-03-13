#define EXTISM_IMPLEMENTATION
//#define EXTISM_ENABLE_LOW_LEVEL_API

#include <stdio.h>
#include "miniz.h"
#include "extism-pdk.h"

int main() {
    printf("Miniz version: %s\n", MZ_VERSION);
    return 0;
}

#define Greet_Max_Input 1024
static const char Greeting[] = "Hello, ";

int32_t EXTISM_EXPORTED_FUNCTION(greet) {
  uint64_t inputLen = extism_input_length();
  if (inputLen > Greet_Max_Input) {
    inputLen = Greet_Max_Input;
  }

  // Load input
  static uint8_t inputData[Greet_Max_Input];
  extism_load_input(0, inputData, inputLen);

  // Allocate memory to store greeting and name
  const uint64_t greetingLen = sizeof(Greeting) - 1;
  const uint64_t outputLen = greetingLen + inputLen;
  ExtismHandle handle = extism_alloc(outputLen);
  extism_store_to_handle(handle, 0, Greeting, greetingLen);
  extism_store_to_handle(handle, greetingLen, inputData, inputLen);

  // Set output
  extism_output_set_from_handle(handle, 0, outputLen);
  return 0;
}