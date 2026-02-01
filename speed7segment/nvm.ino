//Non Volatile Memory for storing configuration parameters (settings)

#include <EEPROM.h>

#define NVM_ADDRESS 0x10

typedef struct nvm_struct
{
  int nvm_rearm_time;
  int nvm_sensor_mode;
  int nvm_sensor_threshold;
} nvm_struct_t;

enum
{
  NVM_REARM_TIME, //time to enable trigger
  NVM_SENSOR_MODE, //digital or analog
  NVM_SENSOR_THRESHOLD, //for analog mode only
};

int nvm_defaults[] =
{
  2,
  0,
  500,
};

nvm_struct_t nvm_data {0};



void nvm_init(void)
{
  Serial.print("Internal EEPROM size: ");
  Serial.println(EEPROM.length());
  nvm_read();
}



void nvm_read(void)
{
  EEPROM.get(NVM_ADDRESS, nvm_data);
}



void nvm_write(void)
{
  EEPROM.put(NVM_ADDRESS, nvm_data);
}



int nvm_get(int index)
{
  int data = 0;

  switch (index)
  {
    case NVM_REARM_TIME:
      data = nvm_data.nvm_rearm_time;
      break;
    
    case NVM_SENSOR_MODE:
      data = nvm_data.nvm_sensor_mode;
      break;
    
    case NVM_SENSOR_THRESHOLD:
      data = nvm_data.nvm_sensor_threshold;
      break;
  }

  return nvm_param_range(index, data);
}



void nvm_set(int index, int value)
{
  value = nvm_param_range(index, value);

  switch (index)
  {
    case NVM_REARM_TIME:
      nvm_data.nvm_rearm_time = value;
      break;
    
    case NVM_SENSOR_MODE:
      nvm_data.nvm_sensor_mode = value;
      break;
    
    case NVM_SENSOR_THRESHOLD:
      nvm_data.nvm_sensor_threshold = value;
      break;
  }
}



int nvm_param_range(int index, int value)
{
  int range_low = 0;
  int range_high = 0;

  switch (index)
  {
    case NVM_REARM_TIME:
      range_low   = 0;
      range_high  = 5;
      break;
    
    case NVM_SENSOR_MODE:
      range_low   = 0;
      range_high  = 1;
      break;
    
    case NVM_SENSOR_THRESHOLD:
      range_low   = 0;
      range_high  = 1023;
      break;
  }

  return ((value >= range_low) && (value <= range_high)) ? value : nvm_defaults[index]; 
}

