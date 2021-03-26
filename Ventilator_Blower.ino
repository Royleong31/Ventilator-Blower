#include <PWM.h>

int pwmPin = 10;
int feedbackPin = 6;
int32_t frequency = 25000; // desired frequency in Hertz
int dutyCycle8Bit = 127;
float dutyCycle;

void setup()
{
  Serial.begin(9600);
  InitTimersSafe();
  bool success = SetPinFrequencySafe(9, frequency);
  if (success) {
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
  }
}

void setDutyCycle(float dutyCycle) { 
  dutyCycle8Bit = round(dutyCycle / 100 * 255);
  float dutyCycleEstimate = (float) dutyCycle8Bit / 255 * 100;
  Serial.println("Duty Cycle: " + String(dutyCycleEstimate) + "%");
}

void frequencyCounter()
{
  float ontime, offtime, freq, rpm, period;
  ontime = pulseIn(feedbackPin, HIGH);
  offtime = pulseIn(feedbackPin, LOW);
  period = ontime + offtime;
  freq = 1000000 / period;
  rpm = freq / 2 * 60;
  if (period == 0) {
    rpm = 0;
  }
  Serial.println("Speed: " + String(rpm));
}

void loop() {
  if (Serial.available () > 0) {
    dutyCycle = Serial.parseFloat();
    setDutyCycle(dutyCycle);
  }

  frequencyCounter();
  pwmWrite(pwmPin, dutyCycle8Bit);


  Serial.println(0);
}
