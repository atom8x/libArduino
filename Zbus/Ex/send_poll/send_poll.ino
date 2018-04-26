
#include <Zbus.h>
Zbus Esp(&Serial2,9600);
uint8_t reg[64];

void setup() {
Esp.begin();
Serial.begin(9600);

}
uint32_t Time_G;
uint8_t temp;
void loop() {
 Esp.poll(reg);
uint32_t l_time = millis();
if(l_time >= Time_G){
  Time_G+= 1000;
  temp += 1;
  reg[0] = reg[1] = temp;
  Serial.print("Reg[3] = ");  Serial.println(reg[3]);
  Serial.print("Reg[4] = ");  Serial.println(reg[4]);
//write(uint8_t id,uint8_t func,uint8_t adr,uint8_t nbyte, uint8_t* data)
  Esp.write(1,1,0,5, reg);
  }
};
