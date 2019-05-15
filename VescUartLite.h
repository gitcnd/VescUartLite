#ifndef VescUartLite_h
#define VescUartLite_h
#include <Arduino.h>

/*
 * cnd - see https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5:-Library-specification
 *
=head1 VescUartLite

Lightweight routines to communicate with modern VESC controllers using their UART serial interface

=head2 SYNOPSIS

=for markdown
```C
  #include <VescUartLite.h>

  void setup() {
    VescUartLite.begin(250000,DataReady);	// NB: Use vesc-tool to change vesc default baud to 250000 (115200 default does not work on 8mhz or 16mhz MCUs like arduino)
  }

  void loop() {
    VescUartLite.readMotor(canbusid);		// requests motor data (put 0 for canbusid if you have only 1 vesc) - calls your DataReady() function when it arrives
  }

  int DataReady(uint32_t mask) {	// this gets called when new data comes in.  The mask says what's new.
    Serial2.print("My ERPM is:");
    Serial2.println(VescUartLite.erpm);
  }
```

=cut

Convert all POD herein to markdown thusly:-

perl -MPod::Markdown -e 'Pod::Markdown->new->filter(@ARGV)' libraries/VescUartLite/VescUartLite.h > libraries/VescUartLite/README.md

*/

// Vesc COMM_PACKET_ID Communication commands - see https://github.com/vedderb/bldc/blob/master/datatypes.h
#define COMM_FW_VERSION 0
#define COMM_JUMP_TO_BOOTLOADER 1
#define COMM_ERASE_NEW_APP 2
#define COMM_WRITE_NEW_APP_DATA 3
#define COMM_GET_VALUES 4
#define COMM_SET_DUTY 5
#define COMM_SET_CURRENT 6
#define COMM_SET_CURRENT_BRAKE 7
#define COMM_SET_RPM 8
#define COMM_SET_POS 9
#define COMM_SET_HANDBRAKE 10
#define COMM_SET_DETECT 11
#define COMM_SET_SERVO_POS 12
#define COMM_SET_MCCONF 13
#define COMM_GET_MCCONF 14
#define COMM_GET_MCCONF_DEFAULT 15
#define COMM_SET_APPCONF 16
#define COMM_GET_APPCONF 17
#define COMM_GET_APPCONF_DEFAULT 18
#define COMM_SAMPLE_PRINT 19
#define COMM_TERMINAL_CMD 20
#define COMM_PRINT 21
#define COMM_ROTOR_POSITION 22
#define COMM_EXPERIMENT_SAMPLE 23
#define COMM_DETECT_MOTOR_PARAM 24
#define COMM_DETECT_MOTOR_R_L 25
#define COMM_DETECT_MOTOR_FLUX_LINKAGE 26
#define COMM_DETECT_ENCODER 27
#define COMM_DETECT_HALL_FOC 28
#define COMM_REBOOT 29
#define COMM_ALIVE 30
#define COMM_GET_DECODED_PPM 31
#define COMM_GET_DECODED_ADC 32
#define COMM_GET_DECODED_CHUK 33
#define COMM_FORWARD_CAN 34
#define COMM_SET_CHUCK_DATA 35
#define COMM_CUSTOM_APP_DATA 36
#define COMM_NRF_START_PAIRING 37
#define COMM_GPD_SET_FSW 38
#define COMM_GPD_BUFFER_NOTIFY 39
#define COMM_GPD_BUFFER_SIZE_LEFT 40
#define COMM_GPD_FILL_BUFFER 41
#define COMM_GPD_OUTPUT_SAMPLE 42
#define COMM_GPD_SET_MODE 43
#define COMM_GPD_FILL_BUFFER_INT8 44
#define COMM_GPD_FILL_BUFFER_INT16 45
#define COMM_GPD_SET_BUFFER_INT_SCALE 46
#define COMM_GET_VALUES_SETUP 47
#define COMM_SET_MCCONF_TEMP 48
#define COMM_SET_MCCONF_TEMP_SETUP 49
#define COMM_GET_VALUES_SELECTIVE 50
#define COMM_GET_VALUES_SETUP_SELECTIVE 51
#define COMM_EXT_NRF_PRESENT 52
#define COMM_EXT_NRF_ESB_SET_CH_ADDR 53
#define COMM_EXT_NRF_ESB_SEND_DATA 54
#define COMM_EXT_NRF_ESB_RX_DATA 55
#define COMM_EXT_NRF_SET_ENABLED 56
#define COMM_DETECT_MOTOR_FLUX_LINKAGE_OPENLOOP 57
#define COMM_DETECT_APPLY_ALL_FOC 58
#define COMM_JUMP_TO_BOOTLOADER_ALL_CAN 59
#define COMM_ERASE_NEW_APP_ALL_CAN 60
#define COMM_WRITE_NEW_APP_DATA_ALL_CAN 61
#define COMM_PING_CAN 62
#define COMM_APP_DISABLE_OUTPUT 63
#define COMM_TERMINAL_CMD_SYNC 64
#define COMM_GET_IMU_DATA 65
#define COMM_BM_CONNECT 66
#define COMM_BM_ERASE_FLASH_ALL 67
#define COMM_BM_WRITE_FLASH 68
#define COMM_BM_REBOOT 69
#define COMM_BM_DISCONNECT 70

  // mc_fault_code;
  #define FAULT_CODE_NONE 0
  #define FAULT_CODE_OVER_VOLTAGE 1
  #define FAULT_CODE_UNDER_VOLTAGE 2
  #define FAULT_CODE_DRV 3
  #define FAULT_CODE_ABS_OVER_CURRENT 4
  #define FAULT_CODE_OVER_TEMP_FET 5
  #define FAULT_CODE_OVER_TEMP_MOTOR 6
  #define FAULT_CODE_GATE_DRIVER_OVER_VOLTAGE 7
  #define FAULT_CODE_GATE_DRIVER_UNDER_VOLTAGE 8
  #define FAULT_CODE_MCU_UNDER_VOLTAGE 9
  #define FAULT_CODE_BOOTING_FROM_WATCHDOG_RESET 10
  #define FAULT_CODE_ENCODER_SPI 11
  #define FAULT_CODE_ENCODER_SINCOS_BELOW_MIN_AMPLITUDE 12
  #define FAULT_CODE_ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE 13
  #define FAULT_CODE_FLASH_CORRUPTION 14
  #define FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_1 15
  #define FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_2 16
  #define FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_3 17
  #define FAULT_CODE_UNBALANCED_CURRENTS 18

  // and a few new ones of our own
  #define FAULT_CODE_BAD_PACKET_BYTES 123
  #define FAULT_CODE_BAD_PACKET_CRC 124
  #define FAULT_CODE_PACKET_TIMEOUT 125
  #define FAULT_CODE_SHORT_PACKET 126
  #define FAULT_CODE_UNHANDLED_PACKET 127
  #define FAULT_CODE_PACKET_LENGTH_PROBLEM 128


