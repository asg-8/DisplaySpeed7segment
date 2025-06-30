int sensorPin = A0;   // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int counter = 0;

signed long milis_sensorA = 0;
signed long milis_sensorB = 0;

bool updated_value = true;
bool sensorA = false;
bool sensorB = false;
bool sensorA_prev = true;
bool sensorB_prev = true;
bool sensorA_new = false;
bool sensorB_new = false;
bool gateA = true;
bool gateB = true;

int speed = 0;

// float SENSOR_DISTANCE = 2.54 * 58; //mm/step * number of steps //in mm
long DISTANCE_nm = 147320000LLU; //in nm --> mm / us
long DISTANCE_um = 147320LU; //in um --> mm / ms
#define SENSOR_DISTANCE (DISTANCE_um)

#define MILIS_GATE 3000

int loop_speed(void)
{
  unsigned long milis = millis();
  // unsigned long milis = micros();
  sensorA = !digitalRead(A3);
  sensorB = !digitalRead(A2);

  if (milis > milis_sensorA + MILIS_GATE) gateA = true;
  if (milis > milis_sensorB + MILIS_GATE) gateB = true;

  digitalWrite(6, gateA);
  digitalWrite(7, gateB);

  if (gateA && gateB)
  {
    sensorA_new = false;
    sensorB_new = false;
  }

  if ((sensorA > sensorA_prev) && gateA)
  {
    milis_sensorA = milis;
    sensorA_new = true;
    gateA = false;
  }

  if ((sensorB > sensorB_prev) && gateB)
  {
    milis_sensorB = milis;
    sensorB_new = true;
    gateB = false;
  }

  sensorA_prev = sensorA;
  sensorB_prev = sensorB;

  if (sensorA_new && sensorB_new)
  {
    sensorA_new = false;
    sensorB_new = false;
    updated_value = true;
  }

  if (updated_value)
  {
    updated_value = false;

    unsigned long time_diff = abs(milis_sensorB - milis_sensorA);
    speed = (unsigned long)SENSOR_DISTANCE / time_diff; //mm/s
  }

  return speed;
}
