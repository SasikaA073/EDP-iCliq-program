#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <string.h>
#include "loading_animation.h"
#include "hexagon_animation.h"
#include "battery.h"

const uint8_t SCREEN_WIDTH = 128;
const uint8_t SCREEN_HEIGHT = 64;

const uint8_t RledPin = 14;
const uint8_t GledPin = 12;
const uint8_t BledPin = 13;
const uint8_t batteryLevelPin = 17;
const uint8_t vibratorPin = 16;
const uint8_t laserPin = 18;


// Change the following numbers for PCB
const uint8_t rightButtonPin = 25; // Replace 12 with the GPIO pin number to which your button is connected
const uint8_t leftButtonPin = 27;
const uint8_t okButtonPin = 26;

const uint8_t touchUp = 32; 
const uint8_t touchDown = 33;

unsigned long okButton_pressedTime = 0;
unsigned long okButton_releasedTime = 0;
bool is_okButton_Pressing = false;
bool is_okButton_LongDetected = false;

unsigned long rightButton_pressedTime = 0;
unsigned long rightButton_releasedTime = 0;
bool is_rightButton_Pressing = false;
bool is_rightButton_LongDetected = false;

unsigned long leftButton_pressedTime = 0;
unsigned long leftButton_releasedTime = 0;
bool is_leftButton_Pressing = false;
bool is_leftButton_LongDetected = false;


const int SHORT_PRESS_TIME = 1000; // 1000 milliseconds
const int LONG_PRESS_TIME = 1000;  // 1000 milliseconds

#define OLED_RESET_PIN 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// TODO: function to display the battery percentage
void displayBatteryPercentage(uint8_t batteryLevelPin)
{
    // Change the battery_level data type to unit8_t if necessary
    int battery_level = analogRead(batteryLevelPin);
    float battery_percentage = battery_level * 100 / 4095;

    // Print the text starting from the top-left corner (0,0)
    display.clearDisplay();

    // delete the below line ***************************************************************************IJLJIOOUOIOIUOIOIIO
    // battery_percentage = 78.0;
    battery_percentage = 73.0;

    if (battery_percentage <= 25.0)
    {
        display.drawBitmap(0, 0, battery_1, 128, 64, 1);
    }
    else if (battery_percentage > 25.0 && battery_percentage <= 50.0)
    {
        display.drawBitmap(0, 0, battery_2, 128, 64, 1);
    }
    else if (battery_percentage > 50.0 && battery_percentage <= 75.0)
    {
        display.drawBitmap(0, 0, battery_3, 128, 64, 1);
    }
    else if (battery_percentage > 75.0 && battery_percentage <= 100.0)
    {
        display.drawBitmap(0, 0, battery_4, 128, 64, 1);
    }
    display.setCursor(50, 48);
    display.print(battery_percentage);
    display.print(" %");
    delay(10);
    display.display();
    delay(10);

    Serial.print("Battery Level : ");
    Serial.println(battery_level);
    Serial.print("Battery Percentage : ");
    Serial.print(battery_percentage);
    Serial.println(" %");

    delay(2000);
    // display.clearDisplay();
    Serial.println("# Battery Percentage Dispalyed.");
    // display.display();
    delay(10);
}


// function to display the triangle
void displayArrowKey(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, bool up)
{
    if (up == true)
    {
        display.fillTriangle(x1, y1, x2, y2, x3, y3, SSD1306_WHITE);
    }
    else
    {
        display.fillTriangle(x1, SCREEN_HEIGHT - y1, x2, SCREEN_HEIGHT - y2, x3, SCREEN_HEIGHT - y3, SSD1306_WHITE);
    }

    delay(10);
}

void changeRGBcolor(uint8_t r_value, uint8_t g_value, uint8_t b_value, int time_delay)
{
    /* Since this is common cathode RGB, (255,255,255) represents off. */
    analogWrite(RledPin, r_value);
    analogWrite(GledPin, g_value);
    analogWrite(BledPin, b_value);
    delay(time_delay);
}




// functions for animations 

