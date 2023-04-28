#include <OneWire.h>
#include <DallasTemperature.h>
using namespace std; // for is_number
#include <string.h> // for setTemp
#include <ctype.h>

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

// initialize light color to white
int green = 255;
int blue = 255;
int red = 255;

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
bool is_number(String str) {
  if(str == ""){
    return false;
  }
  for (int i = 0; i< str.length(); i++) {
    if (isdigit(str.charAt(i)) == false)
      return false;
  }
  return true;
}

void loop(void){ 
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
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

  if (serialListener == "makeHot") {
    // heat drink until minimum of 136 degrees F
    while (a < 136) {
      digitalWrite(AIN1, LOW); // red
      digitalWrite(AIN2, HIGH); // black
      analogWrite(PWMA, 170);
    }
    digitalWrite(AIN1, LOW); // red
    digitalWrite(AIN2, LOW); // black
    analogWrite(PWMA, 0);
  }
  else if (serialListener == "makeCold") {
    // cool drink until minimum of 44 degrees F
    while (a > 44) { 
      digitalWrite(AIN1, HIGH); 
      digitalWrite(AIN2, LOW); 
      analogWrite(PWMA, 170);
    }
    // turn off peltier
    digitalWrite(AIN1, LOW); 
    digitalWrite(AIN2, LOW); 
    analogWrite(PWMA, 0);
  }
  else if (is_number(serialListener)) {// start of setTemp
    float temp = serialListener.toFloat();

    if (temp > 44 && temp < 136) { // check if temp is in the ideal range
      sensors.requestTemperatures();
      while (sensors.getTempFByIndex(0) > (temp + 1) || sensors.getTempFByIndex(0) < (temp)) {
        sensors.requestTemperatures();
        if (sensors.getTempFByIndex(0) < temp){
          digitalWrite(AIN1, LOW); 
          digitalWrite(AIN2, HIGH); 
          analogWrite(PWMA, 170);

          // turn on red light to indicate cooling
          analogWrite(bluePin, 0);
          analogWrite(greenPin, 0);
          analogWrite(redPin, 255);

          sensors.requestTemperatures();
          Serial.println(sensors.getTempFByIndex(0));
        }
        else if (sensors.getTempFByIndex(0) > (temp - 1)) {
          digitalWrite(AIN1, HIGH);
          digitalWrite(AIN2, LOW); 
          analogWrite(PWMA, 170);

          // turn on blue light to indicate cooling
          analogWrite(redPin, 0);
          analogWrite(bluePin, 255);
          analogWrite(greenPin, 0);

          sensors.requestTemperatures();
          Serial.println(sensors.getTempFByIndex(0));
        }
      }
    }
    // turn on green light
    green = 255;
    blue = 0;
    red = 0;

    digitalWrite(AIN1, LOW); 
    digitalWrite(AIN2, LOW); 
    analogWrite(PWMA, 0);
  }
  else{
    // the light is white when first starting up
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
    analogWrite(redPin, red);

    //this turns peltier off
    digitalWrite(AIN1, LOW); 
    digitalWrite(AIN2, LOW); 
    analogWrite(PWMA, 0);
  }

  digitalWrite(STBY, HIGH);
  delay(1000);
}