  /// передатчик, рандомом включает реле
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);
 
 

void setup(void)
{
  Serial.begin(57600);
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ 00);             // канал и адрес узла
}

void loop(void)
{
  // Pump the network regularly
    network.update();                                              // нужно постоянно запускать, чтобы транслировались 
                                                                   // данные между узлами
    int data = random(2);                                          // это что передаем на устройства
    int ID = random(1, 4);                                         // это адрес устройства

    Serial.print("Sending...");
     
    RF24NetworkHeader header(/*to node*/ ID);                      // тут указываем на какое устройство отправляем данные
    bool ok = network.write(header,&data,sizeof(data));            // передаем, функция возвращает статус
    if (ok)                                                        // смотрим на статус и отписываемся в Serial
      Serial.println("ok.");
    else
      Serial.println("failed.");
      
   delay(100);
}
