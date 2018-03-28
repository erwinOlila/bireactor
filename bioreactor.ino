#include <max6675.h>

const float HEAT_RATE    = 20.0;
const float TERMINAL     = 600.0;
const float INITIAL      = 40.0;
const int8_t   PERIOD       = 6000; // duration per interval
const int8_t   SSR_PIN      = 9;
const int8_t   INTERVALS    = 10;
const int8_t   DUMMY_SENSOR = A0;

const int8_t DO   = 50;
const int8_t CS_1 = 48;
const int8_t CS_2 = 46;
const int8_t CS_3 = 44;
const int8_t CS_4 = 42;
const int8_t CS_5 = 40;
const int8_t CLK  = 52;

//MAX6675 thermocouple_1(CLK, CS_1, DO);
//MAX6675 thermocouple_2(CLK, CS_2, DO);
//MAX6675 thermocouple_3(CLK, CS_3, DO);
//MAX6675 thermocouple_4(CLK, CS_4, DO);
//MAX6675 thermocouple_5(CLK, CS_5, DO);

float       temp_per_interval = 0.0;            // target incremental value per 6 seconds
double       dummy_reading    = 0.0;            // just a placeholder
const float CAL_FACTOR        = 620.0 / 1023.0; // 620C is set as the max temperature
const float DIFF              = 0.1000;         // window for error is set as -+ 0.1000

void setup() {
  Serial.begin(9600);
 
  change_temp(false); // set the initial temperature to 40C
  temp_per_interval = float (HEAT_RATE / INTERVALS); // get the target incremental value per 6 seconds
}

void loop() {
  int8_t lapse = millis();

  // read temperatures from 4 sensors (except the middle furnace)
  double temp_2 = readTemp(CLK, CS_2 ,DO);
  double temp_3 = readTemp(CLK, CS_3 ,DO);
  double temp_4 = readTemp(CLK, CS_4 ,DO);
  double temp_5 = readTemp(CLK, CS_5 ,DO);

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
//    dummy_reading = analogRead(DUMMY_SENSOR) * CAL_FACTOR;
    dummy_reading = readTemp(CLK, CS_1 ,DO);
    if (dummy_reading < limit) {
      digitalWrite(SSR_PIN, HIGH); 
    } else {
      digitalWrite(SSR_PIN, LOW);
    }
  }while (abs(dummy_reading - limit) > DIFF);
}

double readTemp(int8_t clk, int8_t cs, int8_t miso) {
  MAX6675 thermo(clk, cs, miso);
  double temperature = thermo.readCelsius();
  delay(250);
  return temperature;
}

