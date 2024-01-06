#include "comms.h"

namespace comms
{
  void comms_setup()
  {
    ser.begin(baud_rate);
    // ser.listen();
  }

  void comms_listener(bool &comm_state, packet &p)
  {
    if (comm_state == RECEIVE)
    {
      handle_receive(p);
    }
    else
    {
      handle_send(p);
      comm_state = RECEIVE;
    }
  }

  bool handle_receive(packet &p)
  {
  }

  bool handle_send(packet &p)
  {
  }

  packet packet_post_process(uint8_t *data)
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
    p.data_length = (uint8_t *)malloc(sizeof(uint8_t) * constants::MAX_DATA_LENGTH);
    p.data = (uint16_t *)malloc(sizeof(uint16_t) * constants::MAX_DATA_LENGTH);

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

    
  }


  void free_packet(packet &p)
  {
    free(p.data_length);
    free(p.data);
  }

}
