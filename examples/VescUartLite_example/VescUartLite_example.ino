/*
      CAUTION!! This will spin up your motors, so be prepared!!

  VescUartLite_example.cpp v1.0 - shows how to read VESC data, and write VESC commands.

  (C) Copyright 2018 (GNU GENERAL PUBLIC LICENSE, Version 3) - Chris Drake <cdrake@cpan.org>
*/

String VERSION;

#include <VescUartLite.h>

// VescUartLite VescUartLite;

int cnd_ledPin = 13;
byte cnd_ledtoggle=0;
unsigned int cnd_ledon=20;
unsigned int cnd_ledoff=500;
unsigned long cnd_ledtimer=millis();

unsigned char testbuf[70];

void setup() {
  VERSION=F("\n#\tv2.04 " __FILE__ "\t" __DATE__ " " __TIME__ "\n");
  pinMode(cnd_ledPin,OUTPUT);

  VescUartLite.begin(250000,DataArrived);	// avoid using 115200 on 8mhz or 16mhz MCUs.
  Serial.print(VERSION);	// VESC will ignore this malformed "packet" - it is here so if we plug our MCU into a PC, we can see what it's running.

  // unsigned short test=VescUartLite.crc16(testbuf,20);


codeserialEvent( 2 );	//packet_id
codeserialEvent( 45 );	//length
codeserialEvent( 50 );	//COMM_GET_VALUES_SELECTIVE
codeserialEvent( 0 );   // mask
codeserialEvent( 7 );   // mask
codeserialEvent( 225 ); // mask
codeserialEvent( 207 ); // mask
codeserialEvent( 1 );
codeserialEvent( 19 );
codeserialEvent( 249 );
codeserialEvent( 136 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 7 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 55 );
codeserialEvent( 0 );
codeserialEvent( 229 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 9 );
codeserialEvent( 88 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 15 );
codeserialEvent( 126 );
codeserialEvent( 0 );
codeserialEvent( 3 );
codeserialEvent( 216 );
codeserialEvent( 167 );
codeserialEvent( 180 );
// delay(1000);
codeserialEvent( 110 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 0 );
codeserialEvent( 205 );
codeserialEvent( 206 );
codeserialEvent( 3 );

Serial.println("-- pkt 1--");

codeserialEvent( 2 );
codeserialEvent( 20 );
codeserialEvent( 0 );
codeserialEvent( 3 );
codeserialEvent( 55 );
codeserialEvent( 52 );
codeserialEvent( 49 );
codeserialEvent( 48 );
codeserialEvent( 0 );
codeserialEvent( 76 );
codeserialEvent( 0 );
codeserialEvent( 40 );
codeserialEvent( 0 );
codeserialEvent( 19 );
codeserialEvent( 81 );
codeserialEvent( 53 );
codeserialEvent( 50 );
codeserialEvent( 54 );
codeserialEvent( 55 );
codeserialEvent( 54 );
codeserialEvent( 55 );
codeserialEvent( 0 );
codeserialEvent( 135 );
codeserialEvent( 74 );
codeserialEvent( 3 );



} // setup

// prefix our example output with "**" - 1st * means the data is new, 2nd * means it's new *and* different to earlier (not impl yet).
void flagnew(uint32_t mask1,uint32_t mask2,unsigned char *i) {
  if(mask1 & ((uint32_t)1 << *i)) Serial.print(F("*")); else Serial.print(F(" ")); 
  if(mask2 & ((uint32_t)1 << *i)) Serial.print(F("*")); else Serial.print(F(" ")); 

  Serial.print(*i);

  *i+=1;
}

