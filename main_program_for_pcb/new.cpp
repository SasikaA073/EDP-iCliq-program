// /* Program for PCB */

// #include "icliq_pcb.h"
// #include "ezButton.h"
// #include <BleKeyboard.h>

// BleKeyboard bleKeyboard;

// ezButton rightButton(rightButtonPin, INPUT_PULLUP); // Create an instance of the ezButton class
// ezButton leftButton(leftButtonPin, INPUT_PULLUP);
// ezButton okButton(okButtonPin, INPUT_PULLUP);

// float first_time_flag = 1.0;
// float second_time_flag = 2.0;
// float third_time_flag = 3.0;

// #define DEBOUNCE_TIME 100
// #define UPPER_TIME_LIMIT 30.0

// // To identify modes of the device
// bool isCharging = false;
// bool timeSetModeOn = false;
// bool isLaserOn = false;
// bool speechModeOn = false;

// // Booleans to store to the serial output
// bool printedToScreen_speechModeOn = true;
// bool printedBluetoothDevice = true;

// long first_time_interval;
// unsigned long previousMillis = 0;

// int okButtonPressedCount = 0;

// // function to write text on the OLED screen
// void setup()
// {

//   Serial.begin(115200);
//   Serial.println("");
//   Serial.println("# Starting iCliQ...");

//   // Initialize leds & laser
//   pinMode(RledPin, OUTPUT);
//   pinMode(GledPin, OUTPUT);
//   pinMode(BledPin, OUTPUT);
//   pinMode(laserPin, OUTPUT);

//   // Switch off the RGB light & laser
//   analogWrite(RledPin, 255);
//   analogWrite(GledPin, 255);
//   analogWrite(BledPin, 255);
//   digitalWrite(laserPin, LOW);
//   Serial.println("- Switched off RGB light & Laser.");

//   // Initialize buttons
//   rightButton.setDebounceTime(DEBOUNCE_TIME); // Adjust the debounce time as needed (in milliseconds)
//   rightButton.setCountMode(COUNT_FALLING);    // Change this to COUNT_RISING or COUNT_BOTH if needed

//   leftButton.setDebounceTime(DEBOUNCE_TIME);
//   leftButton.setCountMode(COUNT_FALLING); // when state change from high to low detect the press

//   okButton.setDebounceTime(DEBOUNCE_TIME);
//   okButton.setCountMode(COUNT_FALLING);

//   // Initialize BLE Keyboard
//   bleKeyboard.begin();

//   // Initialize batteryPin
//   pinMode(batteryLevelPin, INPUT);

//   // Initialize vibratorPin
//   pinMode(vibratorPin, OUTPUT);

//   // Initialize touch output
//   pinMode(touchUp, INPUT);
//   pinMode(touchDown, INPUT);

//   // // Display the logo
//   if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
//   {
//     Serial.println("SSD1306 allocation failed");
//     for (;;)
//       ;
//   }

//   display.setTextSize(1);
//   display.setTextColor(SSD1306_WHITE);

//   // (2000); // Logo for two seconds
//   for (int j = 0; j < 1; j++)
//   {
//     animate_hexagon_loading();
//   }

//   display.clearDisplay();
//   display.display();

//   displayBatteryPercentage(batteryLevelPin);

//   display.clearDisplay();
//   display.display();
//   // (2000); // Logo for two seconds
//   for (int j = 0; j < 1; j++)
//   {
//     animate_android_loading();
//   }
//   Serial.println("# task between animation played.");

//   Serial.println("# Setup is done.\n");
//   Serial.println(" ");
//   // delay(3000);
//   delay(300);
//   display.clearDisplay();
//   display.display();
//   // display.setTextSize(2);
// }

// void loop()
// {
//   // Serial.print("Ok button pressed Count = ");
//   // Serial.println(okButtonPressedCount);

//   // Call the loop method to update the button state
//   leftButton.loop();
//   rightButton.loop();
//   okButton.loop();

//   // ----- Mode changes