#define MAX_CMD_BUF 12	  // caution - this is just the right size
#define MAX_RET_BUF 70	  // Vesc bug: max recieved bytes is about 67 or so
#define MAX_PKT_TIME 300  // If it takes longer than this to get the next byte in a packet - assume a fault


int (*user_callback_function)(uint32_t)=0; // Gets called when a complete new packet has arrived from the VESC

class VescUartLite {

 public:
   unsigned char vesccmd[MAX_CMD_BUF];	// Where we construct outgoing commands into
   unsigned char serialbuf[MAX_RET_BUF];// Where we assemble incoming replies into
   unsigned char serialptr=0; 		// Buffer pointer
   unsigned char packetptr=0; 		// Points to start of packet in buffer
   unsigned char serialstate=0xFF;	// Tracks incoming, so we know when a full packet has arrived (0xFF=idle, >0 means awaiting final byte)

   // These public values get updated by serialEvent. Updates are triggered by readMotor / readVesc
   unsigned long last_reply_msec=0;	// The time of the last reply we got
   unsigned char last_reply_packet_id=0;// The format of the last packet we got over serial (eg: COMM_GET_VALUES_SELECTIVE)

   unsigned char fw_version_major=0;
   unsigned char fw_version_minor=0;
   char hw_name[4];
   unsigned char stm32_uuid_8[12];
   unsigned char pairing_done=0;

   uint32_t last_reply_mask=0;		// For COMM_GET_VALUES_SELECTIVE replies - indicates which data values just got updates
   //uint32_t reply_changed_mask=0;	// For COMM_GET_VALUES_SELECTIVE replies - indicates which data values just got updates, and differ from the last reading