// A function to keep one loop of android_loading the GIF going
void animate_android_loading()
{
    const unsigned char *android_loading_frames[60] = {
        // NOTE : Here ios_loading_ must be replaced by the identifer you gave above in the image to bitmap conversion step.
        android_loading_1, android_loading_2, android_loading_3, android_loading_4, android_loading_5, android_loading_6, android_loading_7, android_loading_8, android_loading_9, android_loading_10, android_loading_11, android_loading_12, android_loading_13, android_loading_14, android_loading_15, android_loading_16, android_loading_17, android_loading_18, android_loading_19, android_loading_20, android_loading_21, android_loading_22, android_loading_23, android_loading_24, android_loading_25, android_loading_26, android_loading_27, android_loading_28, android_loading_29, android_loading_30, android_loading_31, android_loading_32, android_loading_33, android_loading_34, android_loading_35, android_loading_36, android_loading_37, android_loading_38, android_loading_39, android_loading_40, android_loading_41, android_loading_42, android_loading_43, android_loading_44, android_loading_45, android_loading_46, android_loading_47, android_loading_48, android_loading_49, android_loading_50, android_loading_51, android_loading_52, android_loading_53, android_loading_54, android_loading_55, android_loading_56, android_loading_57, android_loading_58, android_loading_59, android_loading_60};
    for (int i = 0; i < 60; i++)
    {
        display.clearDisplay();
        display.drawBitmap(0, 0, android_loading_frames[i], 128, 64, 1);
        display.display();
        delay(10);
    }
}

// A function to keep one loop of hexagon_loading the GIF going
void animate_hexagon_loading()
{
    const unsigned char *hexagon_loading_frames[100] = {
        // NOTE : Here ios_loading_ must be replaced by the identifer you gave above in the image to bitmap conversion step.
        hexagon_loading_1, hexagon_loading_2, hexagon_loading_3, hexagon_loading_4, hexagon_loading_5, hexagon_loading_6, hexagon_loading_7, hexagon_loading_8, hexagon_loading_9, hexagon_loading_10, hexagon_loading_11, hexagon_loading_12, hexagon_loading_13, hexagon_loading_14, hexagon_loading_15, hexagon_loading_16, hexagon_loading_17, hexagon_loading_18, hexagon_loading_19, hexagon_loading_20, hexagon_loading_21, hexagon_loading_22, hexagon_loading_23, hexagon_loading_24, hexagon_loading_25, hexagon_loading_26, hexagon_loading_27, hexagon_loading_28, hexagon_loading_29, hexagon_loading_30, hexagon_loading_31, hexagon_loading_32, hexagon_loading_33, hexagon_loading_34, hexagon_loading_35, hexagon_loading_36, hexagon_loading_37, hexagon_loading_38, hexagon_loading_39, hexagon_loading_40, hexagon_loading_41, hexagon_loading_42, hexagon_loading_43, hexagon_loading_44, hexagon_loading_45, hexagon_loading_46, hexagon_loading_47, hexagon_loading_48, hexagon_loading_49, hexagon_loading_50, hexagon_loading_51, hexagon_loading_52, hexagon_loading_53, hexagon_loading_54, hexagon_loading_55, hexagon_loading_56, hexagon_loading_57, hexagon_loading_58, hexagon_loading_59, hexagon_loading_60, hexagon_loading_61, hexagon_loading_62, hexagon_loading_63, hexagon_loading_64, hexagon_loading_65, hexagon_loading_66, hexagon_loading_67, hexagon_loading_68, hexagon_loading_69, hexagon_loading_70, hexagon_loading_71, hexagon_loading_72, hexagon_loading_73, hexagon_loading_74, hexagon_loading_75, hexagon_loading_76, hexagon_loading_77, hexagon_loading_78, hexagon_loading_79, hexagon_loading_80, hexagon_loading_81, hexagon_loading_82, hexagon_loading_83, hexagon_loading_84, hexagon_loading_85, hexagon_loading_86, hexagon_loading_87, hexagon_loading_88, hexagon_loading_89, hexagon_loading_90, hexagon_loading_91, hexagon_loading_92, hexagon_loading_93, hexagon_loading_94, hexagon_loading_95, hexagon_loading_96, hexagon_loading_97, hexagon_loading_98, hexagon_loading_99, hexagon_loading_100};
    for (int i = 0; i < 100; i++)
    {
        display.clearDisplay();
        display.drawBitmap(0, 0, hexagon_loading_frames[i], 128, 64, 1);
        display.display();
        delay(10);
    }
}