//   if (okButton.isPressed())
//   {
//     okButton_pressedTime = millis();
//     is_okButton_Pressing = true;
//     is_okButton_LongDetected = false;
//   }

//   if (okButton.isReleased())
//   {
//     is_okButton_Pressing = false;
//     okButton_releasedTime = millis();

//     long okButton_pressDuration = okButton_releasedTime - okButton_pressedTime;

//     if (okButton_pressDuration < SHORT_PRESS_TIME)
//     {
//       Serial.println(" Speech Mode    + short press - okButton detected");
//       okButtonPressedCount += 1;

//       okButtonPressedCount += 1;
//       delay(10);
//       Serial.println("okButton pressed");
//       Serial.print("okButtonPressedCount = ");
//       Serial.println(okButtonPressedCount);
//       if ((okButtonPressedCount % 5) == 0)
//       {
//         Serial.println("\n# Presentation Mode :  ");
//       }
//       else if ((okButtonPressedCount % 5) == 1)
//       {
//         Serial.println("\n# Time change Mode : flag 1 ");
//       }
//       else if ((okButtonPressedCount % 5) == 2)
//       {
//         Serial.println("\n# Time change Mode : flag 2 ");
//       }
//       else if ((okButtonPressedCount % 5) == 3)
//       {
//         Serial.println("\n# Time change Mode : flag 3 ");
//       }
//       else if ((okButtonPressedCount % 5) == 4)
//       {
//         Serial.println("\n# Speech Mode ");
//       }
//     }
//   }

//   if (is_okButton_Pressing == true && is_okButton_LongDetected == false)
//   {
//     long okButton_pressDuration = millis() - okButton_pressedTime;

//     if (okButton_pressDuration > LONG_PRESS_TIME)
//     {
//       Serial.println("        + long press - okButton detected");
//       is_okButton_LongDetected = true;

//       // speechModeOn = true;
//       // animate_android_loading();
//       delay(5);
//     }
//   }

//   //  Presentation Mode
//   if ((okButtonPressedCount % 5) == 0)
//   { // ------------------- config for slides change & laser on/off --------------------------------------------

//     if (rightButton.isPressed())
//     {
//       rightButton_pressedTime = millis();
//       is_rightButton_Pressing = true;
//       is_rightButton_LongDetected = false;
//       delay(10);
//     }

//     if (rightButton.isReleased())
//     {
//       is_rightButton_Pressing = false;
//       rightButton_releasedTime = millis();

//       delay(10);

//       long rightButton_pressDuration = rightButton_releasedTime - rightButton_pressedTime;

//       if (rightButton_pressDuration < SHORT_PRESS_TIME)
//         Serial.println("        presentation mode - A short press:right Button is detected");

//       // Move to the next slide
//       if (bleKeyboard.isConnected())
//       {
//         bleKeyboard.write(KEY_RIGHT_ARROW);
//         delay(500);
//         Serial.println("       presentation Mode - Moved to the next slide.");
//       }
//     }

//     // Move to the previous slide
//     if (bleKeyboard.isConnected() && leftButton.isPressed())
//     {
//       bleKeyboard.write(KEY_LEFT_ARROW);
//       delay(500);
//       Serial.println("      presentation Mode - Moved to the previous slide.");
//     }

//     // Switch on & off laser when a long press of right button is detected.
//     if (is_rightButton_Pressing == true && is_rightButton_LongDetected == false)

//     {
//       long rightButton_pressDuration = millis() - rightButton_pressedTime;

//       if (rightButton_pressDuration > LONG_PRESS_TIME)
//       {
//         Serial.println("Presentaion mode - A long press is detected");
//         is_rightButton_LongDetected = true;
//         isLaserOn != isLaserOn;
//         digitalWrite(laserPin, isLaserOn);
//         Serial.print("Presentaion mode - Laser  : ");
//         Serial.print(isLaserOn);
//       }
//     }

//     // ------------------------------config for right button----------------------------------------------------------------------------

