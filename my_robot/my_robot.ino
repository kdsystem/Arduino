// Тестировалось на Arduino IDE 1.0.1
#include <AFMotor.h>  // Подключаем библиотеку для работы с шилдом 
#include <Cyberlib.h>
//********Управление пинами:
// считаем, что motor1 - левый мотор
// считаем, что motor2 - правый мотор
//Dx_Out;  - установка пина Х как выход
//Dx_In; - установка пина Х  как вход
//Dx_Hihg;-  установка высокого уровна на пине Х
//Dx_Low; - установка низкого уровня на пине Х
//Ax_Read; - чтение аналогового пина Х
#define encoders_init D2_In; D3_In; D2_High; D3_High
#define robot_go D4_Low; D5_High; D6_High; D7_Low
#define robot_stop D4_Low; D5_Low; D6_Low; D7_Low
//#define robot_left  D4_Low; D5_High; D6_Low; D7_Low
//#define robot_right D4_Low; D5_Low; D6_High; D7_Low
#define robot_rotation_left D4_Low; D5_High; D6_Low; D7_High

volatile int8_t course=0;
// Подключаем моторы к клеммникам M1, M2, M3, M4
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

void setup() {
  IntOn();  //включить прерывания
  // Задаем максимальную скорость вращения моторов (аналог работы PWM) 
  motor1.setSpeed(155);
  motor2.setSpeed(155);
  motor1.run(RELEASE);
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

void robot_left()
{
  IntOff();
  motor1.setSpeed(55);
  motor2.setSpeed(55);
  //остановили моторы
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  //поворачиваем правое назад, левое вперед на 180 милисек
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  delay(180);
  IntOn();
}

void robot_right()
{
  IntOff();
  motor1.setSpeed(55);
  motor2.setSpeed(55);
  //остановили моторы
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  //поворачиваем правое вперед, левое назад на 180 милисек
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  delay(180);
  IntOn();
}

void  MotorL()  // обработка внешнего прерывания левое колесо
{ 
  course--; 
  if(course > 0) { robot_left(); }else{ robot_right();}
}  

void  MotorR()  // обработка внешнего прерывания правое колесо
{
   course++; 
   if(course < 0){ robot_right(); }else{ robot_left();}
} 

void loop() {
  // Двигаемся условно вперед одну секунду 
  motor1.run(FORWARD); // Задаем движение вперед
  motor2.run(FORWARD);
  motor1.setSpeed(155); // Задаем скорость движения
  motor2.setSpeed(155); 
  delay(1000);
  
  // Останавливаем двигатели
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  delay(500);
  
  //поворот налево
  robot_left();
}
