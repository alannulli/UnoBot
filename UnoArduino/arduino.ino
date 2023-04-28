#include <OneWire.h>
#include <DallasTemperature.h>
#include <iostream> // for is_number
using namespace std; // for is_number
#include <string> // for setTemp

// Data wire is conntec to the Arduino digital pin 4
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

int redPin = 11;                // output pin number to Red pin of the LED
int greenPin = 10;              // output pin number to Green pin
int bluePin = 9;                // output pin number to Blue pin

int STBY = 2;   // A Channel STBY Pin
int AIN1 = 3;   // A Channel Input 1
int AIN2 = 7;   // A Channel Input 2
int PWMA = 5;  // A Channel PWM

// data points storage
const int MAX_DATA = 10; // Maximum number of data points to store
int data[MAX_DATA]; // Array to store data
int num_data = 0; // Number of data points stored so far

void setup(void)
{
  // Start serial communication for debugging purposes
  Serial.begin(9600);
  // Start up the library
  sensors.begin();
  pinMode(redPin, OUTPUT);      // sets the pin as output pin
  pinMode(greenPin, OUTPUT);    // sets the pin as output pin
  pinMode(bluePin, OUTPUT); 

  pinMode(STBY, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
}

// helper: check if string is a number
bool is_number(string str) {
  for (int i = 0; i< str.length(); i++) {
    if (isdigit(str[i]) == false)
      return false;
  }
  return true;
}

void loop(void){ 
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  
  // Serial.print("Celsius temperature: ");
  // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  // Serial.print(sensors.getTempCByIndex(0)); 
  // Serial.print(" - Fahrenheit temperature: ");
  float a = sensors.getTempFByIndex(0);
  Serial.println(sensors.getTempFByIndex(0));

  // if (Serial.available()) {
  //   int val = Serial.parseInt(); // Read the next integer from the serial port
  //   if (num_data < MAX_DATA) {
  //     data[num_data] = val; // Store the data in the array
  //     num_data++; // Increment the number of data points stored
  //   }
  // }

  String serialListener = Serial.readString();
  Serial.println(serialListener);
  if (serialListener == "ON") {
      analogWrite(redPin, 255);
  }
  else if (serialListener == "OFF") {
      analogWrite(redPin, 0);
  } 
  else if (serialListener == "makeHotter") {
    while (a < 90) {
      digitalWrite(AIN1, LOW); // red
      digitalWrite(AIN2, HIGH); // black
      analogWrite(PWMA, 170);
    }
  }
  else if (serialListener == "makeColder") {
    while (a > 60) { // IDK IF THIS IS RIGHT
      digitalWrite(AIN1, HIGH); // red
      digitalWrite(AIN2, LOW); // black
      analogWrite(PWMA, 170);
    }
  }
  else if (is_number(serialListener)) {// start of setTemp
    float temp = std::stof(serialListener); // convert string to float
    if (temp > 60 || temp < 80) { // check if temp is in range IDK THE RANGES
      analogWrite(greenPin, 0); // lets have a color indicate that we r warming/cooling
      analogWrite(bluePin, 0);
      analogWrite(redPin, 0);
      // insert cooling or heating i forget
      analogWrite(PWMA, 0);
    }
    // otherwise don't change temp bc out of range
    Serial.println("out of range");
  }

  digitalWrite(STBY, HIGH);
  
  if (60 < a < 80) {
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
    analogWrite(redPin, 0);
    analogWrite(PWMA, 0);
  }
  if (a > 80) {
    analogWrite(bluePin, 0);
    analogWrite(greenPin, 0);
    analogWrite(redPin, 255);
    // digitalWrite(AIN1, HIGH); // red
    // digitalWrite(AIN2, LOW); // black
    analogWrite(PWMA, 170);
  }
  if (a < 60) {
    analogWrite(redPin, 0);
    analogWrite(bluePin, 255);
    analogWrite(greenPin, 0);
    // digitalWrite(AIN1, LOW); // red
    // digitalWrite(AIN2, HIGH); // black
    analogWrite(PWMA, 170);
  }
  delay(1000);
}