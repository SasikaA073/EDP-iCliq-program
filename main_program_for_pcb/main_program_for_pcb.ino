/* Program for PCB */

#include "icliq_pcb.h"
#include "ezButton.h"
#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

ezButton rightButton(rightButtonPin, INPUT_PULLUP); // Create an instance of the ezButton class
ezButton leftButton(leftButtonPin, INPUT_PULLUP);
ezButton okButton(okButtonPin, INPUT_PULLUP);

float first_time_flag = 1.0;
float second_time_flag = 2.0;
float third_time_flag = 3.0;

#define DEBOUNCE_TIME 70
#define UPPER_TIME_LIMIT 30.0

// To identify modes of the device
bool isCharging = false;
bool timeSetModeOn = false;
bool isLaserOn = false;
bool speechModeOn = false;

// Booleans to store to the serial output
bool printedToScreen_speechModeOn = true;
bool printedBluetoothDevice = true;

int okButtonPressedCount = 0;

// function to write text on the OLED screen
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

  // Initialize BLE Keyboard
  bleKeyboard.begin();

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
  // (2000); // Logo for two seconds
  for (int j = 0; j < 1; j++)
  {
    animate_android_loading();
  }
  Serial.println("# task between animation played.");

  Serial.println("# Setup is done.\n");
  Serial.println(" ");
  // delay(3000);
  delay(300);
  display.clearDisplay();
  display.display();
  // display.setTextSize(2);
}