//     // Serial.println("        # presentation Mode On. ");

//     display.clearDisplay();

//     display.setCursor(0, 0);
//     display.print("Presentation Mode ");
//     delay(5);
//     display.setCursor(24, 24);
//     display.print(first_time_flag);
//     delay(5);
//     display.setCursor(56, 24);
//     display.print(second_time_flag);
//     delay(5);
//     display.setCursor(88, 24);
//     display.print(third_time_flag);
//     delay(5);

//     display.display();
//   }

//   // First time flag change mode
//   else if ((okButtonPressedCount % 5) == 1)
//   {

//     // Change time flags accordingly.
//     if (first_time_flag > second_time_flag)
//     {
//       second_time_flag = first_time_flag;
//     }

//     if (first_time_flag > third_time_flag)
//     {
//       third_time_flag = first_time_flag;
//     }
//     delay(10);

//     // Serial.println("                - first Time flag change mode");
//     display.clearDisplay();
//     displayArrowKey(32, 8, 24, 16, 40, 16, true);
//     delay(10);
//     display.setCursor(24, 24);
//     display.print(first_time_flag);
//     delay(10);
//     display.setCursor(56, 24);
//     display.print(second_time_flag);
//     delay(10);
//     display.setCursor(88, 24);
//     display.print(third_time_flag);
//     displayArrowKey(32, 8, 24, 16, 40, 16, false);
//     display.display();
//     delay(10);

//     if (leftButton.isPressed())
//     {
//       Serial.println("left Button Pressed! - first time flag Mode");
//       if (first_time_flag > 0)
//       {
//         first_time_flag -= 0.5;
//         // changeRGBcolor(127,0,0,500);
//       }
//       // Do something in response to the button press
//     }
//     if (rightButton.isPressed())
//     {
//       Serial.println("right Button Pressed! - first time flag Mode On");
//       if (first_time_flag < UPPER_TIME_LIMIT)
//       {
//         // Do something in response to the button press
//         first_time_flag += 0.5;
//         // changeRGBcolor(0,127,0,500);
//       }
//     }
//   }

//   // Second time flag change mode
//   else if ((okButtonPressedCount % 5) == 2)
//   {

//     // Change time flags accordingly
//     if (second_time_flag > third_time_flag)
//     {
//       third_time_flag = second_time_flag;
//     }

//     // Serial.println("                - second Time flag change mode");
//     display.clearDisplay();
//     displayArrowKey(64, 8, 56, 16, 72, 16, true);
//     delay(10);
//     display.setCursor(24, 24);
//     display.print(first_time_flag);
//     delay(10);
//     display.setCursor(56, 24);
//     display.print(second_time_flag);
//     delay(10);
//     display.setCursor(88, 24);
//     display.print(third_time_flag);
//     displayArrowKey(64, 8, 56, 16, 72, 16, false);
//     display.display();
//     delay(10);

//     if (leftButton.isPressed())
//     {
//       Serial.println("left Button Pressed! - first time flag Mode");
//       if (second_time_flag > 0)
//       {
//         second_time_flag -= 0.5;
//         // changeRGBcolor(127,0,0,500);
//       }
//       // Do something in response to the button press
//     }
//     if (rightButton.isPressed())
//     {
//       Serial.println("right Button Pressed! - first time flag Mode On");
//       if (second_time_flag < UPPER_TIME_LIMIT)
//       {
//         // Do something in response to the button press
//         second_time_flag += 0.5;
//         // changeRGBcolor(0,127,0,500);
//       }
//     }
//   }

//   // Third time flag change mode
//   else if ((okButtonPressedCount % 5) == 3)
//   {

//     // Serial.println("                - third Time flag change mode");
//     display.clearDisplay();
//     displayArrowKey(96, 8, 88, 16, 104, 16, true);
//     delay(10);
//     display.setCursor(24, 24);
//     display.print(first_time_flag);
//     delay(10);
//     display.setCursor(56, 24);
//     display.print(second_time_flag);
//     delay(10);
//     display.setCursor(88, 24);
//     display.print(third_time_flag);
//     displayArrowKey(96, 8, 88, 16, 104, 16, false);
//     display.display();
//     delay(10);

