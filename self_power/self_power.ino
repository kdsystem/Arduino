void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // задействуем последовательный порт
}

void loop() {
  // put your main code here, to run repeatedly:

  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  ADCSRA |= _BV(ADSC); // начало преобразований
  while (bit_is_set(ADCSRA, ADSC)); // измерение
  uint8_t low = ADCL; // сначала нужно прочесть ADCL - это запирает ADCH
  uint8_t high = ADCH; // разлочить оба
  float result = (high << 8) | low;
  result = (1.1 * 1023.0 * 1000) / result; // Результат Vcc в милливольтах
  Serial.println(result);
  long time = millis() / 1000;
  if (time / 60 / 60 < 10) {
    Serial.print ("0");
  }
  Serial.print (time / 60 / 60);
  Serial.print (":");
  if (time / 60 % 60 < 10) {
    Serial.print ("0");
  }
  Serial.print ((time / 60) % 60);
  Serial.print (":");
  if (time % 60 < 10) {
    Serial.print ("0");
  }
  Serial.println (time % 60);
  delay(1000);
}

