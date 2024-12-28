#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

// Define the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Address 0x27, 16 columns, 2 rows

// Servo motor pins for Arduino Uno
const int myServoDoorPin = A0;
const int myServoWindowLeftPin = 11;
const int myServoWindowRightPin = 12;

// Initialize servos
Servo myServoDoor;

// Define the keypad dimensions
const byte ROWS = 4;
const byte COLS = 4;

// Keypad pin connections for Arduino Uno
uint8_t rowPins[ROWS] = { 2, 3, 4, 5 };  // Connect to the row pins of the keypad
uint8_t colPins[COLS] = { 6, 7, 8, 9 };  // Connect to the column pins of the keypad

// Define the keymap for the keypad
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

// Initialize the keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Define PIN parameters
String inputPin = "";
String correctPin = "1234";
int enteredPinCount = 0;

char customKey = "";
// Servo angles
const int closeDoorAngle = 0;
const int openDoorAngle = 90;

const int closeWindowAngleLeft = 140;
const int openWindowAngleLeft = 50;

const int closeWindowAngleRight = 0;
const int openWindowAngleRight = 90;

// Timers
unsigned long previousMillis = 0;
const long interval = 10000;  // 10 seconds
bool welcomeDisplayed = false;

void LCD(){
  if(!welcomeDisplayed){
    for(int x=0;x<8;x++){
      lcd.setCursor(x, 0);
      lcd.print("Welcome!");
      delay(150);
      lcd.clear();
    } 
    welcomeDisplayed = true;
  } 
  else {
      for(int x=0; x<8;x++){
        if(customKey) break;
        else{
          lcd.setCursor(x, 0);
          lcd.print("Welcome!");
          delay(150);
          lcd.clear();
        }
      }
  }
  
}
void setup() {
  Serial.begin(9600);  // Start Serial communication

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize servos
  myServoDoor.attach(myServoDoorPin);

  // Set servos to default positions
  myServoDoor.write(closeDoorAngle);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome!");

  //LCD();
}

void loop() {
  customKey = customKeypad.getKey();  // Get keypad input
  unsigned long currentMillis = millis();

  if (customKey) {
    if (enteredPinCount == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Pin:");
    }

    enteredPinCount++;
    inputPin += customKey;  // Append the key pressed
    Serial.print(customKey);

    // Display the entered PIN
    lcd.setCursor(0, 1);
    lcd.print(inputPin);

    previousMillis = currentMillis;  // Reset timer

    // Check PIN after 4 digits entered
    if (enteredPinCount == 4) {
      if (inputPin == correctPin) {
        Serial.println("Correct PIN!");
        // Open door and windows
        myServoDoor.write(openDoorAngle);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Correct Pin!");

      } else {
        Serial.println("Incorrect PIN!");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Incorrect!");
      }

      // Reset input
      inputPin = "";
      enteredPinCount = 0;
    }
  }

  // Reset system after timeout
  if ((currentMillis - previousMillis) >= interval) {
    enteredPinCount = 0;
    inputPin = "";
    previousMillis = currentMillis;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome!");
    
    //if(inputPin == "") LCD();


    // Reset servos to closed positions
    myServoDoor.write(closeDoorAngle);
  }
}
