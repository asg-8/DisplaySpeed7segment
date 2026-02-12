// float SENSOR_DISTANCE = 2.54 * 58; //mm/step * number of steps //in mm
// long DISTANCE_nm = 147320000LLU; //in nm --> mm / us
// long DISTANCE_um = 147320LU; //in um --> mm / ms
long DISTANCE_um = 127000LU; //in um --> mm / ms
#define SENSOR_DISTANCE (DISTANCE_um)

#define MILIS_GATE 3000

typedef enum sensor_enum {SENSOR_A, SENSOR_B} sensor_enum_t;
typedef enum sensor_type {SENSOR_DIGITAL, SENSOR_ANALOG} sensor_type_t;

// int sensorPin = A0;   // select the input pin for the potentiometer
// int ledPin = 13;      // select the pin for the LED

int sensorValue = 0;  // variable to store the value coming from the sensor
int counter = 0;

signed long milis_sensorA = 0;
signed long milis_sensorB = 0;

bool sensorA = false;
bool sensorB = false;
bool sensorA_prev = true;
bool sensorB_prev = true;
bool sensorA_new = false;
bool sensorB_new = false;
bool gateA = true;
bool gateB = true;
bool updated_value = true;
bool new_speed_value = false;

int speed = 0;
long time = 0; //diff

sensor_type_t sensor_mode = SENSOR_DIGITAL;

int threshold = 511;



bool sensor_detected(sensor_enum_t sensor);
bool sensor_detected_digital(sensor_enum_t sensor);
bool sensor_detected_analog(sensor_enum_t sensor);

/////////////////////////////////////// HYSTERESYS

bool sensors_has_speed_updated(void)
{
  return new_speed_value;
}

int sensors_get_speed(void)
{
  new_speed_value = false;
  return speed;
}

long sensors_get_time(void)
{
  return time;
}


void sensors_set_gate(int)
{
  
}

void sensors_scheduled(void)
{
  unsigned long milis = millis();
  // unsigned long milis = micros();
  // sensorA = !digitalRead(PIN_DI_SENSOR_A);
  // sensorB = !digitalRead(PIN_DI_SENSOR_B);

  sensorA = sensor_detected(SENSOR_A);
  sensorB = sensor_detected(SENSOR_B);

  if (milis > milis_sensorA + MILIS_GATE) gateA = true;
  if (milis > milis_sensorB + MILIS_GATE) gateB = true;

  digitalWrite(PIN_DO_TRIGRDY_A, !gateA);
  digitalWrite(PIN_DO_TRIGRDY_B, !gateB);

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
    new_speed_value = true;

    unsigned long time_diff = abs(milis_sensorB - milis_sensorA); //ms
    
    #ifdef SERIAL_INFO
    Serial.println(time_diff);
    #endif
    
    speed = (unsigned long)SENSOR_DISTANCE / time_diff; //mm/s
  }
}

int sensor_analog_value(sensor_enum_t sensor)
{
  return analogRead(sensor == SENSOR_A ? PIN_AI_SENSOR_A : PIN_AI_SENSOR_B);
}

bool sensor_detected_analog(sensor_enum_t sensor)
{
  return sensor_analog_value(sensor) > threshold;
}

bool sensor_detected(sensor_enum_t sensor)
{
  return sensor_mode == SENSOR_ANALOG ? sensor_detected_analog(sensor) : sensor_detected_digital(sensor);
}

bool sensor_detected_digital(sensor_enum_t sensor)
{
  return !digitalRead(sensor == SENSOR_A ? PIN_DI_SENSOR_A : PIN_DI_SENSOR_B);
}
