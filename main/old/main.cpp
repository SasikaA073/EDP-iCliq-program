#include "button.h"
#include "icliq.h"

const uint8_t batteryLevelPin = 17;
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define LEFT_BTN 35
#define RIGHT_BTN 34
#define OK_BTN 26

Button btn_left;
Button btn_right;
Button btn_ok;

uint8_t time_duration = 3;

// function to write text on the OLED screen
void setup()
{

  // Initialize buttons
  btn_left.begin(LEFT_BTN);
  btn_right.begin(RIGHT_BTN);
  btn_ok.begin(OK_BTN);

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

pinMode(LEFT_BTN, INPUT_PULLUP);
pinMode(RIGHT_BTN, INPUT_PULLUP);

}

void loop()
{


  // displayArrowKey(64, 8, 56, 16, 72, 16, true);
  // delay(10);
  // display.setCursor(60, 24);
  // display.print(time_duration);
  // delay(10);
  // displayArrowKey(64, 8, 56, 16, 72, 16, false);
  // display.display();
  // delay(10);


  if (digitalRead(LEFT_BTN) == LOW){
    Serial.println("Left btn clicked ");
  }
  if (digitalRead(RIGHT_BTN) == LOW){
    Serial.println("Right btn clicked");
  }

  delay(1000);
}