int DataArrived(uint32_t mask) {
  if(mask==0xFFFFFFFE) {
    Serial.print(F("Callback error from VESC: ")); Serial.println(VescUartLite.fault_code);
    Serial.println(VescUartLite.fault_to_string(VescUartLite.fault_code));
  } else {
    unsigned char i=0; uint32_t m2=0; // =VescUartLite.reply_changed_mask;
    Serial.print(F("Got VESC Data (mask="));  Serial.print(mask,HEX); Serial.println(F(") :- "));

    Serial.print(F("Reply is ")); Serial.print(millis()-VescUartLite.last_reply_msec); Serial.println(F("ms old."));

    			 Serial.print(F("  last_reply_packet_id="));Serial.print(VescUartLite.last_reply_packet_id); Serial.println(F(" \t: The format of the last packet we got over serial (eg: COMM_GET_VALUES_SELECTIVE) )"));
     			 Serial.print(F("  last_reply_mask="));	 Serial.print(VescUartLite.last_reply_mask,HEX); Serial.println(F(" \t: For COMM_GET_VALUES_SELECTIVE replies - indicates which data values just got updates )"));
   //uint32_t reply_changed_mask=0;	// For COMM_GET_VALUES_SELECTIVE replies - indicates which data values just got updates, and differ from the last reading

    flagnew(mask,m2,&i); Serial.print(F("temp_fet_filtered="));	Serial.print(VescUartLite.temp_fet_filtered); Serial.println(F(" \t: V.0 filtered MOSFET temperature"));
    flagnew(mask,m2,&i); Serial.print(F("temp_motor_filtered="));Serial.print(VescUartLite.temp_motor_filtered); Serial.println(F(" \t: M.1 filtered motor temperature"));
    flagnew(mask,m2,&i); Serial.print(F("reset_avg_motor_current="));Serial.print(VescUartLite.reset_avg_motor_current); Serial.println(F(" \t: M.2 m_motor_current_sum / m_motor_current_iterations"));
    flagnew(mask,m2,&i); Serial.print(F("reset_avg_input_current="));Serial.print(VescUartLite.reset_avg_input_current); Serial.println(F(" \t: V.3 m_input_current_sum / m_input_current_iterations"));
    flagnew(mask,m2,&i); Serial.print(F("reset_avg_id="));Serial.print(VescUartLite.reset_avg_id); Serial.println(F(" \t: M.4 Read and reset the average direct axis motor current. (FOC only)"));
    flagnew(mask,m2,&i); Serial.print(F("reset_avg_iq="));Serial.print(VescUartLite.reset_avg_iq); Serial.println(F(" \t: M.5 Read and reset the average quadrature axis motor current. (FOC only)"));
    flagnew(mask,m2,&i); Serial.print(F("duty_cycle_now="));Serial.print(VescUartLite.duty_cycle_now); Serial.println(F(" \t: V.6 Maximum current right now ?"));
    flagnew(mask,m2,&i); Serial.print(F("erpm="));Serial.print(VescUartLite.erpm); Serial.println(F(" \t: M.7 current ERPM. sign=direction of motor. divide by (motor poles/2) to get RPM."));
    flagnew(mask,m2,&i); Serial.print(F("input_voltage="));Serial.print(VescUartLite.input_voltage); Serial.println(F(" \t: V.8"));
    flagnew(mask,m2,&i); Serial.print(F("amp_hours="));Serial.print(VescUartLite.amp_hours); Serial.println(F(" \t: V.9 amount of amp hours drawn from the input source."));
    flagnew(mask,m2,&i); Serial.print(F("amp_hours_charged="));Serial.print(VescUartLite.amp_hours_charged); Serial.println(F(" \t: V.10 amount of amp hours fed back into the input source."));
    flagnew(mask,m2,&i); Serial.print(F("watt_hours="));Serial.print(VescUartLite.watt_hours); Serial.println(F(" \t: V.11 amount of watt hours drawn from the input source."));
    flagnew(mask,m2,&i); Serial.print(F("watt_hours_charged="));Serial.print(VescUartLite.watt_hours_charged); Serial.println(F(" \t: V.12 amount of watt hours fed back into the input source."));
    flagnew(mask,m2,&i); Serial.print(F("tachometer_value="));Serial.print(VescUartLite.tachometer_value); Serial.println(F(" \t: M.13 number of steps motor has rotated. negative=backwards.  Motor revolutions will be this number divided by (3 * MOTOR_POLE_NUMBER).  "));
    flagnew(mask,m2,&i); Serial.print(F("tachometer_abs_value="));Serial.print(VescUartLite.tachometer_abs_value); Serial.println(F("  \t: M.14 absolute number of steps the motor has rotated. (see above re div)"));
    flagnew(mask,m2,&i); Serial.print(F("fault_code="));Serial.print(VescUartLite.fault_code); Serial.println(F(" \t: V.15 - see fault_to_string() function )"));
    flagnew(mask,m2,&i); Serial.print(F("pid_pos_now="));Serial.print(VescUartLite.pid_pos_now); Serial.println(F(" \t: M.16 degrees?"));
    flagnew(mask,m2,&i); Serial.print(F("controller_id="));Serial.print(VescUartLite.controller_id); Serial.println(F(" \t: V.17 canbus byte )"));
    flagnew(mask,m2,&i); Serial.print(F("ntc_temp_mos1="));Serial.print(VescUartLite.ntc_temp_mos1); Serial.println(F(" \t: V.18"));
    i--;flagnew(mask,m2,&i); Serial.print(F("ntc_temp_mos2="));Serial.print(VescUartLite.ntc_temp_mos2); Serial.println();
    i--;flagnew(mask,m2,&i); Serial.print(F("ntc_temp_mos3="));Serial.print(VescUartLite.ntc_temp_mos3); Serial.println();

    i=32;
    i--;flagnew(mask,m2,&i); Serial.print(F("fw_version_major="));Serial.print( VescUartLite.fw_version_major ); Serial.println();
    i--;flagnew(mask,m2,&i); Serial.print(F("fw_version_minor="));Serial.print( VescUartLite.fw_version_minor ); Serial.println();
    i--;flagnew(mask,m2,&i); Serial.print(F("hw_name="));Serial.print( VescUartLite.hw_name ); Serial.println();
    i--;flagnew(mask,m2,&i); Serial.print(F("stm32_uuid_8="));for(int i=0;i<12;i++){ Serial.print(VescUartLite.stm32_uuid_8[i],HEX); Serial.print(" ");}; Serial.println();
    i--;flagnew(mask,m2,&i); Serial.print(F("pairing_done="));Serial.print( VescUartLite.pairing_done ); Serial.println();
  }
  return 0;
}