   float temp_fet_filtered=0;		// V.0 filtered MOSFET temperature, scale=10
   float temp_motor_filtered=0;		// M.1 filtered motor temperature, scale=10
   double reset_avg_motor_current=0;	// M.2 m_motor_current_sum / m_motor_current_iterations, scale=100
   double reset_avg_input_current=0;	// V.3 m_input_current_sum / m_input_current_iterations, scale=100
   double reset_avg_id=0;		// M.4 Read and reset the average direct axis motor current. (FOC only), scale=100
   double reset_avg_iq=0;		// M.5 Read and reset the average quadrature axis motor current. (FOC only), scale=100
   float duty_cycle_now=0;		// V.6 Maximum current right now ?, scale=1000
   int32_t erpm=0;			// M.7 current ERPM. sign=direction of motor. divide by (motor poles/2) to get RPM. "  return m_pll_speed / ((2.0 * M_PI) / 60.0); " scale=1
   float input_voltage=0;		// V.8, scale=10
   double amp_hours=0;			// V.9 amount of amp hours drawn from the input source., scale=10000
   double amp_hours_charged=0;		// V.10 amount of amp hours fed back into the input source., scale=10000
   double watt_hours=0;			// V.11 amount of watt hours drawn from the input source., scale=10000
   double watt_hours_charged=0;		// V.12 amount of watt hours fed back into the input source., scale=10000
   int32_t tachometer_value=0;		// M.13 number of steps motor has rotated. negative=backwards.  Motor revolutions will be this number divided by (3 * MOTOR_POLE_NUMBER).  , scale=1
   int32_t tachometer_abs_value=0;	// M.14 absolute number of steps the motor has rotated. (see above re div), scale=1
   unsigned char fault_code=0;		// V.15 - see fault_to_string() function
   double pid_pos_now=0;		// M.16 degrees?, scale=1000000
   unsigned char controller_id=0;	// V.17 canbus byte
   float ntc_temp_mos1=0;	// V.17, scale=10
   float ntc_temp_mos2=0;
   float ntc_temp_mos3=0;

/*
=pod

=head2 AVAILABLE DATA

=for markdown
```C
   // These public values get updated by serialEvent. Updates are triggered by readMotor / readVesca / etc
   unsigned long last_reply_msec=0;	// The time of the last reply we got
   unsigned char last_reply_packet_id=0;// The format of the last packet we got over serial (eg: COMM_GET_VALUES_SELECTIVE)

   unsigned char fw_version_major=0;	// eg. 3
   unsigned char fw_version_minor=0;	// eg. 56   (comined is 3.56)
   char hw_name[4];			// eg. 410  (hardware version 4.10)
   unsigned char stm32_uuid_8[12];	// Hex ID of VESC CPU
   unsigned char pairing_done=0;

   uint32_t last_reply_mask=0;		// For COMM_GET_VALUES_SELECTIVE replies - indicates which data values just got updates
   coming soon://uint32_t reply_changed_mask=0;	// For COMM_GET_VALUES_SELECTIVE replies - indicates which data values just got updates, and differ from the last reading

   float temp_fet_filtered=0;		// V.0 filtered MOSFET temperature, scale=10
   float temp_motor_filtered=0;		// M.1 filtered motor temperature, scale=10
   double reset_avg_motor_current=0;	// M.2 m_motor_current_sum / m_motor_current_iterations, scale=100
   double reset_avg_input_current=0;	// V.3 m_input_current_sum / m_input_current_iterations, scale=100
   double reset_avg_id=0;		// M.4 Read and reset the average direct axis motor current. (FOC only), scale=100
   double reset_avg_iq=0;		// M.5 Read and reset the average quadrature axis motor current. (FOC only), scale=100
   float duty_cycle_now=0;		// V.6 Maximum current right now ?, scale=1000
   int32_t erpm=0;			// M.7 current ERPM. sign=direction of motor. divide by (motor poles/2) to get RPM. "  return m_pll_speed / ((2.0 * M_PI) / 60.0); " scale=1
   float input_voltage=0;		// V.8, scale=10
   double amp_hours=0;			// V.9 amount of amp hours drawn from the input source., scale=10000
   double amp_hours_charged=0;		// V.10 amount of amp hours fed back into the input source., scale=10000
   double watt_hours=0;			// V.11 amount of watt hours drawn from the input source., scale=10000
   double watt_hours_charged=0;		// V.12 amount of watt hours fed back into the input source., scale=10000
   int32_t tachometer_value=0;		// M.13 number of steps motor has rotated. negative=backwards.  Motor revolutions will be this number divided by (3 * MOTOR_POLE_NUMBER).  , scale=1
   int32_t tachometer_abs_value=0;	// M.14 absolute number of steps the motor has rotated. (see above re div), scale=1
   unsigned char fault_code=0;		// V.15 - see fault_to_string() function
   double pid_pos_now=0;		// M.16 degrees?, scale=1000000
   unsigned char controller_id=0;	// V.17 canbus byte
   float ntc_temp_mos1=0;	// V.17, scale=10
   float ntc_temp_mos2=0;
   float ntc_temp_mos3=0;
```

=head2 FUNCTIONS

=for markdown
```C

  void begin(BAUD,CallBackFunction);	// Sets baud rate, and lets you tell VescUartLite which of your functions you want to call when data is ready
=cut
*/
   void begin(unsigned long baud, int (*f)(int) ) {
     user_callback_function=f;	// Note what to call for when packets come in
     Serial.begin(baud);
   } // begin



/*
=pod

  unsigned short crc16(data,length) // This is the VESC CRC16 algorithm - it's internal, but public in case others want to use this
=cut
*/
   unsigned short crc16(const unsigned char* data_p, unsigned char length){
     unsigned char x;
     unsigned short crc = 0;
      while (length--){
       x = crc >> 8 ^ *data_p++;
       x ^= x>>4;
       crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x <<5)) ^ ((unsigned short)x);
     }
     return crc;
   } // crc16
 


/*
=pod

   void vescSend(data,length); Sends a command (adds padding and CRC) over UART to the VESC - this is internal, but public in case others want to use this
=cut
*/
   void vescSend(char *data,unsigned char length) { // Puts the command into a CRC-padded packet, and sends it
     unsigned int crc=crc16(data,length);
     Serial.write(2); // we only support type-2 (max 255 byte) packets here
     Serial.write(length);
     while(length--){Serial.write(data[0]);data++;}
     int i=crc>>8; Serial.write(i);  // send the CRC
     i=(crc&0x00FF); Serial.write(i);
     Serial.write(3); // packet terminator
   } // vescSend


