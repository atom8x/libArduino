

#define   BUFFER_SIZE 128
class Zbus
{
private:
   // Instance-specific properties
     HardwareSerial *mp_port; 
     long m_speed;
     uint8_t u8Buffer[BUFFER_SIZE];
     uint8_t start_add;
     uint8_t nobyte;
     uint8_t quality_byte;
     uint8_t l_state;
     uint8_t buffer;
     uint8_t frame[BUFFER_SIZE];

 public:
  Zbus(HardwareSerial *port, long u32speed);
  void  begin(long u32speed);
  void  begin();
  void  write(uint8_t data);
  void  write(uint8_t id,uint8_t func,uint8_t adr, uint8_t nbyte, uint8_t* data);
  void  poll(uint8_t* reg);
  void  init(HardwareSerial *port, long u32speed);
};
