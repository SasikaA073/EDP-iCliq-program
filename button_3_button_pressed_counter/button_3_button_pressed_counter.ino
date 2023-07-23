#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RESET -1 // Use -1 for boards without a reset pin

#define BUTTON_PIN 34 // Change this to your specific pin number

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


uint32_t buttonPressCount = 0;

void setup() {
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);
}

bool readButton() {
  static uint8_t buttonState = HIGH;
  static uint8_t lastButtonState = HIGH;
  static unsigned long lastDebounceTime = 0;
  const uint16_t debounceDelay = 50;

  // Read the state of the push button
  uint8_t buttonReading = digitalRead(BUTTON_PIN);

  // Check if the button state has changed
  if (buttonReading != lastButtonState) {
    lastDebounceTime = millis();
  }

  // Check if the button state has been stable for the debounce delay
  if (millis() - lastDebounceTime > debounceDelay) {
    // Update the button state only if it's different from the previous state
    if (buttonReading != buttonState) {
      buttonState = buttonReading;

      // Increment the button press count if the button is pressed
      if (buttonState == LOW) {
        buttonPressCount++;
        
        // Display the count on the OLED display
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("Button Press Count:");
        display.setCursor(0, 15);
        display.print(buttonPressCount);
        display.display();
      }

      // Display the count on the serial output
      Serial.print("Button Press Count: ");
      Serial.println(buttonPressCount);
    }
  }

  // Save the current button state for the next iteration
  lastButtonState = buttonReading;

  // Return the debounced button state
  return buttonState == LOW; // Return true when the button is pressed
}

void loop() {
  // Call the function to read the debounced button state
  readButton();
}
