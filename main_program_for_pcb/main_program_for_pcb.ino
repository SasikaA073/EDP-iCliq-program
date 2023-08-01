/* Program for PCB */

// #include "button.h"
#include "icliq.h"
#include "ezButton.h"

const uint8_t batteryLevelPin = 17;
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const uint8_t rightButtonPin = 25; // Replace 12 with the GPIO pin number to which your button is connected
const uint8_t leftButtonPin = 27;
const uint8_t okButtonPin = 26;

const int SHORT_PRESS_TIME = 1000; // 1000 milliseconds
const int LONG_PRESS_TIME = 1000;  // 1000 milliseconds

ezButton rightButton(rightButtonPin, INPUT_PULLUP); // Create an instance of the ezButton class
ezButton leftButton(leftButtonPin, INPUT_PULLUP);
ezButton okButton(okButtonPin, INPUT_PULLUP);

uint8_t time_duration = 3;

// To identify modes of the device
bool isCharging = false;
bool timeSetModeOn = false;

unsigned long okButton_pressedTime = 0;
unsigned long okButton_releasedTime = 0;
bool is_okButton_Pressing = false;
bool is_okButton_LongDetected = false;

// function to write text on the OLED screen
void setup()
{

  // Initialize buttons
  rightButton.setDebounceTime(50);         // Adjust the debounce time as needed (in milliseconds)
  rightButton.setCountMode(COUNT_FALLING); // Change this to COUNT_RISING or COUNT_BOTH if needed

  leftButton.setDebounceTime(50);
  leftButton.setCountMode(COUNT_FALLING); // when state change from high to low detect the press

  okButton.setDebounceTime(50);
  okButton.setCountMode(COUNT_FALLING);

  // Initialize leds

  Serial.begin(115200);
  Serial.println("");
  Serial.println("# Starting iCliQ...");

  // // Display the logo
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ;
  }

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // (2000); // Logo for two seconds
  for (int j = 0; j < 1; j++)
  {
    animate_hexagon_loading();
  }

  // delay(2000);

  display.clearDisplay();
  display.display();

  displayBatteryPercentage(batteryLevelPin);

  display.clearDisplay();
  display.display();
  // (2000); // Logo for two seconds
  for (int j = 0; j < 1; j++)
  {
    animate_android_loading();
  }

  Serial.println("# Setup is done.");
  Serial.println(" ");
  // delay(3000);
  delay(300);
  display.clearDisplay();
  display.display();
  display.setTextSize(2);
}

void loop()
{

  if (isCharging == true)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0, battery_charging, 128, 64, 1);
    display.display();
    delay(10);
  }
  else
  {
    leftButton.loop(); // Call the loop method to update the button state
    rightButton.loop();
    okButton.loop();

    // ---------------------------------------------------- Configuration for Ok Button ----------------------------------------------------------------------
    // short press, long press
    if (okButton.isPressed())
    {
      okButton_pressedTime = millis();

      is_okButton_Pressing = true;
      is_okButton_LongDetected = false;
      
    }

    if (okButton.isReleased())
    {
      is_okButton_Pressing = false;
      okButton_releasedTime = millis();

      long okButton_pressDuration = okButton_releasedTime - okButton_pressedTime;

      if (okButton_pressDuration < SHORT_PRESS_TIME)
        Serial.println("A short press in okButton detected");
      display.setCursor(0, 0);
      // display.print("A short press is detected.");
      timeSetModeOn =! timeSetModeOn;
      // display.display();
      // delay(10);

      delay(100);
      animate_android_loading();
    }

    if (is_okButton_Pressing == true && is_okButton_LongDetected == false)
    {
      long okButton_pressDuration = millis() - okButton_pressedTime;

      if (okButton_pressDuration > LONG_PRESS_TIME)
      {
        Serial.println("A long press in OkButton detected");
        is_okButton_LongDetected = true;
        display.setCursor(0, 0);
        display.print("A long press is detected.");
        display.display();
        delay(10);
      }
    }

    if (timeSetModeOn == true)
    {

      displayArrowKey(64, 8, 56, 16, 72, 16, true);
      delay(10);
      display.setCursor(56, 24);
      display.print(time_duration);
      delay(10);
      displayArrowKey(64, 8, 56, 16, 72, 16, false);
      display.display();
      delay(10);
      // changeRGBcolor(0,50,50,100);

      if (leftButton.isPressed())
      {
        Serial.println("left Button Pressed!");
        if (time_duration > 0)
        {
          time_duration -= 1;
          // changeRGBcolor(127,0,0,500);
        }
        // Do something in response to the button press
      }
      if (rightButton.isPressed())
      {
        Serial.println("right Button Pressed!");
        if (time_duration < 30)
        {
          // Do something in response to the button press
          time_duration += 1;
          // changeRGBcolor(0,127,0,500);
        }
      }
    }

    else if (timeSetModeOn == false)
    {
      display.setCursor(56, 24);
      display.print(time_duration);
      delay(10);
      display.display();
      delay(10);
    }

    display.clearDisplay();
  }
}
