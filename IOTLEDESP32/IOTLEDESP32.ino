#define BLYNK_TEMPLATE_ID "TMPL6sLB8DJ5j"
#define BLYNK_TEMPLATE_NAME "LED"
#define BLYNK_AUTH_TOKEN "L1ATOHnJ86F5bgF2xwgzRUAaZK9xDY6g"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "jhin";
char pass[] = "12345678";

const int buttonPin = 13;
const int relayPin = 26;
const int soundPin = 34;  // Cảm biến âm thanh nối chân D34

bool update = false;
unsigned long debounceTime = 0;
bool relayState = false;

void IRAM_ATTR handleButton() {
  if (millis() - debounceTime > 300) {
    relayState = !relayState;
    digitalWrite(relayPin, relayState ? LOW : HIGH);
    update = true;
    debounceTime = millis();
  }
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V1);
}

BLYNK_WRITE(V1) {
  int state = param.asInt();
  relayState = (state == 1);
  digitalWrite(relayPin, relayState ? LOW : HIGH);
}

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  pinMode(soundPin, INPUT);

  digitalWrite(relayPin, HIGH); // Mặc định tắt (vì relay LOW là bật)

  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButton, FALLING);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  int soundValue = analogRead(soundPin);
  if (soundValue > 500) {
    Serial.println("Phát hiện âm thanh lớn: " + String(soundValue));
    delay(200);  // Tránh bắt liên tục nhiều lần

    // Đảo trạng thái đèn khi phát hiện âm thanh lớn
    relayState = !relayState;
    digitalWrite(relayPin, relayState ? LOW : HIGH);
    update = true;
  }

  Blynk.run();

  if (update) {
    Blynk.virtualWrite(V1, relayState ? 1 : 0);
    update = false;
  }
}
