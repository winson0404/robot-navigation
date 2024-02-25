#include "SoftwareSerial.h"
#include "comms.h"

namespace comms
{
  void comms_setup(SoftwareSerial &ser)
  {
    // ser = SoftwareSerial(constants::RX_PIN, constants::TX_PIN);
    ser.begin(constants::BAUD_RATE);
    // ser.listen();
  }

  void comms_listener(SoftwareSerial &ser, bool &comm_state, uint8_t &task_state, packet &p, uint8_t &task_status)
  {
    if (task_status == constants::TASK_SUCCESS)
    {
      // send ack
      // serial::send_acknowledge(ser, 0, constants::START_MARKER, constants::END_MARKER);
      task_status = constants::TASK_EMPTY;
    }

    if (comm_state == constants::RECEIVE)
    {
      // Serial.print("Comm state: ");
      // Serial.println(comm_state);

      uint8_t data_buffer[constants::RECEIVE_BUFFER_SIZE];
      if (handle_receive(ser, p, data_buffer))
      {
        // Serial.print("Task: ");
        // Serial.println(p.task);
        task_state = p.task / 10;
      }
    }
    else // comm_state == SEND
    {
      // Serial.print("Comm state: ");
      // Serial.println(comm_state);
      handle_send(ser, p);
      comm_state = constants::RECEIVE;
    }
  }

  bool handle_receive(SoftwareSerial &ser, packet &p, uint8_t data[])
  {
    bool new_data_available = false;
    serial::recv_with_start_end_markers(ser, data, new_data_available, constants::START_MARKER, constants::END_MARKER);
    if (new_data_available)
    {
      // packet pack_packet;
      p = construct_packet_from_data_buffer(data);
      // serial::send_acknowledge(ser, constants::ACKNOWLEDGE_SUCCESS, constants::START_MARKER, constants::END_MARKER);
      return true;
    }
    else
    {
      // free(data);
      return false;
    }
  }

  bool handle_send(SoftwareSerial &ser, packet &p)
  {
    serial::send_with_start_end_markers(ser, p, constants::START_MARKER, constants::END_MARKER);
    // serial::receive_acknowledge(ser, p.task, constants::START_MARKER, constants::END_MARKER);
  }

}
