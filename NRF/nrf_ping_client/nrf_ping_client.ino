/**
 * Пример, который позволяет определить задержку передачи
 *
 * Подключение:
 * SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Настраиваемые:
 * CE -> 8
 * CSN -> 7
 *
 */
 
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
 
void setup(){
 
  Serial.begin(9600);
  /* Для указания пинов, к которым подключены
   * пины модуля, можно использовать переменные:
   *
   * Mirf.csnPin = 9;
   * Mirf.cePin = 7;
   */
  //Если переменные не заданы, используются значения по умолчанию (CE->8, CSN->7)
 
  // Инициализируем SPI интерфейс:
  Mirf.spi = &MirfHardwareSpi;
  // Инициализируем модуль:
  Mirf.init();
 
  // Устанавливаем адрес модуля
 
  Mirf.setRADDR((byte *)"clie1");
 
  // Устанавливаем размер пакета (payload)
  // Здесь устанавливаем payload равным размеру переменной unsigned long
  Mirf.payload = sizeof(unsigned long);
 
  // Включаем передатчик
  Mirf.config();
  Serial.println("Beginning ... ");
}
 
void loop(){
  // Получаем количество миллисекунд, прошедших с момента включения платы:
  unsigned long time = millis();
  // Устанавливаем адрес модуля. которому будем передавать пакет:
  Mirf.setTADDR((byte *)"serv1");
  // Передаём значение переменной time в виде массива байт:
  Mirf.send((byte *) &time);
 
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
 
  delay(1000);
}