void loop()
{
  Serial.print("Ok button pressed Count = ");
  Serial.println(okButtonPressedCount);

      if (isCharging == true)
  { //

    Serial.println("# Device is charging...");

    // TODO: Write a function to identify charging.
    display.clearDisplay();
    display.drawBitmap(0, 0, battery_charging, 128, 64, 1);
    display.display();
    delay(10);
  }

  // While not charging
  else if (isCharging == false)
  {

    // Serial.print("Speech Mode ");
    // Serial.println(speechModeOn);
    // main loop ---------------------------------------------------- Configuration for Ok Button ----------------------------------------------------------------------
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
        Serial.println("# main loop      + short press - okButton detected");
    }

    if (is_okButton_Pressing == true && is_okButton_LongDetected == false)
    {
      long okButton_pressDuration = millis() - okButton_pressedTime;

      if (okButton_pressDuration > LONG_PRESS_TIME)
      {
        Serial.println("# main loop       + long press - okButton detected");
        is_okButton_LongDetected = true;

        // Go to speech mode when Ok Button is long pressed.
        // speechModeOn == true;
        animate_android_loading();
        Serial.println("animate android loading after long press");
        delay(100);
      }
    }

    // main loop - config Ok button ends. ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Speech Mode ON
    // if (speechModeOn == true)
    // {

    //   if (printedToScreen_speechModeOn == true)
    //   {
    //     Serial.println("# Speech Mode on.");
    //     printedToScreen_speechModeOn = false;

    //     // TODO: Write a function to count down
    //     // TODO: Function to display the color in RGB
    //     // TODO: Function to vibrate the motor
    //   }
    // }

    // Speech Mode OFF
    // else if (speechModeOn == false) // While not charging
    // {

      leftButton.loop(); // Call the loop method to update the button state
      rightButton.loop();
      okButton.loop();

      // presentation mode On ---------------------------------------------------- Configuration for Ok Button ----------------------------------------------------------------------
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
      }

      if (is_okButton_Pressing == true && is_okButton_LongDetected == false)
      {
        long okButton_pressDuration = millis() - okButton_pressedTime;

        if (okButton_pressDuration > LONG_PRESS_TIME)
        {
          Serial.println("        + long press - okButton detected");
          is_okButton_LongDetected = true;

          speechModeOn = true;
          // animate_android_loading();
          delay(5);
        }
      }

      // ------------------------------------------------------------------------------------------------------------------------------------------------------------

      // ----- Code for mode change ...

      /*
      There are three modes that run in this device.
        - Speech Mode
          - vibrator will vibrates.

        - Presentation Mode

        - */
      if ((okButtonPressedCount % 5) == 0)
      {
        // Time View Mode
        //          - works bluetooth slides changing
        //          - works Laser

        // Serial.println("        # presentation Mode On. ");

        display.clearDisplay();

        display.setCursor(0, 0);
        display.print("Presentation Mode ");
        delay(5);
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

        if (bleKeyboard.isConnected())
        {

          // TODO: A function to show the bluetooth device connected.
          // TODO: Write the code to change the slides in a presentation.

          // Bluetooth learn tutorial - https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/#:~:text=The%20ESP32%20can%20act%20as,installed%20on%20the%20Arduino%20IDE.

          Serial.println("            + Bluetooth device is connected. ");

          if (printedBluetoothDevice == true)
          {
            display.clearDisplay();
            display.setCursor(0, 0);
            // display.print("Bluetooth device is connected.");
            display.display();

            printedBluetoothDevice = false;
            delay(1000);
          }

          if (rightButton.isPressed())
          {
            bleKeyboard.write(KEY_RETURN);
            delay(500);
            Serial.println("Moved to the next slide.");
          }
          if (leftButton.isPressed())
          {
            bleKeyboard.write(KEY_LEFT_ARROW);
            delay(500);
            Serial.println("Moved to the previous slide.");
          }
        }

        // ---------------------------------------------------- Configuration for rightButton ----------------------------------------------------------------------
        // short press, long press

        // TODO: Long press right button - laser ON
        // TODO: Long press left button - laser OFF
        // TODO: Short press right button - Go to next slide
        // TODO: short press right button - Go to previous slide
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
            first_time_flag -= 0.5;
            // changeRGBcolor(127,0,0,500);
          }
          // Do something in response to the button press
        }
        if (rightButton.isPressed())
        {
          Serial.println("right Button Pressed! - first time flag Mode On");
          if (first_time_flag < UPPER_TIME_LIMIT)
          {
            // Do something in response to the button press
            first_time_flag += 0.5;
            // changeRGBcolor(0,127,0,500);
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
          Serial.println("left Button Pressed! - first time flag Mode");
          if (second_time_flag > 0)
          {
            second_time_flag -= 0.5;
            // changeRGBcolor(127,0,0,500);
          }
          // Do something in response to the button press
        }
        if (rightButton.isPressed())
        {
          Serial.println("right Button Pressed! - first time flag Mode On");
          if (second_time_flag < UPPER_TIME_LIMIT)
          {
            // Do something in response to the button press
            second_time_flag += 0.5;
            // changeRGBcolor(0,127,0,500);
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
          Serial.println("left Button Pressed! - first time flag Mode");
          if (third_time_flag > 0)
          {
            third_time_flag -= 0.5;
            // changeRGBcolor(127,0,0,500);
          }
          // Do something in response to the button press
        }
        if (rightButton.isPressed())
        {
          Serial.println("right Button Pressed! - first time flag Mode On");
          if (third_time_flag < UPPER_TIME_LIMIT)
          {
            // Do something in response to the button press
            third_time_flag += 0.5;
            // changeRGBcolor(0,127,0,500);
          }
        }
      }


      // Speech Mode
      else if ((okButtonPressedCount % 5) == 4){
        if (rightButton.isPressed()){
          digitalWrite(laserPin, HIGH);

        }

      if (leftButton.isPressed()){
          digitalWrite(laserPin, LOW);
      }
      }
      display.clearDisplay();
    }
  }
// }

/* Algorithm *******************************************
okButtonsCount = 0

main loop():{

if okButton.pressed():
    okButtonsCount += 1

if okButtonsCount %4 == 0:
    print("You have pressed the OK button 4 times")
    run the program for slides change

    if rightButton.longPress detected:
        start Laser
    if leftButton.longPress detected:
        Switch off Laser


elif okButtonsCount %4 == 1:
    doSomething() first timeChange

elif okButtonsCount %4 == 2:
    doSomething() second timeChange

elif okButtonsCount %4 == 3:
    doSomething() thirdTimeChange

if okButtonLongPress detected:

    start countdowning

    run the program for slides change

    if rightButton.longPress detected:
        start Laser
    if leftButton.longPress detected:
        Switch off Laser

}

*/

// TODO: slides change
// TODO: change font type
// TODO: Count down mode
// TODO: Vibrator down
// TODO: Lasor On off
