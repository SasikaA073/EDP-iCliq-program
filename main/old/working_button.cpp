#include "ezButton.h"

const int buttonPin = 34; // Replace 12 with the GPIO pin number to which your button is connected
ezButton myButton(buttonPin, INPUT_PULLUP); // Create an instance of the ezButton class

void setup()
{
    Serial.begin(115200);
    myButton.setDebounceTime(50); // Adjust the debounce time as needed (in milliseconds)
    myButton.setCountMode(COUNT_FALLING); // Change this to COUNT_RISING or COUNT_BOTH if needed
}

void loop()
{
    myButton.loop(); // Call the loop method to update the button state

    if (myButton.isPressed())
    {
        Serial.println("Button Pressed!");
        // Do something in response to the button press
    }

    // Add other code here as needed
}
