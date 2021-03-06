#include "sim7600e.h"


SIM7600E SIM;

unsigned long cur_time, old_time;
static const char *url = "http://admin.breedlife.com/api/v1/data";
static const char *data = "{\"data\":[\"TR001\",[[8,8,8],[9,9,9],[9,9,9],[234,123,13],[11,123,112]],1,41.2,29,48.1,24,\"11/11/2011 11:11:11\"]}";

void setup()
{
  delay(5000);
  //  SIM.rstSIM();
  //  delay(100);
  Serial.begin(9600);
  delay(100);
  if(SIM.setupSIM(9600))
  { 
    Serial.println(F("\t----------------------------------"));
    Serial.println(F("\tSettup done"));
    Serial.println(F("\t----------------------------------"));
  }
  else
  {
    Serial.println(F("\t----------------------------------"));
    Serial.println("\tSettup fail");
    Serial.println(F("\t----------------------------------"));
  }  
  delay(100);
}
void loop()
{
  cur_time = millis();
  if ( cur_time - old_time >= 5000){
    SIM.HTTP_POST_start(url, data);
    old_time = cur_time;
  }
}