/*
=pod

   void commAlive(canbusid); // Call this regularly to tell VESC you're still alive.
=cut
*/
   void commAlive(unsigned char canbusid) {	// Tell it we are still here
     vesccmd[0]=COMM_ALIVE;
     vescSend(vesccmd,1);			// does not return anything
   }

/*
=pod

   void readMotor(canbusid);	// Request VESC data relating to motors be sent back to us
=cut
*/
   // NB: Vesc buffer bug: cannot get all values at once - buffers are too small.
   void readMotor(unsigned char canbusid) {	// Request VESC data relating to motors be sent back to us
     unsigned char i=0;
     vesccmd[i++]=COMM_GET_VALUES_SELECTIVE;	// Get mask 00000000000000010110000010110110 is 1  2  4  5  7 13  14  15  16
     vesccmd[i++]=0b00000000;
     vesccmd[i++]=0b00000001;			// 3322222222221111111111
     vesccmd[i++]=0b11100000;			// 10987654321098765432109876543210
     vesccmd[i++]=0b10110110;
     vescSend(vesccmd,i);			// Some time later, user_callback_function will happen with the result.
   } // readMotor



/*
=pod

   void readVesc(canbusid);		// Request non-motor VESC data be sent back to us
   // NOTE: The VESC Chip's O/S has a bug; serial buffers >64 bytes get truncated, so reading all VESC data now requires you make 2 calls, to get <64 bytes each time
=cut
*/
   void readVesc(unsigned char canbusid) {	// Request non-motor VESC data be sent back to us
     unsigned char i=0;
     vesccmd[i++]=COMM_GET_VALUES_SELECTIVE;	// Get mask 00000000000001101001111101001001 is 0  3  6  8  9  10  11  12  15  17  18
     vesccmd[i++]=0b00000000;
     vesccmd[i++]=0b00000110;
     vesccmd[i++]=0b10011111;
     vesccmd[i++]=0b01001001;
     vescSend(vesccmd,i);			// Some time later, user_callback_function will happen with the result.
   } // readVesc



/*
=pod

   void readFWversion(unsigned char canbusid);	// Issue COMM_FW_VERSION request
=cut
*/
   void readFWversion(unsigned char canbusid) {	// Issue COMM_FW_VERSION request
     vesccmd[0]=COMM_FW_VERSION;
     vescSend(vesccmd,1);			// Some time later, user_callback_function will happen with the result.
   } // readFWversion



   // Tell caller the details of something that just went wrong
   void vescError(unsigned char faultc) {
     fault_code=faultc;					// This is what they look at to see what it was
     if(user_callback_function!=0)user_callback_function(0xFFFFFFFE);                // Tell caller about the problem - 0xFFFFFFFE tells them to note the fault
   } // vescError


   int16_t get_int16_t(unsigned char **data) {int16_t ret=(*data)[0];ret=ret<<8;ret|=(*data)[1];*data+=2; return ret;}
   uint16_t get_uint16_t(unsigned char **data) {uint16_t ret=(*data)[0];ret=ret<<8;ret|=(*data)[1];*data+=2; return ret;}
   int32_t get_int32_t(unsigned char **data) { int32_t ret=(*data)[0]; ret=ret<<8; ret|=(*data)[1];ret=ret<<8; ret|=(*data)[2];ret=ret<<8; ret|=(*data)[3];*data+=4; return ret; }
   uint32_t get_uint32_t(unsigned char **data) { uint32_t ret=(*data)[0]; ret=ret<<8; ret|=(*data)[1];ret=ret<<8; ret|=(*data)[2];ret=ret<<8; ret|=(*data)[3];*data+=4; return ret; }
   float get_float(unsigned char **data,float scale) {int16_t ret=(*data)[0];ret=ret<<8;ret|=(*data)[1];*data+=2; float ret2=ret;ret2/=scale; return ret2;}
   double get_double(unsigned char **data,double scale) { int32_t ret=(*data)[0]; ret=ret<<8; ret|=(*data)[1];ret=ret<<8; ret|=(*data)[2];ret=ret<<8; ret|=(*data)[3];*data+=4; double ret2=ret; ret2/=scale; return ret2; }
   unsigned char get_byte(unsigned char **data) {unsigned char c=(*data)[0];*data+=1; return c;}


