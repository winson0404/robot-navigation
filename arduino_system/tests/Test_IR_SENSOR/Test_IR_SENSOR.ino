const int right_IR_Pin = 11;  
const int left_IR_Pin = 9; 

void setup() {
  // put your setup code here, to run once:
	pinMode(right_IR_Pin, INPUT);  
	pinMode(left_IR_Pin, INPUT);  
	Serial.begin(9600);  
}

void loop() {
  // put your main code here, to run repeatedly:
  bool irValue_right = digitalRead(right_IR_Pin);
  bool irValue_left = digitalRead(left_IR_Pin);

  if (irValue_right == LOW){
	Serial.println("Right IR Sensor is detecting something");
  }
  else{
	Serial.println("Right IR Sensor is not detecting anything");
  }

  if (irValue_left == LOW){
	Serial.println("Left IR Sensor is detecting something");
  }
  else{
	Serial.println("Left IR Sensor is not detecting anything");
  }

  delay(500);

}
