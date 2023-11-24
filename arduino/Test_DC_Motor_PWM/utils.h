#pragma once
#include <Arduino.h>


unsigned short convert_ushort(char* data, char data_length){
  unsigned short *output;
  for (int i = 0; i < data_length; i++){
    (output + i) = data;
    data++;
  }

  return *output;
}

// unsigned short convert_ushort(char* data, char data_length){
//   unsigned short *output;
//   for (int i = 0; i < data_length; i++){
//     (output + i) = data;
//     data++;
//   }

//   return *output;
// }