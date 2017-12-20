#include <DallasTemperature.h>
#include <OneWire.h>
#include <Adafruit_SleepyDog.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#define ONE_WIRE_BUS 5;                             // Data wire is plugged into digital pin 3 on the Arduino
#define NumberOfDevices 8;                          // Set maximum number of devices in order to dimension
 
OneWire oneWire(ONE_WIRE_BUS);                     // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);               // Pass our oneWire reference to Dallas Temperature.
byte allAddress [NumberOfDevices][8];              // Device Addresses are 8-element byte arrays.
char sn[NumberOfDevices][35];
byte totalDevices;                                 // Declare variable to store number of One Wire devices
// that are actually discovered.
byte discoverOneWireDevices() {
  byte j=0;                                        // search for one wire devices and
  // copy to device address arrays.
  while ((j  NumberOfDevices) ; (oneWire.search(allAddress[j]))) {
    j++;
    }
  return j;                 // return total number of devices found.
}
 
typedef struct{
  char Type[3];
  float Value;         // значение 4bytes
  boolean Status;      // статус - 0-ошибка (false), 1-ок (true) 1byte
  char Note[24];       // комментарий
}ModuleInfo;
 
ModuleInfo mi[NumberOfDevices+2];
 
#define home_net_channel 95
RF24 radio(7,8);
RF24Network network(radio);
 
// Our node address
uint16_t this_node;
 
// Sleep constants.  In this example, the watchdog timer wakes up
// every 1s, and every 4th wakeup we power up the radio and send
// a reading.  In real use, these numbers which be much higher.
// Try wdt_8s and 7 cycles for one reading per minute.; 1
const wdt_prescalar_e wdt_prescalar = wdt_8s;
const short sleep_cycles_per_transmission = 32;
 
void setup(void)
{
  Serial.begin(57600);
  sensors.begin();
  totalDevices = discoverOneWireDevices();
 
  for (byte i=0; i  totalDevices; i++)
  {
    sn[i][0]=0;
    for(int k=7;k;=0;k--)
    {
      char msg[3];
      sprintf(msg,"%02X",allAddress[i][k]);
      strcat(sn[i],msg);
      //DSstring[i]+=msg;
    };
     
    sensors.setResolution(allAddress[i], 10);      // and set the a to d conversion resolution of each.
  }
  this_node=1;
 
  if ( this_node ; 0 )
  Sleep.begin(wdt_prescalar,sleep_cycles_per_transmission);
   
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 95, /*node address*/ this_node);
}
 
void loop(void)
{
  network.update();
  if ( this_node ; 0 )
  {
    sensors.requestTemperatures();
    for (byte i=0; i  totalDevices; i++)
    {
      float tempC = sensors.getTempC(allAddress[i]);
      strcpy(mi[i].Type,"T");
      char msg[2];
      sprintf(msg,"%d",i+1);
      strcat(mi[i].Type,msg);
      strcpy(mi[i].Note,sn[i]);
      if (tempC != -127.00)
      {
      mi[i].Value=tempC;
      mi[i].Status=true;
      }
      else
      mi[i].Status=false;
    }
    for (byte i=0; i  totalDevices; i++)
    {
    if (mi[i].Status)
    {
      RF24NetworkHeader header(/*to node*/ 0, /*type*/ 'Sensor');
      bool ok = network.write(header,mi[i],sizeof(mi[i]));
      Serial.print("Sending: Type=");
      Serial.print(mi[i].Type);
      Serial.print(", Value=");
      Serial.print(mi[i].Value);
      Serial.print(", Status=");
      Serial.print(mi[i].Status);
      Serial.print(", Note=");
      Serial.print(mi[i].Note);
      Serial.print("\n");
    }
    else Serial.println("Error from sensor");
    }
    radio.powerDown();
    Sleep.go();
    nodeconfig_listen();
  }
}
