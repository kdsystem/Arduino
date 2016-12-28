#include <AFMotor.h>  // Подключаем библиотеку для работы с шилдом 
#include <Cyberlib.h>
//Управление пинами:
// считаем, что motorL - левый мотор
// считаем, что motorR - правый мотор
//Dx_Out;  - установка пина Х как выход
//Dx_In; - установка пина Х  как вход
//Dx_Hihg;-  установка высокого уровна на пине Х
//Dx_Low; - установка низкого уровня на пине Х
//Ax_Read; - чтение аналогового пина Х
//Выводы, отвечающие за скорость вращения двигателей
//Цифровой вывод 11- DC Мотор №1 / Шаговый №1
//Цифровой вывод 3- DC Мотор №2 / Шаговый №1
//Цифровой вывод 5- DC Мотор №3 / Шаговый №2
//Цифровой вывод 6- DC Мотор №4 / Шаговый №2
//Выводы, отвечающие за выбор направления вращения двигателей:
//Цифровые выводы 4, 7, 8 и 12
//Выводы для управления сервоприводами (выведены на штырьки на краю платы):
//Цифровой вывод 9- Сервопривод №1
//Цифровой вывод 10- Сервопривод №2

#define encoders_init D2_In; D3_In; D2_High; D3_High
//#define robot_go D4_Low; D5_High; D6_High; D7_Low
//#define robot_stop D4_Low; D5_Low; D6_Low; D7_Low
//#define robot_left  D4_Low; D5_High; D6_Low; D7_Low
//#define robot_right D4_Low; D5_Low; D6_High; D7_Low
#define robot_rotation_left D4_Low; D5_High; D6_Low; D7_High

const byte MOTOR_SPEED = 155; //скорость передвижения робота
int i;

volatile int8_t course=0;
// Подключаем моторы к клеммникам M3, M4
AF_DCMotor motorL(3);
AF_DCMotor motorR(4);

void setup() {
  IntOn();  //включить прерывания
  motorL.setSpeed(MOTOR_SPEED);
  motorR.setSpeed(MOTOR_SPEED);
  robot_stop();
}

void Rotation()
{  
  IntOff();
  robot_rotation_left;
  delay_ms(180);
  robot_stop(); 
  course=0; 
  IntOn();  
}   

void IntOn()
{
  attachInterrupt(0, left_interrupt, RISING);  // настроить срабатывание прерывания interrupt0 pin 2 по перепаду с низкого (Low) на высокий(HIGH)
  attachInterrupt(1, right_interrupt, RISING);  // настроить срабатывание прерывания interrupt1 pin 3 по перепаду с низкого (Low) на высокий(HIGH) 
}

void IntOff()
{
  detachInterrupt(0);  // отключить срабатывание прерывания interrupt0
  detachInterrupt(1);  // отключить срабатывание прерывания interrupt1
}  

void robot_left()
{
  IntOff();
  motorL.setSpeed(MOTOR_SPEED);
  motorR.setSpeed(MOTOR_SPEED);
  //остановили моторы
  robot_stop();
  //поворачиваем правое назад, левое вперед 1сек
  motorL.run(BACKWARD);
  motorR.run(FORWARD);
  delay(1000);
  robot_stop();
  course=0;
  IntOn();
}

void robot_right()
{
  IntOff();
  motorL.setSpeed(MOTOR_SPEED);
  motorR.setSpeed(MOTOR_SPEED);
  //остановили моторы
  robot_stop();
  //поворачиваем правое вперед, левое назад 1сек
  motorL.run(FORWARD);
  motorR.run(BACKWARD);
  delay(1000);
  robot_stop();
  course=0;
  IntOn();
}

void  left_interrupt()  
// обработка внешнего прерывания левое колесо
{ 
  course--; 
  if(course > 0) { robot_left(); }else{ robot_right();}
}  

void  right_interrupt()  
// обработка внешнего прерывания правое колесо
{
   course++; 
   if(course < 0){ robot_right(); }else{ robot_left();}
} 

void robot_stop()
{
  //остановили моторы
  motorL.run(RELEASE); 
  motorR.run(RELEASE);
}

void loop() {
  // Двигаемся условно вперед пять секунд
  motorL.run(FORWARD); // Задаем движение вперед
  motorR.run(FORWARD);
  motorL.setSpeed(MOTOR_SPEED); // Задаем скорость движения
  motorR.setSpeed(MOTOR_SPEED); 
  delay(5000);
  
  // Останавливаем двигатели
  robot_stop();
  delay(500);
  
  //поворот налево
  robot_left();
  delay(500);
  robot_stop();
  
}