/*
=pod

   void vescParsePacket(data,length);	// (internal) Convert incoming packets into vesc data variables
=cut
*/
   void vescParsePacket(unsigned char *data,unsigned char length) {
     unsigned char *maxdata=data+length;
     unsigned int crc=crc16(data,length-3);

     last_reply_packet_id=data[0]; data++; if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);

     switch (last_reply_packet_id) {
       case COMM_FW_VERSION:		// t ln cmd maj min   HW_NAME=410   uuid-12ch			 pairdone  crc     term
					// 2 20  0   3   55   52 49 48 0    76 0 40 0 19 81 53 50 54 55 54 55  0       135 74   3
       case COMM_GET_VALUES_SELECTIVE:	// nb: we deliberately parse before CRC check, so this semi-works when CRC is missing too...
	 if(last_reply_packet_id==COMM_FW_VERSION) {
	   last_reply_mask=0x80000000;
	 } else {
	   last_reply_mask=get_uint32_t(&data); if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }

	 if(last_reply_mask & ((uint32_t)1 << 0)) {
	   temp_fet_filtered=get_float(&data,10.0); 		// V.0 filtered MOSFET temperature, scale=10
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 1)) {
	   temp_motor_filtered=get_float(&data,10.0);	// M.1 filtered motor temperature, scale=10
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 2)) {
	   reset_avg_motor_current=get_double(&data,100.0);	// M.2 m_motor_current_sum / m_motor_current_iterations, scale=100
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 3)) {
	   reset_avg_input_current=get_double(&data,100.0);	// V.3 m_input_current_sum / m_input_current_iterations, scale=100
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 4)) {
	   reset_avg_id=get_double(&data,100.0);		// M.4 Read and reset the average direct axis motor current. (FOC only), scale=100
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 5)) {
	   reset_avg_iq=get_double(&data,100.0);		// M.5 Read and reset the average quadrature axis motor current. (FOC only), scale=100
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 6)) {
	   duty_cycle_now=get_float(&data,1000.0);		// V.6 Maximum current right now ?, scale=1000
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 7)) {
	   erpm=get_uint32_t(&data); // M.7 current ERPM. sign=direction of motor. divide by (motor poles/2) to get RPM. "  return m_pll_speed / ((2.0 * M_PI) / 60.0); " scale=1
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 8)) {
	   input_voltage=get_float(&data,10.0);		// V.8, scale=10
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 9)) {
	   amp_hours=get_double(&data,10000.0);			// V.9 amount of amp hours drawn from the input source., scale=10000
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 10)) {
	   amp_hours_charged=get_double(&data,10000.0);		// V.10 amount of amp hours fed back into the input source., scale=10000
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 11)) {
	   watt_hours=get_double(&data,10000.0);			// V.11 amount of watt hours drawn from the input source., scale=10000
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 12)) {
	   watt_hours_charged=get_double(&data,10000.0);		// V.12 amount of watt hours fed back into the input source., scale=10000
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 13)) {
	   tachometer_value=get_uint32_t(&data); // M.13 number of steps motor has rotated. negative=backwards.  Motor revolutions=this number divided by (3 * MOTOR_POLE_NUMBER).
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 14)) {
	   tachometer_abs_value=get_uint32_t(&data); // M.14 absolute number of steps the motor has rotated. (see above re div), scale=1
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 15)) {
	   fault_code=get_byte(&data); // V.15 - see fault_to_string() function
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 16)) {
	   pid_pos_now=get_double(&data,1000000.0);		// M.16 degrees?, scale=1000000
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 17)) {
	   controller_id=get_byte(&data); // V.17 canbus byte
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }
	 if(last_reply_mask & ((uint32_t)1 << 18)) {
	   ntc_temp_mos1=get_float(&data,10.0);	// V.17, scale=10
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	   ntc_temp_mos2=get_float(&data,10.0);
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	   ntc_temp_mos3=get_float(&data,10.0);
	   if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }

	 // COMM_FW_VERSION
	 if(last_reply_mask & ((uint32_t)1 << 31)) {
   	    fw_version_major=get_byte(&data);
	    if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
            fw_version_minor=get_byte(&data);
	    if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	    int i=0; hw_name[i]=get_byte(&data); while((i<4)&&(hw_name[i++]!=0))hw_name[i]=get_byte(&data);
	    if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	    for(int i=0;i<12;i++) stm32_uuid_8[i]=get_byte(&data);
	    if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	    pairing_done=get_byte(&data);
	    if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 }

	 // data[0] should now be the CRC
	 // uint16_t crc2=get_uint16_t(&data); Serial.print("got CRC="); Serial.print(crc2,HEX);

	 if(crc!=get_uint16_t(&data)) return vescError( FAULT_CODE_BAD_PACKET_CRC );

	 if(data>=maxdata) return vescError(FAULT_CODE_SHORT_PACKET);
	 if(data[0]!=3) return vescError( FAULT_CODE_PACKET_LENGTH_PROBLEM );

	 if(user_callback_function!=0)user_callback_function(last_reply_mask); // tell caller that the data is ready now (and which data is new)

	 break;
       default:
	 Serial.print("Oops - ID="); Serial.println(last_reply_packet_id);
         return vescError(FAULT_CODE_UNHANDLED_PACKET);
     } // switch

   } // vescParsePacket


