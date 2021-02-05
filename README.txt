README.txt
Farmer Frog Hoop House Controller System Instructions

###############################################################################
Sensor Side Board

There are 6 I2C inputs, which are designed to be used with Atlas Scientific
EZO Dissolved Oxygen (DO) sensors, but may work with similar sensors (with no
guarentees). When using other sensors, uploading custom code may be required.
There are 16 inputs which can read a digital binary value or an analog voltage.
These are configured using settings.txt (see settings.txt section of the
README) When using a digital sensor, if the sensor is open-collector, you must
solder a resistor in the designated location on the Sensor Side Board. See the
Sensor Board Setup section of the Repair Manual for more information.

Note: certain inputs can only be certain types. For more information on what
these types mean, see the "settings.txt" secion of this document.
  IN0: ANALOG, DIGITAL, NDIGITAL, BATTERY, 1WIRE
  IN1: ANALOG, DIGITAL, NDIGITAL, BATTERY, 1WIRE
  IN2: ANALOG, DIGITAL, NDIGITAL, BATTERY, 1WIRE
  IN3: ANALOG, DIGITAL, NDIGITAL, BATTERY, 1WIRE
  IN4: ANALOG, DIGITAL, NDIGITAL, BATTERY, 1WIRE
  IN5: ANALOG, DIGITAL, NDIGITAL, BATTERY, 1WIRE
  IN6: ANALOG, DIGITAL, NDIGITAL, BATTERY, 1WIRE
  IN7: ANALOG, DIGITAL, NDIGITAL, BATTERY, 1WIRE
  IN8: ANALOG, DIGITAL, NDIGITAL, BATTERY, 1WIRE
  IN9: ANALOG, DIGITAL, NDIGITAL, BATTERY, 1WIRE
  INA: ANALOG, DIGITAL, NDIGITAL, BATTERY
  INB: ANALOG, DIGITAL, NDIGITAL, BATTERY, 1WIRE
  INC: DIGITAL, NDIGITAL, BATTERY, 1WIRE
  IND: DIGITAL, NDIGITAL, BATTERY, 1WIRE
  INE: DIGITAL, NDIGITAL, BATTERY, 1WIRE
  INF: DIGITAL, NDIGITAL, BATTERY, 1WIRE

###############################################################################
Relay Side Board

There are 16 outputs on the Relay Side Board. These output 5 volts each and are
  rated for a maximum 120mA. These can be connected to DC triggered relays.

###############################################################################
settings.txt

This file is used to configure your system. The following are descriptions of
  each setting and how you can use it.
  
IMPORTANT: when editing settings.txt, make sure to use Notepad++ using the
  "Macintosh (CR)" line ending setting at the bottom right. Without doing this,
  the product may not start correctly.
  
The first two sets of settings are for your inputs.

I2Cx inputs are configured with either ON or OFF. No parameters follow OFF. The
parameters that follow ON are:
    <address> which is the 7 bit I2C address of the sensor you are connecting.
      7 bits just means that it is between 0 and 127 inclusive.
    <min> this is the absolute minimum that you want this value to be. This is
      NOT the minimum that the sensor can measure. For example, the sensor may
      read down to 0, but the minimum you want the value of what you are
      sensing to be is 40. In that case you would put 40 for <min>.
    <max> this is similar to min but is the high end of what you are sensing.
      For example, if the highest you want the thing you are sensing to be is
      80, you should put 80 for this parameter.
    <outputMin> is the name of the output you want to turn on when the sensor
      is too close to the value <min>. This is written like OUT01, OUT02, etc.
      This should match the name of the output. The numbers go from 01 to 16.
      You can see the rest of the outputs in settings.txt. If you do not want
      to connect the input to an output (or do not have something to connect
      it to), you can write none instead.
    <outputMin> is the name of the output you want to turn on when the sensor
      is too close to the value <max>. This works the same way as <outputMin>.

INx inputs have six possible configurations: OFF, ANALOG, DIGITAL, BATTERY, and
  1WIRE
OFF means that the input does not read anything. There are no parameters for
  this configuration.
ANALOG means that the sensor outputs an analog voltage between 0 and 3.3 volts
  This uses 12 bit ADCS on-board the ESP32-DEVKIT-V1. This has the following
  parameters:
    <min> this is the absolute minimum that you want this value to be. This is
      NOT the minimum that the sensor can measure. For example, the sensor may
      read down to 0, but the minimum you want the value of what you are
      sensing to be is 40. In that case you would put 40 for <min>.
    <max> this is similar to min but is the high end of what you are sensing.
      For example, if the highest you want the thing you are sensing to be is
      80, you should put 80 for this parameter.
    <outputMin> is the name of the output you want to turn on when the sensor
      is too close to the value <min>. This is written like OUT01, OUT02, etc.
      This should match the name of the output. The numbers go from 01 to 16.
      You can see the rest of the outputs in settings.txt. If you do not want
      to connect the input to an output (or do not have something to connect
      it to), you can write none instead.
    <outputMin> is the name of the output you want to turn on when the sensor
      is too close to the value <max>. This works the same way as <outputMin>.
