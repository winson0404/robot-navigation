#include "utils.h"

unsigned short convert_ushort(char* data, char data_length){
  unsigned short *output;
  memcpy(output, data, data_length);
  data += data_length;

  return *output;
}

namespace comms{

      packet construct_packet(uint8_t task, uint8_t num_data, uint8_t *data_length, uint16_t *data)
    {
      packet p;
      p.task = task;

      p.num_data = num_data;

      p.packet_length = 1; // packet_length (1 byte each)
      p.packet_length += sizeof(task); // task (1 byte)
      p.packet_length += sizeof(num_data); // num_data (1 byte)

      for (int i = 0; i < p.num_data; i++)
      {
        p.data_length[i] = data_length[i];
        p.data[i] = data[i];
        p.packet_length += sizeof(data_length[i]); // data_length (1 byte)
        p.packet_length += data_length[i]; // data (2 byte)
      }

      p.checksum = 0;
      p.packet_length += sizeof(p.checksum); // checksum (1 byte)

      return p;
    }

    packet construct_packet_from_data_buffer(uint8_t *data)
    {
      // protocol for data: <task|packet_length|data_length|data>
      // task: 1 byte
      // packet_length: 1 byte
      // data_length: 1 byte
      // data: (data_length) byte

      // packet p;
      // checksum : 0
      // packet_length : 1
      // task : 2
      // num_data : 3

      packet p;

      int counter = 0;
      p.packet_length = data[counter++];
      p.task = data[counter++];
      p.num_data = data[counter++];

      for (int i = 0; i < p.num_data; i++)
      {
        p.data_length[i] = data[counter++];
        uint8_t *temp = (uint8_t *)malloc(p.data_length[i]);
        for (int j = 0; j < p.data_length[i]; j++)
        {
          temp[j] = data[counter++];
        }

        p.data[i] = *(reinterpret_cast<uint16_t *>(temp));
        free(temp);
      }

      p.checksum = data[counter++];

      return p;
    }


}