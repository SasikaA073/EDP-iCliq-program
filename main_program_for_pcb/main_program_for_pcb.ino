/*

iCliQ - Company of Noobs

Developed by - Sasika Amarasinghe (https://github.com/SasikaA073)

*/

#include "icliq_pcb.h"
#include "ezButton.h"
#include <BleKeyboard.h>
#include <BLEDevice.h>

BleKeyboard bleKeyboard;

// ESP32Time rtc;
// ESP32Time rtc(3600); // offset in seconds GMT+1

ezButton rightButton(rightButtonPin, INPUT_PULLUP); // Create an instance of the ezButton class
ezButton leftButton(leftButtonPin, INPUT_PULLUP);
ezButton okButton(okButtonPin, INPUT_PULLUP);

int first_time_flag = 1;
int second_time_flag = 2;
int third_time_flag = 3;

#define DEBOUNCE_TIME 70
#define UPPER_TIME_LIMIT 30.0

// To identify modes of the device
bool isCharging = false;
bool timeSetModeOn = false;
bool isLaserOn = false;
bool speechModeOn = false;

bool firstTimeFlagPassed = false;
bool secondTimeFlagPassed = false;
bool thirdTimeFlagPassed = false;

bool timeCountModeOn = false;

uint8_t ledTimeDuration = 10;

// Booleans to store to the serial output
bool printedToScreen_speechModeOn = true;
bool printedBluetoothDevice = true;

long first_time_interval, second_time_interval, third_time_interval;
unsigned long previousMillis = 0;

int okButtonPressedCount = 0;

long start_time;

void setup()
{

  Serial.begin(115200);
  Serial.println("");
  Serial.println("# Starting iCliQ...");

  // Initialize leds & laser
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);
  pinMode(laserPin, OUTPUT);

  // Switch off the RGB light & laser
  analogWrite(RledPin, 255);
  analogWrite(GledPin, 255);
  analogWrite(BledPin, 255);
  digitalWrite(laserPin, LOW);
  Serial.println("- Switched off RGB light & Laser.");

  // Initialize buttons
  rightButton.setDebounceTime(DEBOUNCE_TIME); // Adjust the debounce time as needed (in milliseconds)
  rightButton.setCountMode(COUNT_FALLING);    // Change this to COUNT_RISING or COUNT_BOTH if needed

  leftButton.setDebounceTime(DEBOUNCE_TIME);
  leftButton.setCountMode(COUNT_FALLING); // when state change from high to low detect the press

  okButton.setDebounceTime(DEBOUNCE_TIME);
  okButton.setCountMode(COUNT_FALLING);

  BLEDevice::init("iCliQ");

  // Initialize BLE Keyboard
  bleKeyboard.begin();

  // Initialize Vibrator motor
  ledcSetup(PWMChannel, PWMFreq, PWMResolution);
  /* Attach the LED PWM Channel to the GPIO Pin */
  ledcAttachPin(vibratorPin, PWMChannel);

  // Initialize batteryPin
  pinMode(batteryLevelPin, INPUT);

  // Initialize vibratorPin
  pinMode(vibratorPin, OUTPUT);

  // Initialize touch output
  pinMode(touchUp, INPUT);
  pinMode(touchDown, INPUT);

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

  display.clearDisplay();
  display.display();

  displayBatteryPercentage(batteryLevelPin);

  display.clearDisplay();
  display.display();

  for (int j = 0; j < 1; j++)
  {
    animate_android_loading();
  }
  Serial.println("# task between animation played.");
  rtc.setTime(0, 0, 0, 5, 8, 2024);

  Serial.println("# Setup is done.\n");
  Serial.println(" ");
  // delay(3000);
  delay(300);
  display.clearDisplay();
  display.display();
}

