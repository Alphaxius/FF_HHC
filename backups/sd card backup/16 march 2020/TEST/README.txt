*******************************************************************************
**  How to use the test software to make sure your components are working    **
**  correctly. For Farmer Frog Hoop House Controller. 3 March 2020.          **
*******************************************************************************

All of the following instructions should be completed using the Arduino IDE.
You should already have the ESP32-DEVKIT-V1 drivers installed. If you do not,
we recommend this tutorial: {{{LINK}}}.

Tests 1 through 6 test the relay (output) side circuit board, and references to
"the circuit board" during these tests refer to the relay side circuit board.
Tests 7 through 11 test the sensor (input) side circuit board, and references
follow as before. Tests 12 through 13 use both boards.

1.  Ensure that nothing is plugged into the screw terminals. Load dlatch and
    run. Using an LED and a resistor in series (this should be in the box after
    installation), put the anode into the signal terminal and the cathode into
    the ground terminal of each screw terminal on the relay board. Observe that
    each output flashes the LED. If you do not observe this, consult the repair
    manual (Section XXX).
2.  Load lcd_test and run. Observe that the LCD backlight flashes then stays
    on. Then, the screen should fill up with the letter 'a' coming up in
    individual character spots. You may also see the letter 'b' appear briefly
    before the letter 'a'; this is normal and expected. If you do not observe
    this, consult the repair manual (Section XXX).
3.  Load RTCtest and run. Open the Serial Monitor, this can be done in the top
    menus, or by pressing ctrl+shift+m. Observe that the correct time and date
    is being printed in the Serial Monitor every three seconds. If you do not
    observe this, consult the repair manual (Secion XXX).
4.  Plug the SD card into your computer. Open data.txt. If the file contains
    readings, back them up as you usually would. Plug the SD card back into the
    circuit board. Load SDtest and run. Wait about 30 seconds after the ESP has
    booted, then power off the device. Plug the SD card into your computer and
    open data.txt. Make sure that the file contains new data. Discard this
    data, as it is random and does not contain any useful information. Replace
    the SD card into the circuit board. If data was not successfully stored in
    the SD card, consult the repair manual (Section XXX).
5.  Load RotaryEncoderTest and run. Open the Serial Monitor. Observe that
    rotating the rotary encoder causes a new number to appear in the Serial
    Monitor. Observe that pressing the rotary encoder causes a special message
    to appear in the Serial Monitor. If you do not observe these things, 
    consult the repair manual (Section XXX).
6.  bluetoothTest
7.  Connect one or more i2c sensor to the sensor side board. Open I2Cread.
    Change the variable "i2cCount" to the number of i2cSensors you have
    connected. Change the numbers in the array "i2cAddresses" to the addresses
    of each i2cSensor. For Atlas Scientific Dissolved Oxygen sensors, see
    instructions below. Now, run the I2Cread code. Open the serial monitor
    and observe readings being written to the serial monitor. If you do not
    observe readings, consult the repair manual (Section XXX).
8.  Connect one DallasTemperature/OneWire device to the sensor board. Open
    oneWireTest. Change the value of the variable "one_wire" to the name of the
    input which you have plugged in the OneWire device. Run the code. Open the
    serial monitor. Observe readings being written to the serial monitor. If
    you do not observe readings, consult the repair manual (Section XXX).
9.  Connect one analog sensor, or a variable voltage divider, to the sensor
    side board. Open analogTest. Change the value of the variable "input" to
    the input which you have plugged in the analog device. Run the code. Open
    the serial monitor. Observe readings being written to the serial monitor.
    If you do not observe readings, consult the repair manual (Section XXX).
10. Connect one digital sensor to the sensor side board. Open analogTest.
    Change the value of the variable "input" to the input which you have
    plugged in the analog device. Run the code. Open the serial monitor.
    Observe readings being written to the serial monitor. If you do not observe
    readings, consult the repair manual (Section XXX).
11. batteryTest
12. Load send on the sensor side board and run. Unplug that board from your
    computer. Load receive on the relay side board and run. Replace power to
    the sensor side board (ensure both boards are receiving power). Open the
    serial monitor and observe that the word 'hello' is being printed to the
    serial monitor. Repeat this procedure but with send on the relay side board
    and receive on the sensor side board. If you do not observe 'hello' being
    printed to the serial monitor for both of these cases, consult the repair
    manual (Section XXX).
13. Load sendToControl on the sensor side board and run. Unplug that board from
    your computer. Load receiveAndControl on the relay side board and run.
    Replace power on the sensor side board (ensure both boards are receiving
    power). Using the same steps as test 1 with the LED, observe that each
    screw terminal causes the LED to flash. If you do not observe this, consult
    the repair manual (Section XXX).
    
For Atlas Scientific Dissolved Oxygen Sensors, the EZO device can be set to i2c
mode from UART mode using the i2cSetup arduino sketch. The "command" array 
contains the UART command to do this. The penultimate values that look like
"0x3_" where the underscore is a number between 0 and 9 inclusive, are the
individual digits of the i2c address in ascii form. As an example, to set the
address of the EZO device to 42, the array will look like this:
char command[] = {0x49, 0x32, 0x43, 0x2C, 0x34, 0x32, 0x0D};
Notice that 0x34 and 0x32 are the penultimate values. This corresponds to the
address 42. The address can be a number between 0 and 127. Therefore there can
be one to three values that look like "0x3_".