//     if (leftButton.isPressed())
//     {
//       Serial.println("left Button Pressed! - first time flag Mode");
//       if (third_time_flag > 0)
//       {
//         third_time_flag -= 0.5;
//         // changeRGBcolor(127,0,0,500);
//       }
//       // Do something in response to the button press
//     }
//     if (rightButton.isPressed())
//     {
//       Serial.println("right Button Pressed! - first time flag Mode On");
//       if (third_time_flag < UPPER_TIME_LIMIT)
//       {
//         // Do something in response to the button press
//         third_time_flag += 0.5;
//         // changeRGBcolor(0,127,0,500);
//       }
//     }
//   }

//   // Speech Mode
//   else if ((okButtonPressedCount % 5) == 4)
//   {

//     // ------------------- Speech Mode : config for slides change & laser on/off --------------------------------------------

//     if (rightButton.isPressed())
//     {
//       rightButton_pressedTime = millis();
//       is_rightButton_Pressing = true;
//       is_rightButton_LongDetected = false;
//       delay(10);
//     }

//     if (rightButton.isReleased())
//     {
//       is_rightButton_Pressing = false;
//       rightButton_releasedTime = millis();

//       delay(10);

//       long rightButton_pressDuration = rightButton_releasedTime - rightButton_pressedTime;

//       if (rightButton_pressDuration < SHORT_PRESS_TIME)
//       {
//         Serial.println("speech mode - A short press is detected");

//         // Move to the next slide
//         if (bleKeyboard.isConnected())
//         {
//           bleKeyboard.write(KEY_RIGHT_ARROW);
//           delay(500);
//           Serial.println("Speech Mode - Moved to the next slide.");
//         }
//       }
//     }

//     // Move to the previous slide
//     if (bleKeyboard.isConnected() && leftButton.isPressed())
//     {
//       bleKeyboard.write(KEY_LEFT_ARROW);
//       delay(500);
//       Serial.println("Speech Mode - Moved to the previous slide.");
//     }

//     // Switch on & off laser when a long press of right button is detected.
//     if (is_rightButton_Pressing == true && is_rightButton_LongDetected == false)

//     {
//       long rightButton_pressDuration = millis() - rightButton_pressedTime;

//       if (rightButton_pressDuration > LONG_PRESS_TIME)
//       {
//         Serial.println("Speech mode - A long press is detected");
//         is_rightButton_LongDetected = true;
//         isLaserOn != isLaserOn;
//         digitalWrite(laserPin, isLaserOn);
//         Serial.print("Speech mode - Laser  : ");
//         Serial.print(isLaserOn);
//       }
//     }

//     // ------------------------------config for right button----------------------------------------------------------------------------

//     // Time set to 0
//     // start counting
//     // if check time - start_time > first_time_flag : Blue light
//     // firstTimeShown - true
//     // if check time - start_time >  2nd time flag >
//     // unsigned long
//     first_time_interval = first_time_flag * 60 * 1000;
//     Serial.print("Elapsed time : ");
//     Serial.print(first_time_interval);
//     Serial.println(" ms");

//     display.clearDisplay();
//     display.setCursor(0, 0);
//     display.print("Speech Mode ");
//     updateOLED();
//     display.display();

//     unsigned long currentMillis = millis();
//   }
//   display.clearDisplay();
// }

// /*

// Presentation Mode :
//     - laser Mode
//     - slide change

// Speech Mode
//     - vibrator on
//     - laser Mode
//     - slide change

// CountUp mode


// [] - count up
// [] - change timer to show in 30s
// [] - change font
// [] - try using both cores of CPU
// [] - add animation between speech mode, time change mode
// [] - vibrator motor on
// [] - laser on
// [] - slide change

// */