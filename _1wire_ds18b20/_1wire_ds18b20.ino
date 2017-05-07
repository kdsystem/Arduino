#include <OneWire.h>
OneWire ds(4);
void setup() {
Serial.begin(9600); 
}
void loop() {
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
int Temp = (data[1]<< 8)+data[0];
Temp = Temp>>4;
Serial.println(Temp);
}