/*
=pod

  const __FlashStringHelper *fault_to_string(fault_code);	// Convert VESC (and our) fault numbers to words.
=cut
*/
  const __FlashStringHelper *fault_to_string(unsigned char fault) {
    switch (fault) {
      case FAULT_CODE_NONE: return F("None");
      case FAULT_CODE_OVER_VOLTAGE: return F("Over_voltage");
      case FAULT_CODE_UNDER_VOLTAGE: return F("Under_voltage");
      case FAULT_CODE_DRV: return F("Drv");
      case FAULT_CODE_ABS_OVER_CURRENT: return F("Abs_over_current");
      case FAULT_CODE_OVER_TEMP_FET: return F("Over_temp_FET");
      case FAULT_CODE_OVER_TEMP_MOTOR: return F("Over_temp_Motor");
      case FAULT_CODE_GATE_DRIVER_OVER_VOLTAGE: return F("Gate_driver_over_voltage");
      case FAULT_CODE_GATE_DRIVER_UNDER_VOLTAGE: return F("Gate_driver_under_voltage");
      case FAULT_CODE_MCU_UNDER_VOLTAGE: return F("MCU_under_voltage");
      case FAULT_CODE_BOOTING_FROM_WATCHDOG_RESET: return F("Booting_from_watchdog_reset");
      case FAULT_CODE_ENCODER_SPI: return F("Encoder_SPI");
      case FAULT_CODE_ENCODER_SINCOS_BELOW_MIN_AMPLITUDE: return F("Encoder_sincos_below_min_amplitude");
      case FAULT_CODE_ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE: return F("Encoder_sincos_above_max_amplitude");
      case FAULT_CODE_FLASH_CORRUPTION: return F("Flash_corruption");
      case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_1: return F("High_offset_current_Sensor_1");
      case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_2: return F("High_offset_current_Sensor_2");
      case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_3: return F("High_offset_current_Sensor_3");
      case FAULT_CODE_UNBALANCED_CURRENTS: return F("Unbalanced_currents");

      case FAULT_CODE_BAD_PACKET_BYTES: return F("Packet_terminator_missing");
      case FAULT_CODE_BAD_PACKET_CRC: return F("Packet_CRC_mismatch");
      case FAULT_CODE_PACKET_TIMEOUT: return F("Prev_Packet_Timeout");
      case FAULT_CODE_SHORT_PACKET: return F("Not_enought_packet_bytes");
      case FAULT_CODE_UNHANDLED_PACKET: return F("Code_doesnt_understand_these_packets");
      case FAULT_CODE_PACKET_LENGTH_PROBLEM: return F("Unexpected_packet_trailling_bytes");
      default: return F("Unknown");
    }
  } // fault_to_string 


 private:
   // everything is public, to make it easy for callers to tweak internals if they want

} VescUartLite; // VescUartLite