void loop()
{

  // main loop ---------------------------------------------------- Configuration for Ok Button ----------------------------------------------------------------------
  display.clearDisplay();
  display.setTextSize(1);
  leftButton.loop(); // Call the loop method to update the button state
  rightButton.loop();
  okButton.loop();

  // ----------------------------------------- Configuration for Ok Button ----------------------------------------------------------------------
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
      Serial.println("        + short press - okButton detected");
    okButtonPressedCount += 1;
    delay(10);
    Serial.println("okButton pressed");
    Serial.print("okButtonPressedCount = ");
    Serial.println(okButtonPressedCount);
    if ((okButtonPressedCount % 5) == 0)
    {
      Serial.println("\n# Presentation Mode :  ");
      animate_android_loading();
    }
    else if ((okButtonPressedCount % 5) == 1)
    {
      Serial.println("\n# Time change Mode : flag 1 ");
    }
    else if ((okButtonPressedCount % 5) == 2)
    {
      Serial.println("\n# Time change Mode : flag 2 ");
     
    }
    else if ((okButtonPressedCount % 5) == 3)
    {
      Serial.println("\n# Time change Mode : flag 3 ");
      
    }
    else if ((okButtonPressedCount % 5) == 4)
    {
      Serial.println("\n# Speech Mode ");
    
      // Reset the RTC clock
      start_time = rtc.getMillis();
      rtc.setTime(0, 0, 0, 5, 8, 2024);

      // Serial.println(rtc.getTime());   //  (String) 15:24:38
      // Serial.println(rtc.getSecond()); //  (int)     38    (0-59)
      // Serial.println(rtc.getMinute());
    }
  }

  if (is_okButton_Pressing == true && is_okButton_LongDetected == false)
  {
    long okButton_pressDuration = millis() - okButton_pressedTime;

    if (okButton_pressDuration > LONG_PRESS_TIME)
    {
      Serial.println("        + long press - okButton detected");
      is_okButton_LongDetected = true;

      speechModeOn = true;
      delay(5);
    }
  }

  // ------------------------------------------------------------------------------------------------------------------------------------------------------------
  
  // ----- Code for mode change ...

  if ((okButtonPressedCount % 5) == 0)
  {

    display.clearDisplay();

    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print("Presentation Mode ");
    delay(5);

    if (bleKeyboard.isConnected())
    {
      display.setCursor(120, 0);
      display.print("B");
    }

    display.setTextSize(2);
    display.setCursor(24, 24);
    display.print(first_time_flag);
    delay(5);
    display.setCursor(56, 24);
    display.print(second_time_flag);
    delay(5);
    display.setCursor(88, 24);
    display.print(third_time_flag);
    delay(5);

    display.display();

    // Left button

    if (leftButton.isPressed())
    {
      leftButton_pressedTime = millis();
      is_leftButton_Pressing = true;
      is_leftButton_LongDetected = false;

      delay(10);
    }

    if (leftButton.isReleased())
    {
      is_leftButton_Pressing = false;
      leftButton_releasedTime = millis();
      delay(10);

      long leftButton_pressDuration = leftButton_releasedTime - leftButton_pressedTime;

      if (leftButton_pressDuration < SHORT_PRESS_TIME)
        Serial.println("        presentation mode - A short press:left Button is detected");

      // Run the vibrator motor
      for (int j = 0; j < 5; j++)
      {

        runVibratorMotor();
        Serial.println("presentation mode - Vibrator motor working!");
        
      }
    }

    if (rightButton.isPressed())
    {
      rightButton_pressedTime = millis();
      is_rightButton_Pressing = true;
      is_rightButton_LongDetected = false;
      delay(10);
    }

    if (rightButton.isReleased())
    {
      is_rightButton_Pressing = false;
      rightButton_releasedTime = millis();

      delay(10);

      long rightButton_pressDuration = rightButton_releasedTime - rightButton_pressedTime;

      if (rightButton_pressDuration < SHORT_PRESS_TIME)
        Serial.println("        presentation mode - A short press:right Button is detected");

      // Move to the next slide
      if (bleKeyboard.isConnected())
      {
        bleKeyboard.write(KEY_RIGHT_ARROW);
        delay(500);
        Serial.println("       presentation Mode - Moved to the next slide.");
      }
    }

    // Move to the previous slide
    if (bleKeyboard.isConnected() && leftButton.isPressed())
    {
      bleKeyboard.write(KEY_LEFT_ARROW);
      delay(500);
      Serial.println("      presentation Mode - Moved to the previous slide.");
    }

    // Switch on & off laser when a long press of right button is detected.
    if (is_rightButton_Pressing == true && is_rightButton_LongDetected == false)

    {
      long rightButton_pressDuration = millis() - rightButton_pressedTime;

      if (rightButton_pressDuration > LONG_PRESS_TIME)
      {
        Serial.println("Presentaion mode - A long press is detected");
        is_rightButton_LongDetected = true;
        delay(1000);

        if (isLaserOn == false)
        {
          digitalWrite(laserPin, HIGH);
          Serial.println("Laser is on");
          isLaserOn = true;
        }
        else if (isLaserOn == true)
        {
          digitalWrite(laserPin, LOW);
          Serial.println("Laser is off");
          isLaserOn = false;
        }

        Serial.println(isLaserOn);
      }
    }

   
  }

  // First time flag change mode
  else if ((okButtonPressedCount % 5) == 1)
  {

    // Change time flags accordingly.
    if (first_time_flag > second_time_flag)
    {
      second_time_flag = first_time_flag;
    }

    if (first_time_flag > third_time_flag)
    {
      third_time_flag = first_time_flag;
    }
    delay(10);

    // Serial.println("                - first Time flag change mode");
    display.clearDisplay();
    displayArrowKey(32, 8, 24, 16, 40, 16, true);
    delay(10);
    display.setTextSize(2);
    display.setCursor(24, 24);
    display.print(first_time_flag);
    delay(10);
    display.setCursor(56, 24);
    display.print(second_time_flag);
    delay(10);
    display.setCursor(88, 24);
    display.print(third_time_flag);
    displayArrowKey(32, 8, 24, 16, 40, 16, false);
    display.display();
    delay(10);

    if (leftButton.isPressed())
    {
      Serial.println("left Button Pressed! - first time flag Mode");
      if (first_time_flag > 0)
      {
        first_time_flag -= 1;
        // changeRGBcolor(127,0,0,500);
      }
       
    }
    if (rightButton.isPressed())
    {
      Serial.println("right Button Pressed! - first time flag Mode On");
      if (first_time_flag < UPPER_TIME_LIMIT)
      {
         
        first_time_flag += 1;
         
      }
    }
  }

  // Second time flag change mode
  else if ((okButtonPressedCount % 5) == 2)
  {

    // Change time flags accordingly
    if (second_time_flag > third_time_flag)
    {
      third_time_flag = second_time_flag;
    }

    // Serial.println("                - second Time flag change mode");
    display.clearDisplay();
    displayArrowKey(64, 8, 56, 16, 72, 16, true);
    delay(10);
    display.setTextSize(2);
    display.setCursor(24, 24);
    display.print(first_time_flag);
    delay(10);
    display.setCursor(56, 24);
    display.print(second_time_flag);
    delay(10);
    display.setCursor(88, 24);
    display.print(third_time_flag);
    displayArrowKey(64, 8, 56, 16, 72, 16, false);
    display.display();
    delay(10);

    if (leftButton.isPressed())
    {
      Serial.println("left Button Pressed! - second time flag Mode");
      if (second_time_flag > 0)
      {
        second_time_flag -= 1;
       
      }
      
    }
    if (rightButton.isPressed())
    {
      Serial.println("right Button Pressed! - second time flag Mode On");
      if (second_time_flag < UPPER_TIME_LIMIT)
      {
         
        second_time_flag += 1;
         
      }
    }
  }

  // Third time flag change mode
  else if ((okButtonPressedCount % 5) == 3)
  {

    // Serial.println("                - third Time flag change mode");
    display.clearDisplay();

    displayArrowKey(96, 8, 88, 16, 104, 16, true);
    delay(10);
    display.setTextSize(2);
    display.setCursor(24, 24);
    display.print(first_time_flag);
    delay(10);
    display.setCursor(56, 24);
    display.print(second_time_flag);
    delay(10);
    display.setCursor(88, 24);
    display.print(third_time_flag);
    displayArrowKey(96, 8, 88, 16, 104, 16, false);
    display.display();
    delay(10);

    if (leftButton.isPressed())
    {
      Serial.println("left Button Pressed! - third time flag Mode");
      if (third_time_flag > 0)
      {
        third_time_flag -= 1;
        // changeRGBcolor(127,0,0,500);
      }
       
    }
    if (rightButton.isPressed())
    {
      Serial.println("right Button Pressed! - third time flag Mode On");
      if (third_time_flag < UPPER_TIME_LIMIT)
      {
         
        third_time_flag += 1;
         
      }
    }
  }

  // Speech Mode
  else if ((okButtonPressedCount % 5) == 4)
  {

 
    if ((rtc.getMinute() == first_time_flag) && (!firstTimeFlagPassed))
   
    {
      // Green color (255,0,255)
      analogWrite(GledPin, 0);

      Serial.println("First time flag passed");
      firstTimeFlagPassed = true;
      delay(ledTimeDuration * 1000);
      analogWrite(GledPin, 255);
    }

    // Second time flag passing
    if ((rtc.getMinute() == second_time_flag) && (!secondTimeFlagPassed))
    {
      // Yellow color (0,0,255)
      analogWrite(RledPin, 0);
      analogWrite(GledPin, 128);
      Serial.println("Second time flag passed");
      secondTimeFlagPassed = true;
      delay(ledTimeDuration * 1000);
      analogWrite(RledPin, 255);
      analogWrite(GledPin, 255);
    }

    // Third time flag passing
    if ((rtc.getMinute() >= third_time_flag) && (!thirdTimeFlagPassed))
    {
      // Red color (0,255,255)
      analogWrite(RledPin, 0);
      Serial.println("Third time flag passed");
      thirdTimeFlagPassed = true;
      delay(ledTimeDuration * 1000);
      analogWrite(RledPin, 255);
    }

    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print("Speech Mode ");

    if (bleKeyboard.isConnected())
    {
      display.setCursor(120, 0);
      display.print("B");
    }

    // updateOLEDStartTime();
    updateOLED();
    display.display();

    unsigned long currentMillis = millis();

    // ------------------------------------------------------------------------------------------
    if (rightButton.isPressed())
    {
      rightButton_pressedTime = millis();
      is_rightButton_Pressing = true;
      is_rightButton_LongDetected = false;
      delay(10);
    }

    if (rightButton.isReleased())
    {
      is_rightButton_Pressing = false;
      rightButton_releasedTime = millis();

      delay(10);

      long rightButton_pressDuration = rightButton_releasedTime - rightButton_pressedTime;

      if (rightButton_pressDuration < SHORT_PRESS_TIME)
        Serial.println("        Speech mode - A short press:right Button is detected");

      // Move to the next slide
      if (bleKeyboard.isConnected())
      {
        bleKeyboard.write(KEY_RIGHT_ARROW);
        delay(500);
        Serial.println("       Speech Mode - Moved to the next slide.");
      }
    }

    // Switch on & off laser when a long press of right button is detected.
    if (is_rightButton_Pressing == true && is_rightButton_LongDetected == false)

    {
      long rightButton_pressDuration = millis() - rightButton_pressedTime;

      if (rightButton_pressDuration > LONG_PRESS_TIME)
      {
        Serial.println("Speech mode - A long press is detected");
        is_rightButton_LongDetected = true;
        delay(1000);

        if (isLaserOn == false)
        {
          digitalWrite(laserPin, HIGH);
          Serial.println("Laser is on");
          isLaserOn = true;
        }
        else if (isLaserOn == true)
        {
          digitalWrite(laserPin, LOW);
          Serial.println("Laser is off");
          isLaserOn = false;
        }

        Serial.println(isLaserOn);
      }
      // --------------------------------------------------------------------------------------------

      if (leftButton.isPressed())
      {
        leftButton_pressedTime = millis();
        is_leftButton_Pressing = true;
        is_leftButton_LongDetected = false;
        delay(10);
      }

      if (leftButton.isReleased())
      {
        is_leftButton_Pressing = false;
        leftButton_releasedTime = millis();

        delay(10);

        long leftButton_pressDuration = leftButton_releasedTime - leftButton_pressedTime;

        if (leftButton_pressDuration < SHORT_PRESS_TIME)
          Serial.println("        Speech mode - A short press:right Button is detected");

        // Move to the next slide
        if (bleKeyboard.isConnected())
        {
          bleKeyboard.write(KEY_LEFT_ARROW);
          delay(500);
          Serial.println("       Speech Mode - Moved to the previous slide.");
        }
      }

      // Switch on & off laser when a long press of right button is detected.
      if (is_leftButton_Pressing == true && is_leftButton_LongDetected == false)

      {
        long leftButton_pressDuration = millis() - leftButton_pressedTime;

        if (leftButton_pressDuration > LONG_PRESS_TIME)
        {
          Serial.println("Speech mode - Left button - long press is detected");
          is_leftButton_LongDetected = true;
          delay(1000);

          if (timeCountModeOn == false)
          {
            Serial.println("Speech mode - Start counting up");

            timeCountModeOn = true;
            delay(1000);
          }
        }
      }

      // -----------------------------------------------------------------------------------------------

    }
  }
  display.clearDisplay();
}



String getFormattedStartTime()
{
  unsigned long minutes = (millis() / 1000) / 60;
  unsigned int seconds = ((millis() - start_time) / 1000) % 60;
  if (minutes < 1.0)
  {
    minutes = 0.0;
  }
  return String(minutes) + " : " + String(seconds);
}

void updateOLEDStartTime()
{
 
  display.setCursor(32, 16);
  display.println(getFormattedStartTime());
  delay(10);

}
