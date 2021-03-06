#include "acs712.h"
#include <SimpleKalmanFilter.h>

SimpleKalmanFilter* fillterACS = (SimpleKalmanFilter*)malloc(sizeof(SimpleKalmanFilter) * 11);

ACS712_Sensor::ACS712_Sensor(float vcc, float sensitivity, float quiescent_Output_voltage, float magnetic_offset_error)
{
  this->_vcc = vcc;
  this->_sensitivity = sensitivity;
  this->_quiescent_Output_voltage = quiescent_Output_voltage;
  this->_magnetic_offset_error = magnetic_offset_error;

  for (int index = 0; index < 10; index++) 
  {
    fillterACS[index] = SimpleKalmanFilter(1, 1, 0.01);
  }
}

float ACS712_Sensor::getCurrentAverage(int _vpin, int i)
{
  float sum = 0, average = 0;

  for(int index = 0; index < i; index++)
  {
    sum += getCurrent(_vpin);
  }
  
  average = sum /i;
  return average;
}

float ACS712_Sensor::getCurrent(int _vpin)
{
  readCurrent(_vpin);
  return this->_current;
} 

//float ACS712_Sensor::readCurrent(int _vpin)
//{
//  const float FACTOR = this->_sensitivity/1000;// set sensitivity for selected model - unit V
//  const float QOV    = this->_quiescent_Output_voltage * this->_vcc;// set quiescent Output voltage for selected model
//  
//  float voltage;// internal variable for voltage
//  float voltageOut  = fillterACS[_vpin-56].updateEstimate(analogRead(_vpin));
//  float voltage_raw =   (5.0 / 1023.0)* voltageOut;// Read the voltage from sensor
// 
//  // quá trình xả từ accuy diễn ra Ip+ -> Ip- : 0-(+max)
//  // kiểm tra đang sạc không
//  if(voltage_raw > QOV + OFFSET_SEN)
//  {
//    this->_charging =  DISCHARGE;
//  }
//  else if(voltage_raw < QOV - OFFSET_SEN)
//  {
//    this->_charging =  CHARGE;
//  }
//  else
//  {
//    this->_charging =  NONE;
//  }
//  
//  voltage =  voltage_raw - QOV;
//  this->_outVoltage = voltage;
//  
//  this->_current = abs(voltage / FACTOR) - (this->_magnetic_offset_error/1000);
//}


float ACS712_Sensor::readCurrent(int _vpin)
{
  float sample = 509.84;
  //float sample = 512.0;
  float adc_raw = fillterACS[_vpin-56].updateEstimate(analogRead(_vpin));
  //ECHOLN(adc_raw);
  float offset  = adc_raw - sample;
  float current = abs(offset / 17.0);
  ECHOLN("Vpin"+String(_vpin)+ "  "+ String(adc_raw));
  // quá trình xả từ accuy diễn ra Ip+ -> Ip- : 0-(+max)
  if(adc_raw > 512)
  {
    this->_charging =  DISCHARGE;
  }
  else if(adc_raw < 508)
  {
    this->_charging =  CHARGE;
  }
  else
  {
    this->_charging =  NONE;
  }

  this->_current = current;
}

IsChargeStatus ACS712_Sensor::isCharging()
{
  this->readCurrent(ACS1); 
  return this->_charging;
}

float ACS712_Sensor::getAmpleOfCircuit(void)
{
  float Isum = 0.0;
  for(int i = 0 ; i < 10; i++)
  {
    Isum += this->getCurrent(i+56);
  }
  return Isum;
}

void ACS712_Sensor::debug(int _vpin)
{
  ECHOLN("=========================================");
  ECHO("Sensitivity: ");
  ECHO(this->_sensitivity);
  ECHOLN(" mV");
  ECHO("Quiesent voltage: ");
  ECHO(this->_quiescent_Output_voltage);
  ECHOLN(" V");

  ECHOLN("Readings....");
  ECHO("\tInput Pin: ");
  ECHOLN(_vpin);
  ECHO("\tAnalog: ");
  ECHOLN(analogRead(_vpin));
  ECHO("\tVoltage: ");
  Serial.print(analogRead(_vpin) * 5.0 / 1023.0, 3);
  ECHOLN(" V");
  
  ECHO("\tCurrent: ");
  ECHO(this->getCurrent(_vpin));
  ECHOLN(" A");

  ECHO("\tAverage of 200 readings: ");
  ECHO(getCurrentAverage(_vpin, 200));
  ECHOLN(" A");

  ECHO("\tIs charging : ");
  ECHOLN(this->_charging);
  ECHOLN("=========================================");
} 