/*
=pod

  void serialEvent();	// Including this header will include this interrupt function (which assembles VESC packets) in your code.
  // SerialEvent occurs whenever a new data comes in the hardware serial RX. Multiple bytes of data may be available.
```
=cut
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    int inChar = Serial.read(); // returns -1 if no chars... sholud never happen
    if(inChar>=0) {
      VescUartLite.serialbuf[VescUartLite.serialptr]=inChar & 0x00FF; // add it to the buffer

      if( (VescUartLite.serialstate!=0xFF) &&
	  ((millis()-VescUartLite.last_reply_msec)>MAX_PKT_TIME)) {	// If it took longer than 300ms to get the next packet byte - something is broken or out-of-sync
	VescUartLite.serialstate=0xFF; 
	VescUartLite.vescError(FAULT_CODE_PACKET_TIMEOUT);		// Tell caller about the problem - calls user_callback_function(0xFFFFFFFE);
      }
      VescUartLite.last_reply_msec=millis();				// Update packet timeout timer

      if(VescUartLite.serialstate==0xFF) {				// We were awaiting a first byte
	if(inChar==2) {							// we only do 255byte pkts
	  VescUartLite.serialstate=0xFE;				// state, so we know we're expecting a length next
	  VescUartLite.serialptr=0;					// position buffer to start, so we don't wrap, to make vescParsePacket() easy
	}
      } else if(VescUartLite.serialstate==0xFE) {			// the previous byte was a packet start, so this must now be the length byte
	VescUartLite.serialstate=inChar+3;				// how many we expect to read now... 3 extra for the crc and term
	VescUartLite.packetptr=VescUartLite.serialptr;			// remember pointer to pkt len, and, +1=Where our first byte of packet will arrive into
      } else {
	if(--VescUartLite.serialstate==0) {				// Got the last byte we were expecting
	  VescUartLite.serialstate=0xFF; 				// prep for next packet
	  if(inChar==3) { 						// correctly got the terminator
	    unsigned char length=VescUartLite.serialbuf[VescUartLite.packetptr];
	    VescUartLite.vescParsePacket(&VescUartLite.serialbuf[VescUartLite.packetptr+1],length+3);
	    //unsigned int crc=VescUartLite.crc16(VescUartLite.serialbuf[VescUartLite.packetptr+1],length);
	    //length+=VescUartLite.packetptri+1; if(length>=MAX_RET_BUF)length-=MAX_RET_BUF; // get pointer to crc
	    
	  } else {
	    VescUartLite.vescError(FAULT_CODE_BAD_PACKET_BYTES);	// Tell caller about the problem - calls user_callback_function(0xFFFFFFFE);
	    Serial.println("missing terminator");
	  }
	}
      } // serialstate

      VescUartLite.serialptr++; if(VescUartLite.serialptr>=MAX_RET_BUF)VescUartLite.serialptr=0;
    } // inChar>=0
  } // Serial.available
} // serialEvent

#define VescUartLite_DEBUG 1

#ifdef VescUartLite_DEBUG 
// for debuggig serialEvent offline
void codeserialEvent(int inChar) {
  if (1) {
    // get the new byte:
    //int inChar = Serial.read(); // returns -1 if no chars... sholud never happen
    if(inChar>=0) {
      VescUartLite.serialbuf[VescUartLite.serialptr]=inChar & 0x00FF; // add it to the buffer

      if( (VescUartLite.serialstate!=0xFF) &&
	  ((millis()-VescUartLite.last_reply_msec)>MAX_PKT_TIME)) {	// If it took longer than 300ms to get the next packet byte - something is broken or out-of-sync
	VescUartLite.serialstate=0xFF; 
	VescUartLite.vescError(FAULT_CODE_PACKET_TIMEOUT);		// Tell caller about the problem - calls user_callback_function(0xFFFFFFFE);
Serial.println("time fault");	
      }
      VescUartLite.last_reply_msec=millis();				// Update packet timeout timer

      if(VescUartLite.serialstate==0xFF) {				// We were awaiting a first byte
	if(inChar==2) {							// we only do 255byte pkts
	  VescUartLite.serialstate=0xFE;				// state, so we know we're expecting a length next
	  VescUartLite.serialptr=0;					// position buffer to start, so we don't wrap, to make vescParsePacket() easy
	}
      } else if(VescUartLite.serialstate==0xFE) {			// the previous byte was a packet start, so this must now be the length byte
	VescUartLite.serialstate=inChar+3;				// how many we expect to read now... 3 extra for the crc and term
	VescUartLite.packetptr=VescUartLite.serialptr;			// remember pointer to pkt len, and, +1=Where our first byte of packet will arrive into
      } else {
	if(--VescUartLite.serialstate==0) {				// Got the last byte we were expecting
	  VescUartLite.serialstate=0xFF; 				// prep for next packet
	  if(inChar==3) { 						// correctly got the terminator
	    unsigned char length=VescUartLite.serialbuf[VescUartLite.packetptr];
	    VescUartLite.vescParsePacket(&VescUartLite.serialbuf[VescUartLite.packetptr+1],length+3);
	    //unsigned int crc=VescUartLite.crc16(VescUartLite.serialbuf[VescUartLite.packetptr+1],length);
	    //length+=VescUartLite.packetptri+1; if(length>=MAX_RET_BUF)length-=MAX_RET_BUF; // get pointer to crc
	    
	  } else {
	    VescUartLite.vescError(FAULT_CODE_BAD_PACKET_BYTES);	// Tell caller about the problem - calls user_callback_function(0xFFFFFFFE);
	    Serial.println("missing terminator");
	  }
	}
      } // serialstate

      VescUartLite.serialptr++; if(VescUartLite.serialptr>=MAX_RET_BUF)VescUartLite.serialptr=0;
    } // inChar>=0
  } // Serial.available
} // serialEvent
#endif // VescUartLite_DEBUG


#endif  // VescUartLite_h


/*
=pod



=head2 HOW TO INSTALL

=head3 (Method 1)


=for markdown
1. Open a terminal (commandline, bash shell, whatever)
2. Change into your Arduino folder
```bash
   cd /Applications/Arduino.app/Contents/Java/libraries/
   (or)
   cd ~/Arduino/libraries/
```
3. grab and install this code
```bash
   git clone https://github.com/gitcnd/VescUartLite.git
```
4. restart your arduino IDE
5. Choose File => Examples => VescUartLite => VescUartLite_example
6. Hit the "build" button and enjoy!


=head3 (Method 2) - see https://www.arduino.cc/en/Guide/Libraries


=for markdown
1. Download the ZIP of this repo: https://github.com/gitcnd/VescUartLite/archive/master.zip
2. In your IDE, select Sketch -> Include Library -> Add .ZIP Library
3. Choose File => Examples => VescUartLite => hello
4. Hit the "build" button and enjoy!


=cut















/*   old code:-
 *
        case COMM_GET_VALUES:
        case COMM_GET_VALUES_SELECTIVE: {
                int32_t ind = 0;
                chMtxLock(&send_buffer_mutex);
                uint8_t *send_buffer = send_buffer_global;
1                send_buffer[ind++] = packet_id;

                uint32_t mask = 0xFFFFFFFF;
                if (packet_id == COMM_GET_VALUES_SELECTIVE) {
                        int32_t ind2 = 0;
                        mask = buffer_get_uint32(data, &ind2);
0000                        buffer_append_uint32(send_buffer, mask, &ind);
                }

#   M  V

                if (mask & ((uint32_t)1 << 0)) {
2      0                 buffer_append_float16(send_buffer, mc_interface_temp_fet_filtered(), 1e1, &ind); //  * Get filtered MOSFET temperature.	
                }
                if (mask & ((uint32_t)1 << 1)) {
2   1                    buffer_append_float16(send_buffer, mc_interface_temp_motor_filtered(), 1e1, &ind); // * Get filtered motor temperature.
                }
                if (mask & ((uint32_t)1 << 2)) {
4   2                    buffer_append_float32(send_buffer, mc_interface_read_reset_avg_motor_current(), 1e2, &ind); // m_motor_current_sum / m_motor_current_iterations
                }
                if (mask & ((uint32_t)1 << 3)) {
4      3                 buffer_append_float32(send_buffer, mc_interface_read_reset_avg_input_current(), 1e2, &ind);	// m_input_current_sum / m_input_current_iterations
                }
                if (mask & ((uint32_t)1 << 4)) {
4   4                    buffer_append_float32(send_buffer, mc_interface_read_reset_avg_id(), 1e2, &ind);	//  * Read and reset the average direct axis motor current. (FOC only)
                }
                if (mask & ((uint32_t)1 << 5)) {
4   5                    buffer_append_float32(send_buffer, mc_interface_read_reset_avg_iq(), 1e2, &ind);	//  * Read and reset the average quadrature axis motor current. (FOC only)
                }
                if (mask & ((uint32_t)1 << 6)) {
2      6                 buffer_append_float16(send_buffer, mc_interface_get_duty_cycle_now(), 1e3, &ind);	// // Maximum current right now ?
                }
                if (mask & ((uint32_t)1 << 7)) {
4   7                    buffer_append_float32(send_buffer, mc_interface_get_rpm(), 1e0, &ind); //  * Calculate the current RPM of the motor. This is a signed value and the sign depends on the direction the motor is rotating in. Note that this value has to be divided by half the number of motor poles.  return m_pll_speed / ((2.0 * M_PI) / 60.0); 
                }
                if (mask & ((uint32_t)1 << 8)) {
2      8                 buffer_append_float16(send_buffer, GET_INPUT_VOLTAGE(), 1e1, &ind);
                }
                if (mask & ((uint32_t)1 << 9)) {
4      9                 buffer_append_float32(send_buffer, mc_interface_get_amp_hours(false), 1e4, &ind);	//  * Get the amount of amp hours drawn from the input source.
                }
                if (mask & ((uint32_t)1 << 10)) {
4      10                buffer_append_float32(send_buffer, mc_interface_get_amp_hours_charged(false), 1e4, &ind); //  * Get the amount of amp hours fed back into the input source.
                }
                if (mask & ((uint32_t)1 << 11)) {
4      11                buffer_append_float32(send_buffer, mc_interface_get_watt_hours(false), 1e4, &ind);	//  * Get the amount of watt hours drawn from the input source.
                }
                if (mask & ((uint32_t)1 << 12)) {
4      12                buffer_append_float32(send_buffer, mc_interface_get_watt_hours_charged(false), 1e4, &ind); //  * Get the amount of watt hours fed back into the input source.
                }
                if (mask & ((uint32_t)1 << 13)) {
4   13                   buffer_append_int32(send_buffer, mc_interface_get_tachometer_value(false), &ind);	//  * Read the number of steps the motor has rotated. This number is signed and will return a negative number when the motor is rotating backwards.  * @return * The tachometer value in motor steps. The number of motor revolutions will be this number divided by (3 * MOTOR_POLE_NUMBER).  
                }
                if (mask && ((uint32_t)1 << 14)) {
4   14                   buffer_append_int32(send_buffer, mc_interface_get_tachometer_abs_value(false), &ind); //  * Read the absolute number of steps the motor has rotated. (see above re div)
                }
                if (mask & ((uint32_t)1 << 15)) {
1       15               send_buffer[ind++] = mc_interface_get_fault();
                }
                if (mask & ((uint32_t)1 << 16)) {
4  16                      buffer_append_float32(send_buffer, mc_interface_get_pid_pos_now(), 1e6, &ind); // degrees?
                }
                if (mask & ((uint32_t)1 << 17)) {
1      17                  send_buffer[ind++] = app_get_configuration()->controller_id;	// canbus byte
                }
       18         if (mask & ((uint32_t)1 << 18)) {
2                        buffer_append_float16(send_buffer, NTC_TEMP_MOS1(), 1e1, &ind);
2                        buffer_append_float16(send_buffer, NTC_TEMP_MOS2(), 1e1, &ind);
2                        buffer_append_float16(send_buffer, NTC_TEMP_MOS3(), 1e1, &ind);
                }

                reply_func(send_buffer, ind);
                chMtxUnlock(&send_buffer_mutex);
        } break;
*/

	    //unsigned int crc=VescUartLite.crc16(VescUartLite.serialbuf[VescUartLite.packetptr+1],length);
	    //length+=VescUartLite.packetptri+1; if(length>=MAX_RET_BUF)length-=MAX_RET_BUF; // get pointer to crc
	    //
/*
 *
 */
