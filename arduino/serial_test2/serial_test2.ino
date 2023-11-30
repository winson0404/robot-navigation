void setup()
{
  Serial.begin(9600);
}

// Turn on Left and Right motors at different speeds.
void loop()
{
  //receive data from serial port
  char *data;
  if(Serial.available()){
    Serial.readBytes(data, 1);
    //convert data to string
    //print string
    Serial.println(*data);

  }


  // delay(1000);
  
}