void loop() {


  // Blink the LED so we know we are alive
  if(cnd_ledtimer<millis()) {
    cnd_ledtoggle++; digitalWrite(cnd_ledPin, cnd_ledtoggle&1);
    cnd_ledtimer=millis();
    if(cnd_ledtoggle&1) { cnd_ledtimer+=cnd_ledon; } else {cnd_ledtimer+=cnd_ledoff;}
  }
  
} // loop


/* example output:

Got VESC Data (mask=80000000) :- 
Reply is 3ms old.
  last_reply_packet_id=0 	: The format of the last packet we got over serial (eg: COMM_GET_VALUES_SELECTIVE) )
  last_reply_mask=80000000 	: For COMM_GET_VALUES_SELECTIVE replies - indicates which data values just got updates )
  0temp_fet_filtered=27.50 	: V.0 filtered MOSFET temperature
  1temp_motor_filtered=-165.60 	: M.1 filtered motor temperature
  2reset_avg_motor_current=0.07 	: M.2 m_motor_current_sum / m_motor_current_iterations
  3reset_avg_input_current=0.00 	: V.3 m_input_current_sum / m_input_current_iterations
  4reset_avg_id=0.00 	: M.4 Read and reset the average direct axis motor current. (FOC only)
  5reset_avg_iq=0.00 	: M.5 Read and reset the average quadrature axis motor current. (FOC only)
  6duty_cycle_now=0.00 	: V.6 Maximum current right now ?
  7erpm=55 	: M.7 current ERPM. sign=direction of motor. divide by (motor poles/2) to get RPM.
  8input_voltage=22.90 	: V.8
  9amp_hours=0.00 	: V.9 amount of amp hours drawn from the input source.
  10amp_hours_charged=0.00 	: V.10 amount of amp hours fed back into the input source.
  11watt_hours=0.00 	: V.11 amount of watt hours drawn from the input source.
  12watt_hours_charged=0.00 	: V.12 amount of watt hours fed back into the input source.
  13tachometer_value=2392 	: M.13 number of steps motor has rotated. negative=backwards.  Motor revolutions will be this number divided by (3 * MOTOR_POLE_NUMBER).  
  14tachometer_abs_value=3966  	: M.14 absolute number of steps the motor has rotated. (see above re div)
  15fault_code=0 	: V.15 - see fault_to_string() function )
  16pid_pos_now=64.53 	: M.16 degrees?
  17controller_id=110 	: V.17 canbus byte )
  18ntc_temp_mos1=0.00 	: V.18
  18ntc_temp_mos2=0.00
  18ntc_temp_mos3=0.00
* 31fw_version_major=3
* 31fw_version_minor=55
* 31hw_name=410
* 31stm32_uuid_8=4C 0 28 0 13 51 35 32 36 37 36 37 
* 31pairing_done=0

*/
