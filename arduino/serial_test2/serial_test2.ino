const uint8_t numChars = 32;
char *receivedChars;
const char startMarker = '<';
const char endMarker = '>';

const char *target_string = "test";

boolean newData = false;

const int TEST1 = 2;
const int TEST2 = 4;

void setup()
{
    Serial.begin(9600);
    receivedChars = (char *)malloc(numChars * sizeof(char)); // Allocate memory for receivedChars
    pinMode(TEST1, OUTPUT);
    pinMode(TEST2, OUTPUT);
    digitalWrite(TEST1, LOW);
    digitalWrite(TEST2, LOW);

    Serial.println("<Arduino is ready>");
}

// Turn on Left and Right motors at different speeds.
void loop()
{
    // receive data from serial port
    recvWithStartEndMarkers();
    showNewData();
    // Serial.print("<test>");
    // delay(1000);
    // Serial.println("a");
}

// adopted from https://forum.arduino.cc/t/serial-input-basics/278284/2
void recvWithStartEndMarkers()
{
    static bool recvInProgress = false;
    static uint8_t ndx = 0;
    char rc;

    // if (Serial.available() > 0) {
    while (Serial.available() > 0 && newData == false)
    {
        rc = Serial.read();

        if (recvInProgress == true)
        {
            if (rc != endMarker)
            {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars)
                {
                    ndx = numChars - 1;
                }
            }
            else
            {
                // Serial.print("In end ");
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker)
        {
            // Serial.print("In start ");
            recvInProgress = true;
        }
    }
}

void showNewData()
{
    if (newData == true)
    {
        // Serial.print("This just in ... ");

        if (strcmp(receivedChars, target_string) == 0)
        {
            // Serial.println("Matched");
            digitalWrite(TEST1, HIGH);
            digitalWrite(TEST2, LOW);
            delay(1000);
        }
        else
        {
            // Serial.println("Not Matched");
            digitalWrite(TEST1, LOW);
            digitalWrite(TEST2, HIGH);
            delay(1000);
        }

        digitalWrite(TEST1, LOW);
        digitalWrite(TEST2, LOW);
        char temp = 101;
        Serial.print("<");
        Serial.write(&temp, 1);
        Serial.print(">");
        Serial.flush();
        newData = false;
    }
}
