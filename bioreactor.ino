
const float HEAT_RATE    = 20.0;
const float TERMINAL     = 600.0;
const float INITIAL      = 40.0;
const int   PERIOD       = 6000; // duration per interval
const int   SSR_PIN      = 9;
const int   INTERVALS    = 10;
const int   DUMMY_SENSOR = A0;

float       temp_per_interval = 0.0;            // target incremental value per 6 seconds
float       dummy_reading     = 0.0;            // just a placeholder
const float CAL_FACTOR        = 620.0 / 1023.0; // 620C is set as the max temperature
const float DIFF              = 0.1000;         // window for error is set as -+ 0.1000

void setup() {
  Serial.begin(9600);
 
  change_temp(false); // set the initial temperature to 40C
  temp_per_interval = float (HEAT_RATE / INTERVALS); // get the target incremental value per 6 seconds
}

void loop() {
  int lapse = millis();

  // Do the following functions until 6 seconds is reached
  while (millis() - lapse < PERIOD) {
    change_temp(true);
  }
}

// Change the temperature up to the set limit
// Argument: target, Type: boolean
/// set false if there is no target temperature per interval (e.g. initializing temperature)
/// set true if there is a target per intervall (e.g. raising the temperature up to the TERMINAL) 
void change_temp (boolean target) {
  float limit = 0.0;
  if (target) {
    limit = dummy_reading + temp_per_interval;
  } else {
    limit = INITIAL;
  }
  do {
    dummy_reading = analogRead(DUMMY_SENSOR) * CAL_FACTOR;
    if (dummy_reading < limit) {
      digitalWrite(SSR_PIN, HIGH); 
    } else {
      digitalWrite(SSR_PIN, LOW);
    }
  }while (abs(dummy_reading - limit) > DIFF);
}

