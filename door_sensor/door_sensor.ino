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
#include <RF24.h>
#include <stdio.h>

// CE,CSN пины
RF24 radio(9, 10);
unsigned long count = 0;
int switchPin = 2; // к выводу 2 подключён геркон
int ledPin = 3; // к пину 3 подключён cветодиод (13 - встроенный)

void setup() {
  pinMode(switchPin, INPUT); // задаём вывод 2 в качестве входа (будем считывать с него)
  digitalWrite(switchPin, HIGH); // активируем внутренний подтягивающий резистор вывода
  pinMode(ledPin, OUTPUT); // задаём вывод 3 в качестве выхода
  Serial.begin(9600); // задействуем последовательный порт
  // Инициализируем канал связи и не только.
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x4c);
  radio.openWritingPipe(0xf0f0f0f0d2LL);
  radio.enableDynamicPayloads();
  radio.powerUp();
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  //Установим прерывание
  attachInterrupt(0, Send2HUB, CHANGE );
}

void Send2HUB() {
  // Получаем количество миллисекунд, прошедших с момента включения платы:
  unsigned long time = millis();
  char outBuffer[32] = "";
  delay(200);
  int doorstate = digitalRead(switchPin); // считываем показания с геркона
  digitalWrite(ledPin, !doorstate); // инвертированные показания записываем в порт со светодиодом
  Serial.println(doorstate);
  //Отправить состояние двери (откр или закр)
  //когда геркон замкнут, значение "0" (LOW), когда разомкнут - "1" (HIGH)
  String out = "sensor1:switch1:" + String(doorstate);
  out.toCharArray(outBuffer, 32);
  radio.write(outBuffer, 32);
  delay(50);
  // Не знаю почему, но периодически трансивер зависает. Помогает повторная инициализация.
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x4c);
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  radio.enableDynamicPayloads();
  radio.powerUp();
  delay(50);
}

void loop() {
  //int doorstate = digitalRead(switchPin); // считываем показания с геркона
  //digitalWrite(ledPin, !doorstate); // инвертированные показания записываем в порт со светодиодом
  //Serial.println(doorstate); // посылаем в последовательный порт значения с геркона
  //когда геркон замкнут, значение "0" (LOW), когда разомкнут - "1" (HIGH)
}
