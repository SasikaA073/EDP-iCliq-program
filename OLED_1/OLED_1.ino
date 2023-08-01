#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BUTTON_PIN 2
#define OLED_RESET_PIN 4

Adafruit_SSD1306 display(OLED_RESET_PIN);

unsigned long previousMillis = 0;
const unsigned long interval = 60000;  // Time in milliseconds

enum RGBColor {
  GREEN,
  YELLOW,
  RED
};

RGBColor currentColor = GREEN;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Time:");
  display.display();
}

void loop() {
  unsigned long currentMillis = millis();

  if (digitalRead(BUTTON_PIN) == LOW) {
    previousMillis = currentMillis;
  }

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    switch (currentColor) {
      case GREEN:
        currentColor = YELLOW;
        display.setTextColor(YELLOW);
        break;
      case YELLOW:
        currentColor = RED;
        display.setTextColor(RED);
        break;
      case RED:
        currentColor = GREEN;
        display.setTextColor(GREEN);
        break;
    }

    updateOLED();
  }
}

void updateOLED() {
  display.setCursor(0, 16);
  display.println(getFormattedTime());
  display.display();
}

String getFormattedTime() {
  unsigned long minutes = (millis() / 1000) / 60;
  unsigned int seconds = (millis() / 1000) % 60;
  return String(minutes) + " min " + String(seconds) + " sec";
}
