/// приемник с реле
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(9,10);

RF24Network network(radio);
 
#define relayPin 5                     // пин для реле
#define ID 03                         // ID устройства

int data = 0; 

void setup(void)
{
  Serial.begin(57600);
  pinMode(relayPin, OUTPUT);
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ ID);
}

void loop(void)
{
  // Pump the network regularly
  // нужно постоянно запускать, чтобы транслировались данные между узлами
  network.update();
  // Is there anything ready for us?
  // если пришли данные
  while ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;                               
    network.read(header,&data,sizeof(data));               // читаем данные 
    Serial.println(data);                                  // отправляем в сериал для отладки
    digitalWrite(relayPin, boolean(data));                 // и пишим их в выход с реле
  }
}
