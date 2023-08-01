// #include "button.h"
#include "icliq.h"
#include "ezButton.h"

const uint8_t batteryLevelPin = 17;
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const uint8_t rightButtonPin = 25; // Replace 12 with the GPIO pin number to which your button is connected
const uint8_t leftButtonPin = 27;
const uint8_t okButtonPin = 26;

ezButton rightButton(rightButtonPin, INPUT_PULLUP); // Create an instance of the ezButton class
ezButton leftButton(leftButtonPin, INPUT_PULLUP);
ezButton okButton(okButtonPin, INPUT_PULLUP);

uint8_t time_duration = 3;

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

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  displayLogo(2000); // Logo for two seconds
  delay(2000);
  displayBatteryPercentage(batteryLevelPin);

  display.clearDisplay();
  Serial.println("# Setup is done.");
  Serial.println(" ");
  // delay(3000);
  delay(300);
}

void loop()
{

  leftButton.loop(); // Call the loop method to update the button state
  rightButton.loop();
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
  display.clearDisplay();
}