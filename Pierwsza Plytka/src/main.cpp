#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  Serial.begin(9600);
  int result = myFunction(2, 3);
}

void loop() {
  Serial.print("cccc");
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}