#include <Servo.h> //servo library
#include "DHT.h"

// ultrasonic sensor for ambient distance
const int trigPin_ambi = 5;
const int echoPin_ambi = 6;

// ultrasonic sensor for fill level
const int trigPin_fill = 8;
const int echoPin_fill = 9;

// servo configuration
Servo servo;
const int servoPin = 7;

// led for 'on' indication
const int led = 10;

// LED bar graph
int ledCount = 5; // the number of LEDs in the bar graph
int ledPins[] = {0, 1, 2, 3, 4}; // an array of pin numbers to which LEDs are attached

// ADXL-335
const int xpin = A3; // x-axis of the accelerometer
const int ypin = A2; // y-axis
const int zpin = A1; // z-axis (only on 3-axis models)

// DHT11 - temperature humidity sensor
#define DHTTYPE DHT22
const int DHTPIN = 11;
DHT dht(DHTPIN, DHTTYPE);

long dist_ambi, dist_fill, duration_ambi, duration_fill, bin_height; //variables for dustbin fill level
long aver_ambi[3];   //array for average
long cali_height[3]; //array for bin height calibration
// variables for ADXL335
int xsample = 0;
int ysample = 0;
int zsample = 0;
long start;
bool topple;
#define samples 50
#define maxVal 20
#define minVal -20
#define ckTime 2000

// function to measure ambient distance
long measure_ambi()
{
  long cm, inches;

  digitalWrite(led, HIGH);
  digitalWrite(trigPin_ambi, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin_ambi, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin_ambi, LOW);
  pinMode(echoPin_ambi, INPUT);

  duration_ambi = pulseIn(echoPin_ambi, HIGH);
  // inches = microsecondsToInches(duration_ambi); // distance in inches
  cm = microsecondsToCentimeters(duration_ambi); // distance in cm

  Serial.print("Ambient distance measure: ");
  // Serial.print(inches);
  Serial.print(cm);
  Serial.print("\n");

  return cm;
}

// function to measure fill level
long measure_fill()
{
  long cm, inches;

  digitalWrite(trigPin_fill, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin_fill, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin_fill, LOW);
  pinMode(echoPin_fill, INPUT);

  duration_fill = pulseIn(echoPin_fill, HIGH);
  // inches = microsecondsToInches(duration_fill);  // distance in inches
  cm = microsecondsToCentimeters(duration_fill); // distance in cm

  Serial.print("Fill level measure: ");
  // Serial.print(inches);
  Serial.print(cm);
  Serial.print("\n");

  return cm;
}

// function to convert duration to inches
long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}

// function to convert duration to cms
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}

// function to open bin when personnel is standing near
void open(long d)
{
  if (d < 20)
  {
    //Change distance as per your need
    servo.attach(servoPin);
    delay(1);
    servo.write(0);
    delay(3000);
    servo.write(150);
    delay(1000);
    servo.detach();
    Serial.print("Distance from bin: ");
    Serial.print(d);
    Serial.print("\n");
  }
}

// calibrating the ADXL335
void calibrate_adxl(){
  for (int i = 0; i < samples; i++)
  {
    xsample += analogRead(xpin);
    ysample += analogRead(ypin);
    zsample += analogRead(zpin);
  }
  xsample /= samples;
  ysample /= samples;
  zsample /= samples;
}

// setup process - will run only once
void setup()
{
  Serial.begin(9600); // Starting Serial Terminal

  delay(5000);

  // ADXL335 setup
  calibrate_adxl();
  topple = false;

  // servo setup
  servo.attach(servoPin);
  
  // ultrasonic sensor
  pinMode(trigPin_ambi, OUTPUT);
  pinMode(echoPin_ambi, INPUT);
  pinMode(trigPin_fill, OUTPUT);
  pinMode(echoPin_fill, INPUT);
  servo.write(0); //close cap on power on
  delay(100);
  servo.detach();

  // loop over the led pin array and set them all to output:
  for (int thisLed = 0; thisLed < ledCount; thisLed++)
  {
    pinMode(ledPins[thisLed], OUTPUT);
  }

  // calibrating bin_height
  for (int i = 0; i <= 2; i++)
  { //average distance
    cali_height[i] = measure_fill();
    delay(1000); //delay between measurements
  }
  // calculating bin height
  bin_height = (cali_height[0] + cali_height[1] + cali_height[2]) / 3;

  // starting dht11 sensor
  dht.begin();

}

// constantly looping code
void loop()
{
  for (int i = 0; i <= 2; i++)
  { 
    //average distance
    aver_ambi[i] = measure_ambi();
    delay(10); //delay between measurements
  }
  dist_ambi = (aver_ambi[0] + aver_ambi[1] + aver_ambi[2]) / 3;

  // open bin if person nearby
  open(dist_ambi);

  /* mapping to led graphs */
  // measuring fill level
  int fill_level = measure_fill();
  // map the result to a range from 0 to the number of LEDs: 
  int ledLevel = map(fill_level, 0, bin_height, 0, ledCount);
  // loop over the LED array:
  for (int thisLed = 0; thisLed < ledCount; thisLed++){
    // if the array element's index is less than ledLevel, 
    // turn the pin for this element on:
    if (thisLed < ledLevel){
      digitalWrite(ledPins[thisLed], HIGH);
    }
    // turn off all pins higher than the ledLevel:
    else
    {
      digitalWrite(ledPins[thisLed], LOW);
    }
  }

  // detecting if excess movement occurs
  int value1 = analogRead(xpin);
  int value2 = analogRead(ypin);
  int value3 = analogRead(zpin);

  int xValue = xsample - value1;
  int yValue = ysample - value2;
  int zValue = zsample - value3;

  // printing ADXL335 values
  // Serial.print("x=");
  // Serial.println(xValue);
  // Serial.print("y=");
  // Serial.println(yValue);
  // Serial.print("z=");
  // Serial.println(zValue);
  // Serial.println(" $");

  if (xValue < minVal || xValue > maxVal || yValue < minVal || yValue > maxVal || zValue < minVal || zValue > maxVal)
  {
    // excess movement detected
    if(topple = false)
      start = millis();
    topple = true;
  }
  else if(topple == true){
    if(millis()>=start+ckTime){
      // dustbin has fallen, stop measuring
      Serial.print("Dustbin has fallen!");
      Serial.print("\n");
      exit(1);
    }
  }

  // Reading temperature or humidity takes about 250 milliseconds
  float hum = dht.readHumidity();
  float temp = dht.readTemperature(); // in celcius

  // Compute heat index in Celsius (isFahreheit = false) 
  float hic = dht.computeHeatIndex(temp, hum, false);

  Serial.print("temperature and humidity readings");
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print("\n");
  Serial.print("Temperature (*C): ");
  Serial.print(temp);
  Serial.print("\n");
  Serial.print("Heat index: ");
  Serial.print(hic);
}
