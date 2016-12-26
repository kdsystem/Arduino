// Тестировалось на Arduino IDE 1.0.1
#include <AFMotor.h>  // Подключаем библиотеку для работы с шилдом 
#define encoders_init D2_In; D3_In; D2_High; D3_High
#define robot_go D4_Low; D5_High; D6_High; D7_Low
#define robot_stop D4_Low; D5_Low; D6_Low; D7_Low
#define robot_left  D4_Low; D5_High; D6_Low; D7_Low
#define robot_right D4_Low; D5_Low; D6_High; D7_Low
#define robot_rotation_left D4_Low; D5_High; D6_Low; D7_High

volatile int8_t course=0;
// Подключаем моторы к клеммникам M1, M2, M3, M4
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

void setup() {
  IntOn();  //включить прерывания
  // Задаем максимальную скорость вращения моторов (аналог работы PWM) 
  motor1.setSpeed(255);
  motor1.run(RELEASE);
  motor2.setSpeed(255);
  motor2.run(RELEASE);
}

int i;

void Rotation()
{  
                IntOff();
                robot_rotation_left;
                delay_ms(180);
                robot_stop; 
                course=0; 
                IntOn();  
}   

void IntOn()
{
  attachInterrupt(0, MotorL, RISING);  // настроить срабатывание прерывания interrupt0 pin 2 по перерпапду с низкого (Low) на высокий(HIGH)
  attachInterrupt(1, MotorR, RISING);  // настроить срабатывание прерывания interrupt1 pin 3 по перерпапду с низкого (Low) на высокий(HIGH) 
}

void IntOff()
{
  detachInterrupt(0);  // отключить срабатывание прерывания interrupt0
  detachInterrupt(1);  // отключить срабатывание прерывания interrupt1
}  

void  MotorL()  // обработка внешнего прерывания левое колесо
{ 
  course--; 
  if(course > 0) { robot_left; }else{ robot_right;}
}  

void  MotorR()  // обработка внешнего прерывания правое колесо
{
   course++; 
   if(course < 0){ robot_right; }else{ robot_left;}
} 

void loop() {
  // Двигаемся условно вперед одну секунду 
  motor1.run(FORWARD); // Задаем движение вперед
  motor2.run(FORWARD);
  motor1.setSpeed(255); // Задаем скорость движения
  motor2.setSpeed(255); 
  delay(1000);
  
  // Останавливаем двигатели
  /* Очень не рекомендуем резко переключать направление вращения двигателей.
  Лучше дать небольшой промежуток времени.*/
  
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  delay(500);
  
  // Двигаемся в обратном направлении
  motor1.run(BACKWARD);  // Задаем движение назад
  motor2.run(BACKWARD);
  motor1.setSpeed(255);  // Задаем скорость движения 
  motor2.setSpeed(255); 
  delay(1000);
  
  // Останавливаем двигатели  
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(500);
  
  // Разгоняем двигатели в одном направлении
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  for (i=0; i<255; i++) {
    motor1.setSpeed(i); 
    motor2.setSpeed(i); 
    delay(10);
 }
 
 // Останавливаем двигатели  
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(500);
  
  // Разгоняем двигатели в обратном направлении
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  for (i=255; i>=0; i--) {
    motor1.setSpeed(i); 
    motor2.setSpeed(i); 
    delay(10);
 }
  // Останавливаем движение  
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(500);
}
