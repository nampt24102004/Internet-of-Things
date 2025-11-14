#define BLYNK_TEMPLATE_ID "TMPL6sLB8DJ5j"
#define BLYNK_TEMPLATE_NAME "LED"
#define BLYNK_AUTH_TOKEN "L1ATOHnJ86F5bgF2xwgzRUAaZK9xDY6g"


#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


char ssid[] = "nam";
char pass[] = "11111111";



const int buttonPin = 13;
const int toggleSoundPin = 12;
const int soundPin = 34; 
const int relayPin = 26;
const int lm35Pin = 35;  
const int buzzerPin = 27;


bool relayState = false;
bool soundEnabled = false;
bool updateRelay = false;
bool updateSoundState = false;


bool manualBuzzer = false; 


unsigned long debounceTime = 0;
unsigned long debounceTimeToggle = 0;
unsigned long lastToggleTime = 0;


int soundToggleCounter = 0;
int lastSoundValue = -1;


const int threshold = 550;
const int delta_threshold = 150;


BlynkTimer timer;


volatile bool buttonPressed = false;
volatile bool toggleSoundPressed = false;


bool overheat = false;
unsigned long lastBlinkTime = 0;
bool blinkState = false;



void IRAM_ATTR handleButton() {
  if (millis() - debounceTime > 300) {
    buttonPressed = true;
    debounceTime = millis();
  }
}


void IRAM_ATTR handleToggleSound() {
  if (millis() - debounceTimeToggle > 300) {
    toggleSoundPressed = true;
    debounceTimeToggle = millis();
  }
}


// BLYNK 
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V0); // đèn
  Blynk.syncVirtual(V1); // sound sensor
  Blynk.syncVirtual(V3); // buzzer
}


BLYNK_WRITE(V0) {
  relayState = param.asInt();
  digitalWrite(relayPin, relayState ? LOW : HIGH);
  Serial.println("Blynk V0 - Relay: " + String(relayState ? "ON" : "OFF"));
}


BLYNK_WRITE(V1) {
  soundEnabled = param.asInt();
  Serial.println("Blynk V1 - Sound Sensor: " + String(soundEnabled ? "ON" : "OFF"));
}


BLYNK_WRITE(V3) {
  manualBuzzer = param.asInt();
  Serial.println("Blynk V3 - Buzzer: " + String(manualBuzzer ? "ON" : "OFF"));
}


//SOUND SENSOR 
void readAndSendSoundValue() {
  if (!soundEnabled || overheat) return;


  int soundValue = analogRead(soundPin);


  if (abs(soundValue - lastSoundValue) >= delta_threshold) {
    Blynk.virtualWrite(V2, soundValue);
    Serial.println("Sound Value: " + String(soundValue));
    lastSoundValue = soundValue;
  }


  if (soundValue > threshold && millis() - lastToggleTime > 1000) {
    soundToggleCounter++;
    if (soundToggleCounter >= 2) {
      relayState = !relayState;
      digitalWrite(relayPin, relayState ? LOW : HIGH);
      updateRelay = true;
      soundToggleCounter = 0;
    }
    lastToggleTime = millis();
  }
}


// NHIỆT ĐỘ (LM35) 
void readAndSendTemperature() {
  int analogValue = analogRead(lm35Pin);
  float voltage = analogValue * (3.3 / 4095.0);
  float temperature = voltage * 100.0;


  Blynk.virtualWrite(V4, (int)temperature);
  Serial.println("Temperature: " + String(temperature));


  if (temperature > 35.0) {
    overheat = true;
  } else {
    overheat = false;
    digitalWrite(relayPin, relayState ? LOW : HIGH); 
  }

  if (overheat || manualBuzzer) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }
}


void setup() {
  Serial.begin(115200);


  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(toggleSoundPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  pinMode(soundPin, INPUT);
  pinMode(lm35Pin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(relayPin, HIGH); 


  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(toggleSoundPin), handleToggleSound, FALLING);


  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);


  timer.setInterval(100L, readAndSendSoundValue);     
  timer.setInterval(500L, readAndSendTemperature);    
}


void loop() {
  Blynk.run();
  timer.run();


  if (buttonPressed) {
    relayState = !relayState;
    digitalWrite(relayPin, relayState ? LOW : HIGH);
    updateRelay = true;
    buttonPressed = false;
  }


  if (toggleSoundPressed) {
    soundEnabled = !soundEnabled;
    updateSoundState = true;
    toggleSoundPressed = false;
  }


  if (updateRelay) {
    Blynk.virtualWrite(V0, relayState ? 1 : 0);
    updateRelay = false;
  }


  if (updateSoundState) {
    Blynk.virtualWrite(V1, soundEnabled ? 1 : 0);
    Serial.println(soundEnabled ? "Sound Sensor: ON" : "Sound Sensor: OFF");
    updateSoundState = false;
  }


  if (overheat) {
    if (millis() - lastBlinkTime > 300) {
      blinkState = !blinkState;
      digitalWrite(relayPin, blinkState ? LOW : HIGH);
      lastBlinkTime = millis();
    }
  }
}
