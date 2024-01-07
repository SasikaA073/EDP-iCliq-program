struct GreenColorParameters {
    uint8_t green_led;
    uint8_t duration;
};

void handleGreenLed(void *parameter) {
    GreenColorParameters *params = (GreenColorParameters *)parameter;

    // Extract parameters
    uint8_t green_led_pin = params->green_led;
    uint8_t led_time_duration = params->duration;

    // Green color (255,0,255)
    analogWrite(green_led_pin, 0);
    vTaskDelay(led_time_duration * 1000 / portTICK_PERIOD_MS); // Non-blocking delay using FreeRTOS function
    analogWrite(green_led_pin, 255);

    vTaskDelete(NULL); // Delete the task after execution
}


// -----------------------------------------------------------------
// Yellow color
struct YellowColorParameters {
    uint8_t red_led;
    uint8_t green_led;
    uint8_t duration;
};

void handleYellowLed(void *parameter) {
    YellowColorParameters *params = (YellowColorParameters *)parameter;

    // Extract parameters
    uint8_t green_led_pin = params->green_led;
    uint8_t red_led_pin = params->red_led;
    uint8_t led_time_duration = params->duration;

    // Yellow color (0,128,255)
    analogWrite(red_led_pin, 0);
    analogWrite(green_led_pin, 128);
    vTaskDelay(led_time_duration * 1000 / portTICK_PERIOD_MS); // Non-blocking delay using FreeRTOS function
    analogWrite(red_led_pin, 255);
    analogWrite(green_led_pin, 255);

    vTaskDelete(NULL); // Delete the task after execution
}

// -----------------------------------------------------------------
// Red Color


struct RedColorParameters {
    uint8_t red_led;
    uint8_t duration;
};

void handleRedLed(void *parameter) {
    RedColorParameters *params = (RedColorParameters *)parameter;

    // Extract parameters
    uint8_t red_led_pin = params->red_led;
    uint8_t led_time_duration = params->duration;

    // Red color (0,255,255)
    analogWrite(red_led_pin, 0);
    vTaskDelay(led_time_duration * 1000 / portTICK_PERIOD_MS); // Non-blocking delay using FreeRTOS function
    analogWrite(red_led_pin, 255);

    vTaskDelete(NULL); // Delete the task after execution
}
