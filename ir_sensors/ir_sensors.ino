#define ir_l A0 //пин 1 ИК-датчика
#define ir_ml A1 //пин 2 ИК-датчика
#define ir_mr A2 //пин 3 ИК-датчика
#define ir_r A3 //пин 4 ИК-датчика
String triggered_ir;

void setup() {                
  Serial.begin(9600);
  pinMode(ir_l, INPUT);
  pinMode(ir_ml, INPUT);
  pinMode(ir_mr, INPUT);
  pinMode(ir_r, INPUT);
  Serial.println("Starting...");
  
}

void loop() {
  triggered_ir=""; 
  if (digitalRead(ir_l) == LOW)
    {
      triggered_ir="ir_l";
      Serial.println("Triggered ir is LEFT");
    }

  if (digitalRead(ir_ml) == LOW)
    {
      triggered_ir="ir_ml";
      Serial.println("Triggered ir is MIDDLE LEFT");
    }

  if (digitalRead(ir_mr) == LOW)
    {
      triggered_ir="ir_mr";
      Serial.println("Triggered ir is MIDDLE RIGHT");
    }

  if (digitalRead(ir_r) == LOW)
    {
      triggered_ir="ir_r";
      Serial.println("Triggered ir is RIGHT");
    }
  delay(100);              // wait for a second
}
