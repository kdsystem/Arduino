/**
   Подключение:
   SPI:
   MISO -> 12
   MOSI -> 11
   SCK -> 13
   CE -> 9
   CSN -> 10
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
  //Инициализация передатчика

  //Установим прерывание
  attachInterrupt(switchPin, Send2HUB, CHANGE );
}

void Send2HUB() {
  // Получаем количество миллисекунд, прошедших с момента включения платы:
  unsigned long time = millis();
  int doorstate = digitalRead(switchPin); // считываем показания с геркона
  digitalWrite(ledPin, !doorstate); // инвертированные показания записываем в порт со светодиодом
  //Отправить состояние двери (откр или закр)

}

void loop() {
  int doorstate = digitalRead(switchPin); // считываем показания с геркона
  digitalWrite(ledPin, !doorstate); // инвертированные показания записываем в порт со светодиодом
  Serial.println(doorstate); // посылаем в последовательный порт значения с геркона
  //когда геркон замкнут, значение "0" (LOW), когда разомкнут - "1" (HIGH)
}
