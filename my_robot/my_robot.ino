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

//#define encoders_init D2_In; D3_In; D2_High; D3_High
//#define robot_go D4_Low; D5_High; D6_High; D7_Low
//#define robot_stop D4_Low; D5_Low; D6_Low; D7_Low
//#define robot_left  D4_Low; D5_High; D6_Low; D7_Low
//#define robot_right D4_Low; D5_Low; D6_High; D7_Low
//#define robot_rotation_left D4_Low; D5_High; D6_Low; D7_High
#define ir_l A0 //пин 1 ИК-датчика
#define ir_ml A1 //пин 2 ИК-датчика
#define ir_mr A2 //пин 3 ИК-датчика
#define ir_r A3 //пин 4 ИК-датчика
String triggered_ir;

const byte MOTOR_SPEED = 80; //скорость передвижения робота
const byte TURN_SPEED = 80;
const int TURN_TIME = 100;
int i;

volatile int8_t course=0;
// Подключаем моторы к клеммникам M3, M4
AF_DCMotor motorL(3);
AF_DCMotor motorR(4);

void setup() {
  //звуковое оповещение готовности робота
  //for(uint8_t i=0; i<12; i++) beep(70, random(100, 2000));
  //Serial.begin(9600);
  //pinMode(2,INPUT);
  //pinMode(3,INPUT);
  //motorL.setSpeed(MOTOR_SPEED);
  //motorR.setSpeed(MOTOR_SPEED);
  pinMode(ir_l, INPUT);
  pinMode(ir_ml, INPUT);
  pinMode(ir_mr, INPUT);
  pinMode(ir_r, INPUT);
  robot_stop();
  IntOn();  //включить прерывания
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

void TurnLeft(String turn)
{
  if (turn == "left")
  {
    IntOff();
    motorL.setSpeed(TURN_SPEED);
    motorR.setSpeed(TURN_SPEED);
    //поворачиваем левое назад
    motorR.run(RELEASE);
    motorL.run(BACKWARD);
    delay(TURN_TIME);
    //IntOn();          
  }
  if (turn == "middle_left")
  {
    IntOff();
    motorL.setSpeed(TURN_SPEED);
    motorR.setSpeed(TURN_SPEED);
    //поворачиваем левое назад
    motorR.run(RELEASE);
    motorL.run(BACKWARD);
    delay(TURN_TIME/2);
    //IntOn();          
  }
}

void TurnRight(String turn)
{
  if (turn == "right")
  {
    motorL.setSpeed(TURN_SPEED);
    motorR.setSpeed(TURN_SPEED);
    //поворачиваем левое назад
    motorL.run(RELEASE);
    motorR.run(BACKWARD);
    delay(TURN_TIME);
  }
  if (turn == "middle_right")
  {
    motorL.setSpeed(TURN_SPEED);
    motorR.setSpeed(TURN_SPEED);
    //поворачиваем левое назад
    motorL.run(RELEASE);
    motorR.run(BACKWARD);
    delay(TURN_TIME/2);
  }
}

void getSensors()
{
  if (digitalRead(ir_l) == LOW)
    {
      triggered_ir="ir_l";
      TurnLeft("left");
      //Serial.println("Triggered ir is LEFT");
    }

  if (digitalRead(ir_ml) == LOW)
    {
      triggered_ir="ir_ml";
      TurnLeft("middle_left");
      //Serial.println("Triggered ir is MIDDLE LEFT");
    }

  if (digitalRead(ir_mr) == LOW)
    {
      triggered_ir="ir_mr";
      TurnRight("right");
      //Serial.println("Triggered ir is MIDDLE RIGHT");
    }

  if (digitalRead(ir_r) == LOW)
    {
      triggered_ir="ir_r";
      TurnRight("middle_right");
      //Serial.println("Triggered ir is RIGHT");
    }
}

void correct_rigth()
{
  IntOff();
  motorL.setSpeed(TURN_SPEED);
  motorR.setSpeed(TURN_SPEED);
  //поворачиваем правое назад
  motorL.run(RELEASE);
  motorR.run(BACKWARD);
  IntOn();
}

void correct_left()
{
  IntOff();
  motorL.setSpeed(TURN_SPEED);
  motorR.setSpeed(TURN_SPEED);
  //поворачиваем левое назад
  motorR.run(RELEASE);
  motorL.run(BACKWARD);
  IntOn();
}

void  left_interrupt()  
// обработка внешнего прерывания левое колесо
{ 
  course--;
  //Serial.print("Left interrupt......course "); 
  //Serial.println(course);   
  getSensors;
  if(course > 0) { correct_left(); }else{ correct_rigth();}
}  

void  right_interrupt()  
// обработка внешнего прерывания правое колесо
{
   course++;
   //Serial.print("Right interrupt......course ");
   //Serial.println(course);   
   getSensors;
   if(course < 0){ correct_rigth(); }else{ correct_left();}
} 

void robot_stop()
{
  //остановили моторы
  motorL.run(RELEASE); 
  motorR.run(RELEASE);
}

void loop() {
  //Задаем скорость движения
  motorL.setSpeed(MOTOR_SPEED);
  motorR.setSpeed(MOTOR_SPEED); 
  // Двигаемся вперед
  motorL.run(FORWARD);
  motorR.run(FORWARD);
}
