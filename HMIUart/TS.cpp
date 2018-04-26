#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "TS.h"



TS::TS(HardwareSerial *port, long u32speed, uint8_t r3, uint8_t ra)
{
    init(port, u32speed, r3, ra);
}

 void TS::init(HardwareSerial *port, long u32speed, uint8_t r3, uint8_t ra){
	mp_port = port;
	m_speed = u32speed;
	m_r3 = r3;
	m_ra = ra;
 }


String TS::FixedSizeString(const String& str, uint8_t size)
{  
    if(str.length() <= size){
		m_string.reserve(size);
	 	m_string = String{str};
	 	while(m_string.length() < size)
		 m_string += ' ';
		}
    else
         m_string=str.substring(0, size);

         return m_string;
}

 void TS::SetFrameHead(uint8_t r3, uint8_t ra){
		this->m_r3 = r3;
		this->m_ra = ra;
  }
 void TS::Write_Register(uint8_t dlen,uint8_t adr, uint8_t* data){
	uint8_t idx=0; 
	u8Buffer[idx++]= m_r3;
	u8Buffer[idx++]= m_ra;
	u8Buffer[idx++]= dlen;
	u8Buffer[idx++]= WR_REG;
	u8Buffer[idx++]= adr;
	for(int i=0; i<(dlen-1); i++) u8Buffer[idx++] = data[i];
	for(int i=0;i<idx;i++) mp_port->write(u8Buffer[i]);
  }
 void TS::Read_Register( uint8_t adr, uint8_t len){
	uint8_t idx=0; 
	u8Buffer[idx++]= m_r3;
	u8Buffer[idx++]= m_ra;
	u8Buffer[idx++]= 3;
	u8Buffer[idx++]= RE_REG;
	u8Buffer[idx++]= adr;
	u8Buffer[idx++]= len;
	for(int i=0;i<idx;i++) {
		mp_port->write(u8Buffer[i]); 
		
	}

  }
  
 void TS::Write_DataFlash(uint8_t dlen, uint16_t adr, uint16_t* data){
	uint8_t idx=0; 
	u8Buffer[idx++]= m_r3;
	u8Buffer[idx++]= m_ra;
	u8Buffer[idx++]= dlen;
	u8Buffer[idx++]= WR_FLASH;
	u8Buffer[idx++]= highByte(adr);
	u8Buffer[idx++]= lowByte(adr);
	uint8_t datalen = (dlen-3)/2;
	for(int i=0; i< datalen; i++) {
		u8Buffer[idx++] = highByte(data[i]);
		u8Buffer[idx++] = lowByte(data[i]);
		}
	for(int i=0;i<idx;i++){
		mp_port->write(u8Buffer[i]);
		//Serial.write(u8Buffer[i]);
		} 

  }


 void TS::Write_DataFlash(uint16_t adr, const String data){
	uint8_t idx=0; 
	uint8_t dlen;
	u8Buffer[idx++]= m_r3;
	u8Buffer[idx++]= m_ra;
	dlen = data.length() + 3;
	u8Buffer[idx++]= dlen;
	u8Buffer[idx++]= WR_FLASH;
	u8Buffer[idx++]= highByte(adr);
	u8Buffer[idx++]= lowByte(adr);
	for(int i=0;i<idx;i++){
		mp_port->write(u8Buffer[i]);
	}
       mp_port->print(data);
    }


 void TS::Read_DataFlash(uint16_t adr, uint8_t len){

	uint8_t idx=0; 
	u8Buffer[idx++]= m_r3;
	u8Buffer[idx++]= m_ra;
	u8Buffer[idx++]= 3;
	u8Buffer[idx++]= RE_FLASH;
	u8Buffer[idx++]= highByte(adr);
	u8Buffer[idx++]= lowByte(adr);
	u8Buffer[idx++]= highByte(len);
	u8Buffer[idx++]= lowByte(len);
	for(int i=0;i<idx;i++) mp_port->write(u8Buffer[i]);

  }
  void  TS::begin(long u32speed){
		mp_port->begin(u32speed);

  }
   void TS::begin(){
		begin(m_speed);
  }  


  void TS:: poll(uint16_t* RAM, uint8_t* REG){

  	if(mp_port->available()){
  		uint8_t rx = mp_port->read();
  		int i=0;
  		switch (m_state) {
  		    case 0:
  		      if(rx == m_r3)m_state++;
  		      break;
  		    case 1:
  		      if(rx == m_ra) m_state++;
  		      else m_state=0;
  		      break;
  		    case 2:
  		      m_lenframe = rx;
  		      m_idx_in=0;
  		      m_state++;
  		      break;  	
  		    case 3:
  		    
  		      u8BufferIn[m_idx_in++] = rx;
  		      if(m_idx_in>=m_lenframe){

				 if(u8BufferIn[0]==RE_FLASH){
					m_ram_adr = word(u8BufferIn[1],u8BufferIn[2]);
					m_ram_num = u8BufferIn[3];
					for( i = 0; i< m_ram_num; i++){
						RAM[m_ram_adr+i] = word(u8BufferIn[4+i*2],u8BufferIn[5+i*2]);
					}
				 }
				
  		      	 	m_state=0;

  		     }   
  		     break;   
  		    default:
  		      // do something
  		       break; 
  		}

  	}
  }


