# arduino-projects

## Moisture Sensor LCD Display
===========================

This code is for reading the moisture level using a moisture sensor and displaying it on a 16x2 LCD screen using an I2C interface. The code uses the LiquidCrystal\_I2C library to interface with the LCD screen.

## Setup
-----

*   Connect the moisture sensor to analog pin A0 and power it using pin 6.
*   Connect the I2C interface of the LCD screen to the appropriate pins on the microcontroller.
*   Upload the code to the microcontroller.

## Usage
-----

Once the code is uploaded to the microcontroller, the moisture level will be displayed on the LCD screen. The screen will refresh every 2 seconds. The moisture level is displayed as a percentage value.

## Code Explanation
----------------

The `LiquidCrystal_I2C` library is used to interface with the LCD screen. The library is initialized with the address of the screen and the number of columns and rows.

The `setup()` function initializes the LCD screen, sets the power pin as an output, and starts the serial communication.

The `loop()` function clears the LCD screen, reads the moisture level using the `bacaSensor()` function, and displays it on the LCD screen. The `bacaSensor()` function turns on the power pin, reads the analog value from the sensor, turns off the power pin, and returns the moisture level as a percentage.

The `delay()` function is used to ensure that the moisture level is read every 2 seconds.
