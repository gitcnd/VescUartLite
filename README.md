# VescUartLite

Lightweight routines to communicate with modern VESC controllers using their UART serial interface

## SYNOPSIS


```C
  #include <VescUartLite.h>
  VESC VescUartLite;

    void setup() {
      VESC.begin(250000,DataReady); // NB: Use vesc-tool to change vesc default baud to 250000 (115200 default does not work on 8mhz or 16mhz MCUs like arduino)
      VESC.readMotor(canbusid); // requests motor data (put 0 for canbusid if you have only 1 vesc) - calls your DataReady() function when it arrives
    }
  ```

## FUNCTIONS


```C

    void begin(BAUD,CallBackFunction);    // Sets baud rate, and lets you tell VescUartLite which of your functions you want to call when data is ready

    unsigned short crc16(data,length) // This is the VESC CRC16 algorithm - it's internal, but public in case others want to use this



    begin(BAUD,CallBackFunction); // Sets baud rate, and lets you tell VescUartLite which of your functions you want to call when data is ready
    readMotor(canbusid);  // Get VESC data relating to motors
    readVesc(canbusid);   // Get non-motor VESC data (battery, temps, etc)
    unsigned short crc16(buffer,length) // The VESC crc16 algorithm
  ```

## HOW TO INSTALL

### (Method 1)

>
> 1. Open a terminal (commandline, bash shell, whatever)
> 2. Change into your Arduino folder
> ```bash
>    cd /Applications/Arduino.app/Contents/Java/libraries/
>    (or)
>    cd ~/Arduino/libraries/
> ```
> 3. grab and install this code
> ```bash
>    git clone https://github.com/gitcnd/VescUartLite.git
> ```
> 4. restart your arduino IDE
> 5. Choose File => Examples => VescUartLite => VescUartLite_example
> 6. Hit the "build" button and enjoy!

### (Method 2) - see https://www.arduino.cc/en/Guide/Libraries

1\. Download the ZIP of this repo: https://github.com/gitcnd/VescUartLite/archive/master.zip
2\. In your IDE, select Sketch -> Include Library -> Add .ZIP Library
3\. Choose File => Examples => VescUartLite => hello
4\. Hit the "build" button and enjoy!
