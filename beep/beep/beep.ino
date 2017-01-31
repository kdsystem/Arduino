void setup()  { 
  // declare pin 9 to be an output:
  pinMode(9, OUTPUT);
  //beep(50);
  //beep(50);
  //beep(50);
  //delay(1000);
} 

void loop()  { 
  for(uint8_t i=0; i<12; i++) beep(70, random(100, 2000));
  //beep(200); 
}

void beep(unsigned char volume, unsigned char delayms){
  analogWrite(9, volume);      // Almost any value can be used except 0 and 255
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(9, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
}  
