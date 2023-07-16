5const int LEDPin = 33;  /* GPIO16 */

int dutyCycle;
/* Setting PWM Properties */
const int PWMFreq = 5000; /* 5 KHz */
const int PWMChannel = 0;
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);
void setup()
{  
  ledcSetup(PWMChannel, PWMFreq, PWMResolution);
  /* Attach the LED PWM Channel to the GPIO Pin */
  ledcAttachPin(LEDPin, PWMChannel);
}
void loop()
{
  /* Increasing the LED brightness with PWM */
  for(dutyCycle = 0; dutyCycle <= MAX_DUTY_CYCLE; dutyCycle++)
  {
    ledcWrite(PWMChannel, dutyCycle);
    delay(3);
    //delayMicroseconds(100);
  }
      /* Decreasing the LED brightness with PWM */
  for(dutyCycle = MAX_DUTY_CYCLE; dutyCycle >= 0; dutyCycle--)
  {
    ledcWrite(PWMChannel, dutyCycle);
    delay(3);
    //delayMicroseconds(100);
  }
}