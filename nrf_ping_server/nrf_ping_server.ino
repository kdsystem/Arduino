/**
 *
 * Подключение:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Настраиваемые пины:
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
  
  /*
   * Подключение SPI драйвера.
   */
 
  Mirf.spi = &MirfHardwareSpi;
  
  /*
   * Установка пинов.
   */
   
  Mirf.init();
  
  /*
   * Задаём адрес приёмника.
   */
   
  Mirf.setRADDR((byte *)"serv1");
  
  /*
   * Устанавливаем размер полезной нагрузки (payload)
   * Значение должно совпадать для сервера и для клиента
   */
   
  Mirf.payload = sizeof(unsigned long);
  
  /*
   * Включаем приём
   */
   
  Mirf.config();
  
  Serial.println("Listening..."); 
}
 
void loop(){
  /*
   * Приёмный буфер
   */
   
  byte data[Mirf.payload];
  
  /*
   * Если пакет получен
   *
   * isSending так же восстанавливает режим прослушивания эфира
   * при переходе от true к false
   */
   
  if(!Mirf.isSending() && Mirf.dataReady()){
    Serial.println("Got packet");
    
    /*
     * Загружаем пакет в буфер
     */
     
    Mirf.getData(data);
    Mirf.setTADDR((byte *)"clie1");
    
    /*
     * Отправляем данные назад клиенту
     */
     
    Mirf.send(data);
    
    /*
     * Ожидаем окончания передачи
     */
      
    Serial.println("Reply sent.");
  }
}
