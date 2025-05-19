#include <Wire.h>
#include <Keypad.h>
#include <ESP32Servo.h>
#include <DHT.h>

#define BLYNK_TEMPLATE_ID "TMPL6c0MXaBxy"
#define BLYNK_TEMPLATE_NAME "HomeESP32"
#define BLYNK_PRINT Serial

#define BOTtoken "7913234088:AAH3sGTCLY46rSpx7PvfGgrtIexKRpkkLKw"
#define CHAT_ID "8156767868"

#define DHTPIN 15
#define DHTTYPE DHT11

#define ENB 23
#define IN3 26 
#define IN4 25  

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "jXMURT4w-0XHTzW7xJHCfI6ozlzlfXtc";
char ssid[] = "Pixel_1259";
char pass[] = "password12";

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

const int myServoWindowLeftPin = 18;
const int myServoWindowRightPin = 19;

Servo myServoWindowLeft;
Servo myServoWindowRight;

DHT dht(DHTPIN, DHTTYPE);

const int closeWindowAngleLeft = 140;
const int openWindowAngleLeft = 50;

const int closeWindowAngleRight = 0;
const int openWindowAngleRight = 90;

const int MQ2Pin = 34; 
const int rainDropPin = 35;
const int warningLed = 16;
const int buzzerPin = 4;

int redPin = 13;
int greenPin = 12;
int bluePin = 14;

BLYNK_WRITE(V0){
  // Left Window
  int windowLeft = param.asInt(); // Get value from Blynk
  Serial.println(windowLeft);

  myServoWindowLeft.write(windowLeft);
}

BLYNK_WRITE(V1){
  // Right Window
  int windowRight = param.asInt(); // Get value from Blynk
  Serial.println(windowRight);

  myServoWindowRight.write(windowRight);
}

BLYNK_WRITE(V2) {
  int fanState = param.asInt();

  if(fanState == 0){
    Serial.println("Fan is OFF");

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0); 

  } else if(fanState == 1){
    Serial.println("Fan is ON");
    
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 255); 
  }

}

BLYNK_WRITE(V3){
  int rgbModuleColor = param.asInt();

  switch(rgbModuleColor){
    case 1:
    Serial.println("RGB OFF");
    analogWrite(redPin, 0);     
    analogWrite(greenPin, 0);  
    analogWrite(bluePin, 0);  
    break;

    case 2:
    Serial.println("RGB YELLOW");
    analogWrite(redPin, 255);     
    analogWrite(greenPin, 255);  
    analogWrite(bluePin, 0);  
    break;

    case 3:
    Serial.println("RGB GREEN");
    analogWrite(redPin, 255);     
    analogWrite(greenPin, 0);  
    analogWrite(bluePin, 255);  
    break;

    case 4:
    Serial.println("RGB RED");
    analogWrite(redPin, 255);     
    analogWrite(greenPin, 0);  
    analogWrite(bluePin, 0);  
    break;

    case 5:
    Serial.println("RGB RED");
  }
}

const int bathroomLedPin = 21;

BLYNK_WRITE(V4){
  int bathroomLedState = param.asInt();

  if(bathroomLedState == 0){
    Serial.println("Restroom LED OFF");
    digitalWrite(bathroomLedPin, LOW);;
  } else if(bathroomLedState == 1){
    Serial.println("Restroom LED ON");
    digitalWrite(bathroomLedPin, HIGH);
  }

}

void setup() {
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  bot.sendMessage(CHAT_ID, "Smart Home ESP32 is now online.");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.begin(115200);

  myServoWindowLeft.attach(myServoWindowLeftPin);
  myServoWindowRight.attach(myServoWindowRightPin);

  myServoWindowLeft.write(closeWindowAngleLeft);
  myServoWindowRight.write(closeWindowAngleRight);

  dht.begin();

  pinMode(DHTPIN, INPUT);
  pinMode(MQ2Pin, INPUT); 
  pinMode(rainDropPin, INPUT); 

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(warningLed, OUTPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(buzzerPin, OUTPUT);
  pinMode(bathroomLedPin, OUTPUT);
}

unsigned long previousMillis = 0;  // Stores the last time the message was updated
const long interval = 10000;
bool welcomeDisplayed = false;

int checkIfRepeating = 0;

void loop() {
  Blynk.run();

  // SENSORS EXTRACTION
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  int mq2Value = analogRead(MQ2Pin);
  int rainDropValue = analogRead(rainDropPin);

  // DISPLAY AND BLYNK
  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Humidity: ");
    Serial.print(humidity); 
    Serial.print("%\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
    
    Blynk.virtualWrite(V6, temperature);
    Blynk.virtualWrite(V7, humidity);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  Serial.print("MQ-2 Gas Sensor Value: ");
  Serial.println(mq2Value);

  if(mq2Value > 1000){
    checkIfRepeating++;
  } else {
    digitalWrite(warningLed, LOW);
    checkIfRepeating = 0;
  }

  if(temperature > 34){
    Blynk.virtualWrite(V2, 1);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 255); 
  }

  if(checkIfRepeating > 1){
    // Will only be called if it's consistently > 1000 twice
    bot.sendMessage(CHAT_ID, "WARNING: Dangerous Gas Concentration Detected!");
    bot.sendMessage(CHAT_ID, "Fan and Windows have been opened for ventilation.");
    Serial.println("Dangerous Gas Concentration Detected");

    // Turn on warning LED
    digitalWrite(warningLed, HIGH);

    Blynk.virtualWrite(V0, openWindowAngleLeft);
    Blynk.virtualWrite(V1, openWindowAngleRight);

    myServoWindowLeft.write(openWindowAngleLeft);
    myServoWindowRight.write(openWindowAngleRight);

    // Buzzer warning sounds
    tone(buzzerPin, 2000, 10000);

    // Turn on FAN
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, 255); 
    delay(15000);

    // Turn off buzzer
    noTone(buzzerPin);

    // Turn off FAN
    Blynk.virtualWrite(V2, 0);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0); 

    checkIfRepeating = 0;
  }
  
  Blynk.virtualWrite(V8, mq2Value);

  Serial.print("Raindrop Value: ");
  Serial.println(rainDropValue);
  //Blynk.virtualWrite(V9, rainDropValue);

  if(rainDropValue > 2000){
    Serial.println("Rain Detected!");

    bot.sendMessage(CHAT_ID, "Reminder: Rain has been detected!");
    bot.sendMessage(CHAT_ID, "We closed your windows for you.");

    Blynk.virtualWrite(V0, closeWindowAngleLeft);
    Blynk.virtualWrite(V1, closeWindowAngleRight);

    myServoWindowLeft.write(closeWindowAngleLeft);
    myServoWindowRight.write(closeWindowAngleRight);
  }

  delay(2000);
}
