#include <AFMotor.h>  // Подключаем библиотеку для работы с шилдом 
//Управление пинами:
// считаем, что motorL - левый мотор
// считаем, что motorR - правый мотор
//Цифровой вывод 11- DC Мотор №1 / Шаговый №1
//Цифровой вывод 3- DC Мотор №2 / Шаговый №1
//Цифровой вывод 5- DC Мотор №3 / Шаговый №2
//Цифровой вывод 6- DC Мотор №4 / Шаговый №2
//Выводы, отвечающие за выбор направления вращения двигателей:
//Цифровые выводы 4, 7, 8 и 12
//Цифровой вывод 9- Сервопривод №1
//Цифровой вывод 10- Сервопривод №2

#define ir_l A0 //пин 1 ИК-датчика
#define ir_ml A1 //пин 2 ИК-датчика
#define ir_mr A2 //пин 3 ИК-датчика
#define ir_r A3 //пин 4 ИК-датчика
String triggered_ir;

const byte MOTOR_SPEED = 80; //скорость передвижения робота
const byte TURN_SPEED = 80;
const int TURN_TIME = 100;
const byte beep_pin = 9;
int i;

volatile int8_t course=0;
// Подключаем моторы к клеммникам M3, M4
AF_DCMotor motorL(3);
AF_DCMotor motorR(4);

void setup() {
  //Serial.begin(9600);
  //pinMode(2,INPUT);
  //pinMode(3,INPUT);
  //motorL.setSpeed(MOTOR_SPEED);
  //motorR.setSpeed(MOTOR_SPEED);
  pinMode(ir_l, INPUT);
  //pinMode(ir_ml, INPUT);
  //pinMode(ir_mr, INPUT);
  pinMode(ir_r, INPUT);
  //pinMode(beep_pin, OUTPUT);
  robot_stop();
  //звуковое оповещение готовности робота
  IntOn();  //включить прерывания
  //for(uint8_t i=0; i<12; i++) beep(70, random(100, 2000));
  //delay(2000);

}

void beep(unsigned char volume, unsigned char delayms){
  analogWrite(9, volume);
  delay(delayms);
  analogWrite(9, 0);
  delay(delayms);
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
  getSensors();
  if(course > 0) { correct_left(); }else{ correct_rigth();}
}  

void  right_interrupt()  
// обработка внешнего прерывания правое колесо
{
   course++;
   //Serial.print("Right interrupt......course ");
   //Serial.println(course);   
   getSensors();
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
