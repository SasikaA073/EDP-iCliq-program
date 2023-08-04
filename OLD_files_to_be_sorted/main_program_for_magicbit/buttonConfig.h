
    // // ---------------------------------------------------- Configuration for rightButton ----------------------------------------------------------------------
    // // short press, long press
    // if (rightButton.isPressed())
    // {
    //   rightButton_pressedTime = millis();
    //   is_rightButton_Pressing = true;
    //   is_rightButton_LongDetected = false;
    // }

    // if (rightButton.isReleased())
    // {
    //   is_rightButton_Pressing = false;
    //   rightButton_releasedTime = millis();

    //   long rightButton_pressDuration = rightButton_releasedTime - rightButton_pressedTime;

    //   if (rightButton_pressDuration < SHORT_PRESS_TIME)
    //     Serial.println("        + short press - rightButton detected");
    // }

    // if (is_rightButton_Pressing == true && is_rightButton_LongDetected == false)
    // {
    //   long rightButton_pressDuration = millis() - rightButton_pressedTime;

    //   if (rightButton_pressDuration > LONG_PRESS_TIME)
    //   {
    //     Serial.println("        + long press - rightButton detected");
    //     is_rightButton_LongDetected = true;

    //   }
    // }

    // // ------------------------------------------------------------------------------------------------------------------------------------------------------------