1WIRE means
DIGITAL means that the sensor outputs a digital binary signal, either 0 or 3.3
  volts, where 0 means that the sensor is reading true, and 3.3 means that the
  sensor is reading false (positive logic). This has the following parameter.
    <output> This is the name of the output that you want to turn on when your
      sensor goes high.
NDIGITAL means that the sensor outputs a digital binary signal, either 0 or 3.3
  volts, where 0 means that the sensor is reading false, and 3.3 means that the
  sensor is reading true (negative logic). This has the following parameter.
    <output> This is the name of the output that you want to turn on when your
      sensor goes low.
BATTERY means that the sensor is reading the battery level from the UPS system.
  ONLY SET ONE SIGNAL TO THE BATTERY SETTING. The system is only designed to
  use exactly one battery level. See the UPS section of the Repair Manual for
  more information about this. There are no parameters for this input type.

The next set of settings are for your outputs.

OUTxx outputs are either configured as ON or OFF. There are no parameters for
  OFF. There is one parameter for ON:
    <power> which is how much power the output is expected to consume. It is
      recommended that you keep units consistent, you might want to use Watts
      for all power parameters.

The last line of the settings file is the power limit. This is the amount of
  power that your power supply can reasonably handle. As with the <power> 
  parameter in the output lines, you should use the same units, such as Watts.
  
The following is an example of what could be in settings.txt. This is not the
  default, it is just here to provide an example of ways you can set up the
  system. Notice, everything after the semicolon is comment, you can put any
  regular text after the semicolon.
  
ON,51,0,6,none,none;I2C1 - OFF/ON,<address>,<min>,<max>,<outputMin>,<outputMax>
ON,86,3,8,none,OUT02;I2C2 - OFF/ON,<address>,<min>,<max>,<outputMin>,<outputMax>
OFF;I2C3 - OFF/ON,<address>,<min>,<max>,<outputMin>,<outputMax>
OFF;I2C4 - OFF/ON,<address>,<min>,<max>,<outputMin>,<outputMax>
OFF;I2C5 - OFF/ON,<address>,<min>,<max>,<outputMin>,<outputMax>
OFF;I2C6 - OFF/ON,<address>,<min>,<max>,<outputMin>,<outputMax>
ANALOG,2770,3200,none,none;IN0 - OFF/ANALOG,<min>,<max>,<outputMin>,<outputMax>/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
ANALOG,1000,2000,OUT01,none;IN1 - OFF/ANALOG,<min>,<max>,<outputMin>,<outputMax>/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
OFF;IN2 - OFF/ANALOG,<min>,<max>,<outputMin>,<outputMax>/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
OFF;IN3 - OFF/ANALOG,<min>,<max>,<outputMin>,<outputMax>/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
OFF;IN4 - OFF/ANALOG,<min>,<max>,<outputMin>,<outputMax>/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
OFF;IN5 - OFF/ANALOG,<min>,<max>,<outputMin>,<outputMax>/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
OFF;IN6 - OFF/ANALOG,<min>,<max>,<outputMin>,<outputMax>/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
OFF;IN7 - OFF/ANALOG,<min>,<max>,<outputMin>,<outputMax>/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
OFF;IN8 - OFF/ANALOG,<min>,<max>,<outputMin>,<outputMax>/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
OFF;IN9 - OFF/ANALOG,<min>,<max>,<outputMin>,<outputMax>/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
NDIGITAL,OUT04;INA - OFF/ANALOG,<min>,<max>,<outputMin>,<outputMax>/DIGITAL,<output>/NDIGITAL,<output>/BATTERY
DIGITAL,none;INB - OFF/ANALOG,<min>,<max>,<outputMin>,<outputMax>/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
1WIRE,30,100,none,none;INC - OFF/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
1WIRE,60,80,OUT03,OUT03;IND - OFF/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
OFF;INE - OFF/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
BATTERY;INF - OFF/DIGITAL,<output>/NDIGITAL,<output>/BATTERY/1WIRE,<min>,<max>,<outputMin>,<outputMax>
ON,100;OUT01 - OFF/ON,<power>
ON,600;OUT02 - OFF/ON,<power>
ON,50;OUT03 - OFF/ON,<power>
ON,300;OUT04 - OFF/ON,<power>
OFF;OUT05 - OFF/ON,<power>
OFF;OUT06 - OFF/ON,<power>
OFF;OUT07 - OFF/ON,<power>
OFF;OUT08 - OFF/ON,<power>
OFF;OUT09 - OFF/ON,<power>
OFF;OUT10 - OFF/ON,<power>
OFF;OUT11 - OFF/ON,<power>
OFF;OUT12 - OFF/ON,<power>
OFF;OUT13 - OFF/ON,<power>
OFF;OUT14 - OFF/ON,<power>
OFF;OUT15 - OFF/ON,<power>
OFF;OUT16 - OFF/ON,<power>
1000;POWER_LIMIT

###############################################################################
data.txt

This file contains data from the sensors logged with time and date.

###############################################################################
errors.txt

This file contains error codes outputted by the system.