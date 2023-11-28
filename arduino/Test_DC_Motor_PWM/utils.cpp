#include "utils.h"

unsigned short convert_ushort(char* data, char data_length){
  unsigned short *output;
  memcpy(output, data, data_length);
  data += data_length;

  return *output;
}