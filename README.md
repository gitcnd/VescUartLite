# VescUartLite

Lightweight routines to communicate with modern VESC controllers using their UART serial interface

## SYNOPSIS


```C
  #include <VescUartLite.h>

    void setup() {
      VescUartLite.begin(250000,DataReady);       // NB: Use vesc-tool to change vesc default baud to 250000 (115200 default does not work on 8mhz or 16mhz MCUs like arduino)
    }

    void loop() {
      VescUartLite.readMotor(canbusid);           // requests motor data (put 0 for canbusid if you have only 1 vesc) - calls your DataReady() function when it arrives
    }

    int DataReady(uint32_t mask) {        // this gets called when new data comes in.  The mask says what's new.
      Serial2.print("My ERPM is:");
      Serial2.println(VescUartLite.erpm);
    }
  ```

## AVAILABLE DATA


```C
   // These public values get updated by serialEvent. Updates are triggered by readMotor / readVesca / etc
   unsigned long last_reply_msec=0;	// The time of the last reply we got
   unsigned char last_reply_packet_id=0;// The format of the last packet we got over serial (eg: COMM_GET_VALUES_SELECTIVE)

    unsigned char fw_version_major=0;    // eg. 3
    unsigned char fw_version_minor=0;    // eg. 56   (comined is 3.56)
    char hw_name[4];                     // eg. 410  (hardware version 4.10)
    unsigned char stm32_uuid_8[12];      // Hex ID of VESC CPU
    unsigned char pairing_done=0;

    uint32_t last_reply_mask=0;          // For COMM_GET_VALUES_SELECTIVE replies - indicates which data values just got updates
    coming soon://uint32_t reply_changed_mask=0; // For COMM_GET_VALUES_SELECTIVE replies - indicates which data values just got updates, and differ from the last reading

    float temp_fet_filtered=0;           // V.0 filtered MOSFET temperature, scale=10
    float temp_motor_filtered=0;         // M.1 filtered motor temperature, scale=10
    double reset_avg_motor_current=0;    // M.2 m_motor_current_sum / m_motor_current_iterations, scale=100
    double reset_avg_input_current=0;    // V.3 m_input_current_sum / m_input_current_iterations, scale=100
    double reset_avg_id=0;               // M.4 Read and reset the average direct axis motor current. (FOC only), scale=100
    double reset_avg_iq=0;               // M.5 Read and reset the average quadrature axis motor current. (FOC only), scale=100
    float duty_cycle_now=0;              // V.6 Maximum current right now ?, scale=1000
    int32_t erpm=0;                      // M.7 current ERPM. sign=direction of motor. divide by (motor poles/2) to get RPM. "  return m_pll_speed / ((2.0 * M_PI) / 60.0); " scale=1
    float input_voltage=0;               // V.8, scale=10
    double amp_hours=0;                  // V.9 amount of amp hours drawn from the input source., scale=10000
    double amp_hours_charged=0;          // V.10 amount of amp hours fed back into the input source., scale=10000
    double watt_hours=0;                 // V.11 amount of watt hours drawn from the input source., scale=10000
    double watt_hours_charged=0;         // V.12 amount of watt hours fed back into the input source., scale=10000
    int32_t tachometer_value=0;          // M.13 number of steps motor has rotated. negative=backwards.  Motor revolutions will be this number divided by (3 * MOTOR_POLE_NUMBER).  , scale=1
    int32_t tachometer_abs_value=0;      // M.14 absolute number of steps the motor has rotated. (see above re div), scale=1
    unsigned char fault_code=0;          // V.15 - see fault_to_string() function
    double pid_pos_now=0;                // M.16 degrees?, scale=1000000
    unsigned char controller_id=0;       // V.17 canbus byte
    float ntc_temp_mos1=0;               // V.17, scale=10
    float ntc_temp_mos2=0;
    float ntc_temp_mos3=0;
 ```

## FUNCTIONS


```C

    void begin(BAUD,CallBackFunction);    // Sets baud rate, and lets you tell VescUartLite which of your functions you want to call when data is ready

    unsigned short crc16(data,length);    // This is the VESC CRC16 algorithm - it's internal, but public in case others want to use this

    void vescSend(data,length);          // Sends a command (adds padding and CRC) over UART to the VESC - this is internal, but public in case others want to use this

    void commAlive(canbusid);            // Call this regularly to tell VESC you're still alive.

    void readMotor(canbusid);            // Request VESC data relating to motors be sent back to us

    void readVesc(canbusid);             // Request non-motor VESC data be sent back to us
    // NOTE: The VESC Chip's O/S has a bug; serial buffers >64 bytes get truncated, so reading all VESC data now requires you make 2 calls, to get <64 bytes each time

    void readFWversion(unsigned char canbusid);  // Issue COMM_FW_VERSION request

    void vescParsePacket(data,length);   // (internal) Convert incoming packets into vesc data variables

    const __FlashStringHelper *fault_to_string(fault_code);       // Convert VESC (and our) fault numbers to words.

    void serialEvent();           // Including this header will include this interrupt function (which assembles VESC packets) in your code.
    // SerialEvent occurs whenever a new data comes in the hardware serial RX. Multiple bytes of data may be available.
  ```

## HOW TO INSTALL

### (Method 1)


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

### (Method 2) - see https://www.arduino.cc/en/Guide/Libraries


1. Download the ZIP of this repo: https://github.com/gitcnd/VescUartLite/archive/master.zip
2. In your IDE, select Sketch -> Include Library -> Add .ZIP Library
3. Choose File => Examples => VescUartLite => hello
4. Hit the "build" button and enjoy!
