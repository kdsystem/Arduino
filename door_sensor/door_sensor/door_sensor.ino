/**
 * Подключение:
 * SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 * CE -> 9
 * CSN -> 10
 */

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

int switchPin = 2; // к выводу 2 подключён геркон
int ledPin = 3; // к пину 13 подключён встроенный светодиод

void setup() {
  pinMode(switchPin, INPUT); // задаём вывод 2 в качестве входа (будем считывать с него)
  digitalWrite(switchPin, HIGH); // активируем внутренний подтягивающий резистор вывода
  pinMode(ledPin, OUTPUT); // задаём вывод 13 в качестве выхода
  Serial.begin(9600); // задействуем последовательный порт

  Mirf.csnPin = 10;
  Mirf.cePin = 9;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"clie1");
  Mirf.payload = sizeof(unsigned long);
  Mirf.config();
  Serial.println("Beginning ... ");
}

void loop() {
  // Получаем количество миллисекунд, прошедших с момента включения платы:
  unsigned long time = millis();
  // Устанавливаем адрес модуля. которому будем передавать пакет:
  Mirf.setTADDR((byte *)"serv1");
  // Передаём значение переменной time в виде массива байт:
  Mirf.send((byte *) &time);

  int g = digitalRead(switchPin); // считываем показания с геркона
  digitalWrite(ledPin, !g); // инвертированные показания записываем в порт со светодиодом
  Serial.println(g); // посылаем в последовательный порт значения с геркона
  //когда геркон замкнут, значение "0" (LOW), когда разомкнут - "1" (HIGH)
  delay(1000); // повторяем цикл через 1000 мсек
 
  // Ждём окончания передачи
  while(Mirf.isSending()){
  }
  Serial.println("Finished sending");
  delay(10);
  // Если ничего не получено в ответ
  while(!Mirf.dataReady()){
    Serial.println("Waiting");
    // В течении секунды
    if ( ( millis() - time ) > 1000 ) {
      // Считаем. что пакет потерян
      Serial.println("Timeout on response from server!");
      return;
    }
  }
  // пробуем получить ответный пакет:
  Mirf.getData((byte *) &time);
  // Вычисляем задержку передачи
  // и выводим полученное значение в серийный порт:
  Serial.print("Ping: ");
  Serial.println((millis() - time));
}
