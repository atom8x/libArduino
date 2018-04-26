
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Zbus.h"


Zbus::Zbus(HardwareSerial *port, long u32speed)
{
    init(port, u32speed);
}

void Zbus::init(HardwareSerial *port, long u32speed){
mp_port = port;
m_speed = u32speed;
}

void Zbus::begin(long u32speed){
    mp_port->begin(u32speed);
    m_speed = u32speed;
}

void Zbus::begin(){
    begin(m_speed);
}

void Zbus::write(uint8_t id,uint8_t func,uint8_t adr,uint8_t nbyte, uint8_t* data){

	uint8_t idx=0;
	uint8_t crc = 0;
	u8Buffer[idx++]=0x5A;
	u8Buffer[idx++]=id;
	u8Buffer[idx++]=func;
	u8Buffer[idx++]=adr;
	u8Buffer[idx++]=nbyte;	

	for(int i=adr; i<(adr+nbyte); i++)	u8Buffer[idx++]=data[i];
	for(int i=1  ; i< idx       ; i++)	crc += u8Buffer[i]; 
	u8Buffer[idx++]= crc ;
	for(int i=0  ; i< idx       ; i++)  mp_port->write(u8Buffer[i]);
}


void Zbus::write(uint8_t data){
	mp_port->write(data);
}



void Zbus::poll(uint8_t* reg){
      uint8_t crc = 0;
      if (mp_port->available())
      {
        if(buffer >= BUFFER_SIZE){

            while (mp_port->available()>0){
                uint8_t dm = mp_port->read();
            };
            buffer=0;
            return;
        }

          frame[buffer] = mp_port->read();
        
            switch (l_state) {
              case 0:
                if(frame[0] == 'Z'){
                            l_state=1;
                            buffer++;
                            quality_byte=4;

                            //Serial.println("Header OK");
                    }
                break;
              case 1:
                  if(buffer==3) {
                        start_add = frame[buffer];
                        //Serial.print("start_add = ");Serial.println(start_add);
                    }
                  if(buffer==4) {
                        nobyte       = frame[buffer];
                        quality_byte=0;
                        quality_byte = buffer + nobyte;
                        //Serial.print("nobyte = ");Serial.println(nobyte);
                    }

                  if((buffer>4)&&(buffer==quality_byte)){
                                l_state=2;
                      }
                      buffer++;
                break;
              case 2://crc
                        crc=0;
                        for(int i=1; i<buffer ;i++){
                            crc+=frame[i];
                            } 
                        if(crc == frame[buffer] ) {
                            for(int i=0;i<nobyte;i++){
                                reg[start_add+i]=frame[5+i];
                            }
                            //Serial.println("MSM OK");
                        }
                        else
                        {   
                            //Error crc
                            //Serial.println("CRC !OK");
                        }
                    l_state=0;
                    buffer=0;
                break;            
              default:
                // do something
                 break;
            } 
      } 

      
    }
