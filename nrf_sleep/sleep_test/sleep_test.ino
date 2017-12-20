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
int ledPin = LED_BUILTIN; // к пину 3 подключён cветодиод (13 - встроенный)

void setup() {
  pinMode(ledPin, OUTPUT); // задаём вывод 3 в качестве выхода
  Serial.begin(19200); // задействуем последовательный порт
  // Инициализируем канал связи и не только.
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x4c);
  radio.openWritingPipe(0xf0f0f0f0d2LL);
  radio.enableDynamicPayloads();
  radio.powerUp();
  //Установим прерывание
  //attachInterrupt(0, Send2HUB, CHANGE );
}

void Send2HUB() {
    char outBuffer[40] = "";
    delay(200);
    //int doorstate = digitalRead(switchPin); // считываем показания с геркона
    digitalWrite(ledPin, HIGH); // инвертированные показания записываем в порт со светодиодом
    delay(2000);
    //Serial.println(doorstate);
    //Узнаем напряжение питания платы
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    ADCSRA |= _BV(ADSC); // начало преобразований
    while (bit_is_set(ADCSRA, ADSC)); // измерение
    uint8_t low = ADCL; // сначала нужно прочесть ADCL - это запирает ADCH
    uint8_t high = ADCH; // разлочить оба
    float volt = (high << 8) | low;
    volt = (1.1 * 1023.0 * 1000) / volt; // Результат Vcc в милливольтах

    //Отправить состояние двери (откр или закр)
    //когда геркон замкнут, значение "0" (LOW), когда разомкнут - "1" (HIGH)
    //String out = "s02|sw02:" + String(doorstate)+"|volt:"+volt/1000+"|temp:"+"text"+"|";
    String out = "s02|volt:5.5|";
    //Serial.println(out);
    out.toCharArray(outBuffer, 40);

    radio.begin();
    radio.setPALevel(RF24_PA_MAX);
    radio.setChannel(0x4c);
    radio.openWritingPipe(0xF0F0F0F0E1LL);
    radio.enableDynamicPayloads();
    radio.powerUp();
    delay(50);

    radio.write(outBuffer, 40);
    Serial.println("Sended:"+out);
    radio.powerDown();
    digitalWrite(ledPin, LOW);
    //delay(50);
    // Не знаю почему, но периодически трансивер зависает. Помогает повторная инициализация.
}

void loop() {
  Send2HUB();
  //int doorstate = digitalRead(switchPin); // считываем показания с геркона
  //digitalWrite(ledPin, !doorstate); // инвертированные показания записываем в порт со светодиодом
  //Serial.println(doorstate); // посылаем в последовательный порт значения с геркона
  //когда геркон замкнут, значение "0" (LOW), когда разомкнут - "1" (HIGH)
}
