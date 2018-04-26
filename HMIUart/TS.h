
#define  MAX_BUFFER  64	//!< maximum size for the communication buffer in bytes
#define  WR_REG  0x80	
#define  RE_REG  0x81	
#define  WR_FLASH 0x82
#define  RE_FLASH 0x83	

class TS
{
private:
   // Instance-specific properties
   HardwareSerial *mp_port; 
   long m_speed;
   uint8_t   m_r3,m_ra;
   uint8_t   u8Buffer[MAX_BUFFER];
   uint8_t   u8BufferIn[MAX_BUFFER];
   uint8_t   m_lenframe;
   uint8_t   m_state;
   uint8_t   m_idx_in;
   uint16_t  m_ram_adr;
   uint8_t   m_ram_num;
   String    m_string;

 public:
  TS(HardwareSerial *port, long u32speed, uint8_t r3, uint8_t ra);
  void  SetFrameHead(uint8_t r3, uint8_t ra);
  void  Write_Register(uint8_t dlen,uint8_t adr, uint8_t* data);
  void  Read_Register(uint8_t adr, uint8_t len);
  void  Write_DataFlash(uint8_t dlen,uint16_t adr, uint16_t* data);
  void  Write_DataFlash(uint16_t adr,  const String data);
  void  Read_DataFlash(uint16_t adr, uint8_t len);
  void  begin(long u32speed);
  void  begin();
  void  poll(uint16_t* RAM, uint8_t* REG);
  String  FixedSizeString(const String& str, uint8_t size);
  void init(HardwareSerial *port, long u32speed, uint8_t r3, uint8_t ra);
};


