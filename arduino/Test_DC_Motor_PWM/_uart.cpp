#include "_uart.h"

namespace serial{

      void recvWithStartEndMarkers(char *data, bool &newData)
  {
    static bool recvInProgress = false;
    static uint8_t ndx = 0;
    char rc;
    // memset(data, '\0', 32);

    // if (Serial.available() > 0) {
    // digitalWrite(ERROR2, HIGH);
    while (ser.available() > 0 && newData == false)
    {
      rc = ser.read();

      if (recvInProgress == true)
      {
        int data_count = 0;
        int packet_length = 0;
        char checksum = 0;
        if (rc != endMarker)
        {
          // digitalWrite(TEST1, HIGH);
          // delay(500);
          // digitalWrite(TEST1, LOW);
          // delay(500);
          // digitalWrite(ERROR1, HIGH);
          data[ndx] = rc;
          ndx++;
          if (ndx == 0)
          {
            packet_length = data[ndx];
          }

          data_count++;
          if (ndx >= numChars)
          {
            // Serial.println("In here");
            ndx = numChars - 1;
          }

          if (ser.overflow())
          {
            clear_buffer();
            send_response(1);
            break;
          }
        }
        else
        {
          recvInProgress = false;
          ndx = 0;
          newData = true;
          if (data_count != packet_length)
          {

            clear_buffer();
            newData = false;
            send_response(1);
            break;
          }
        }
      }

      else if (rc == startMarker)
      {
        recvInProgress = true;
      }
    }
  }
}