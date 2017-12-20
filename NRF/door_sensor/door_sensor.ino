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
#include <OneWire.h>
// CE,CSN пины
RF24 radio(9, 10);
OneWire ds(4);
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
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200) {
    char outBuffer[40] = "";
    delay(200);
    int doorstate = digitalRead(switchPin); // считываем показания с геркона
    digitalWrite(ledPin, !doorstate); // инвертированные показания записываем в порт со светодиодом
    //Serial.println(doorstate);
    //Узнаем напряжение питания платы
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    ADCSRA |= _BV(ADSC); // начало преобразований
    while (bit_is_set(ADCSRA, ADSC)); // измерение
    uint8_t low = ADCL; // сначала нужно прочесть ADCL - это запирает ADCH
    uint8_t high = ADCH; // разлочить оба
    float volt = (high << 8) | low;
    volt = (1.1 * 1023.0 * 1000) / volt; // Результат Vcc в милливольтах

    //Узнаем температуру DS18b20
    byte data[2];
    ds.reset(); 
    ds.write(0xCC);
    ds.write(0x44);
    delay(750);
    ds.reset();
    ds.write(0xCC);
    ds.write(0xBE);
    data[0] = ds.read(); 
    data[1] = ds.read();
    int temp = (data[1]<< 8)+data[0];
    temp = temp>>4;
    
    //Отправить состояние двери (откр или закр)
    //когда геркон замкнут, значение "0" (LOW), когда разомкнут - "1" (HIGH)
    String out = "s01|sw01:" + String(doorstate)+"|volt:"+volt/1000+"|temp:"+temp+"|";
    Serial.println(out);
    out.toCharArray(outBuffer, 40);
    radio.write(outBuffer, 40);
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
 last_interrupt_time = interrupt_time;
}

void loop() {
  //int doorstate = digitalRead(switchPin); // считываем показания с геркона
  //digitalWrite(ledPin, !doorstate); // инвертированные показания записываем в порт со светодиодом
  //Serial.println(doorstate); // посылаем в последовательный порт значения с геркона
  //когда геркон замкнут, значение "0" (LOW), когда разомкнут - "1" (HIGH)
}
