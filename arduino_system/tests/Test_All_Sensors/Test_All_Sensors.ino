const int trigPin = 10;
const int echoPin = 3;
const int right_IR_Pin = 11;
const int mid_IR_Pin = A0;
const int left_IR_Pin = 9;

void setup()
{
    // put your setup code here, to run once:
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(right_IR_Pin, INPUT);
    pinMode(left_IR_Pin, INPUT);
    pinMode(mid_IR_Pin, INPUT);
    Serial.begin(9600);
}

void loop()
{
    // put your main code here, to run repeatedly:
    //ULTRA SONIC
    float duration, distance;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration * .0343) / 2;

    Serial.print("Distance: ");
    Serial.println(distance);

    // IR
    bool irValue_right = digitalRead(right_IR_Pin);
    bool irValue_left = digitalRead(left_IR_Pin);
    bool irValue_mid = digitalRead(mid_IR_Pin);

    if (irValue_right == LOW)
    {
        Serial.println("Right 1"); // 1 means detecting something
    }
    else
    {
        Serial.println("Right 0"); // 0 means not detecting anything
    }

    if (irValue_left == LOW)
    {
        Serial.println("Left 1");
    }
    else
    {
        Serial.println("Left 0");
    }

    if (irValue_mid == LOW)
    {
        Serial.println("Mid 1");
    }
    else
    {
        Serial.println("Mid 0");
    }
    delay(1000);
}
