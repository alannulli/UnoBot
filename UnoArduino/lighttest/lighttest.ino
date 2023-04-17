const int MAX_DATA = 10; // Maximum number of data points to store
int data[MAX_DATA]; // Array to store data
int num_data = 0; // Number of data points stored so far

int redPin = 11;
int greenPin = 10;
int bluepin = 9;

void setup() {
  Serial.begin(115200); // Initialize serial communication
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluepin, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    int val = Serial.parseInt(); // Read the next integer from the serial port
    if (num_data < MAX_DATA) {
      data[num_data] = val; // Store the data in the array
      num_data++; // Increment the number of data points stored
    }
  }

  String serialListener = Serial.readString();
    Serial.println(serialListener);
    if (serialListener == "ON") {
        analogWrite(redPin, 255);
    }
    else if (serialListener == "OFF") {
        analogWrite(redPin, 0);
    } 
  
}