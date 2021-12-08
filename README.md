# Programming the Arduino Uno in C
This repository provides a framework in  Standard C which mirrors that of the Arduino framework. This allows a student to program the ATmega328P using C in a relatively familar (Arduino) context. The value of programming the ATmega328P in C is that it is easier to understand some of the C concepts using an 8-bit processor as compared to programming in C on a PC. It also allows someone to learn how to program an embedded microcontroller in an easier environment as compared to the Raspberry Pi Pico (32-bit microcontroller).

In order to use this framework, one must install the avr-gcc tool chain appropriate for their platform (Linux, macOS, or Windows). The directions to do so is [here](https://wellys.com/posts/avr_c_setup/).
## Arduino Framework  and standard C Replacement Routines
Much of the C Standard Library is provided by [AVR Libc](https://www.nongnu.org/avr-libc/). I recommend having a link to the online manual open while developing code. The code in this repository is the code required to program the Uno using similar routines as in the Arduino Framework.

### Arduino Framework Functions

**Each function used requires an #include in order to be used (example):**
```C
#include "functionname.h" /* format of include */

#include "analogRead.h" /* for example to use analogRead() */
#include "unolib.h" /* add this file for general definitions */
```
This keeps the code smaller than with a large file containing all of the functions available.

* **analogRead(pin)**: read one of the 6 Analog pins (A0-A5). Returns a 10-bit value in reference to AREF see [analogReference()](https://www.arduino.cc/reference/en/language/functions/analog-io/analogreference/). In this case, it only DEFAULT value of VCC or 5V. To convert reading to voltage, multiply by 0.0048.
* **analogWrite(pin, n)**: setup the Timer/Counters to provide a PWM signal.
	* pin = Arduino UNO Pin Number, must have a "\~" in its name (3, 5, 6, 9, 10, 11)
	* n = n/255 Duty Cycle, i.e; n=127, 127/255 \~= 50% duty cycle
	* Pin PWM Frequencies
		* UNO pin 3/PD3, 488.3Hz
		* UNO pin 5/PD5, 976.6Hz
		* UNO pin 6/PD6, 976.6Hz
		* UNO pin 9/PB1, 976.6Hz
		* UNO pin 10/PB2, 976.6Hz
		* UNO pin 11/PB3, 488.3Hz
* **digitalRead(pin)**: returns value (1 or 0) of Uno pin (pins 0-13 only). If using serial I/O (printf/puts/getchar) then Uno pins 0 and 1 are not usable. Is not configured to use A0-A5.
* **digitalWrite(pin, level)**: set an UNO pin to HIGH, LOW or TOG (pins 0-13 only).  If using serial I/O (printf/puts/getchar) then Uno pins 0 and 1 are not usable. This version also adds TOG, which toggles the level. Much easier than checking the level and setting it to be the opposite level and requires less code. digitalWrite() is not written to use A0-A5.
* **pinMode(pin, mode)**: define INPUT, OUTPUT, INPUT_PULLUP for an UNO pin (pins 0-13 only). Is not configured to use A0-A5.
* **delay(ms)**: Blocking delay uses Standard C built-in \_delay_ms, however allows for a variable to be used as an argument. 
* **millis()**: Returns a long int containing the current millisecond tick count. Review the millis example to understand how to use it. millis() uses a SCALAR1 value to determine the clock rate. Change the value of SCALAR1 in the Library/sysclock.h file to change the period of the clock (default value of SCALAR01_8 for a 1millisecond clock. **IF YOU DO CHANGE THE VALUE OF THE SCALAR1**, you will need to run *make LIB_clean* to clean the Library folder and force it to recompile the functions.
### Standard C I/O functions adapted for the ATmega328P
Use these standard C I/O functions instead of the Arduino Serial class. See example *serialio* for an example implementation. Requires the following in the file:
```C
# in the include section at the top of the file
#include "uart.h"
#include <stdio.h>

# at the top of the main function, prior to using I/O functions
	init_serial;
```
* **getChar(char)**: same as C getChar (non-interrupt at this time)
* **printf(string, variables)**: same as C printf(), limited functionality to be documented. There are two ways to add printf and those are documented in the Makefile in the examples. It is also helpful to review the [avr-libc printf](https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html) documentation.
* **puts(string)**: same as C puts()

### Added functions beyond Arduino Framework
* **buttons[i]** - provides a debounced button response. Each button must attach to an Uno pin
	* Requires sysclock to have a *SCALAR1 = SCALAR01_64*, as this provides a 8 millis pulse to run the button check function
	* *buttons[i].uno* are the Uno pins attached to a button and like digitalRead, function will translate Uno pin to port/pin
	* *buttons[i].pressed* indicates if the button has been pressed (true or non-zero)
	* depending on the application, you might need to set *buttons[i].pressed* to zero, following a successful press, if you depend on a second press to change state. Otherwise, you'll have a race condition where one press is counted as two presses (its not a bounce, its a fast read in a state machine)

	See example in *button* folder as to how to use

## Examples 
### analogRead: 
Demo file for using analogRead(), requires a pot to be setup with outerpins to GND and 5V. Then connect center pin to one of A0-A5, adjust pot to see value chance in a serial monitor.

### analogWrite: 
Demo file for using analogWrite(), requires a scope (Labrador used) to see the output of the PWM signal

### button: 
Demo file for using debounced buttons, requires a button attached to a Uno digital pin with INPUT_PULLUP. *buttons[i].pressed* provides a indication of the button pressed.

### blink: 
Minimal blink sketch. Intended as a minimal test program while working on code, it doesn't use the AVR_C Library.

### digitalRead: 
Uses loops to go through each digital pin (2-13) and print out level on pin. Uses INPUT_PULLUP, so pin needs to be grounded to show 0, otherwise it will be a 1. 

### durationTest:
An inline test of playing a melody using tone(). This version is easier to test and debug than melody.

### melody: 
Fundamentally, the same as the melody sketch on the Arduino website. The changes made are those required for standard C vs. the Arduino framework.

### millis:
Shows an example of using millis() to demonstrate the effectiveness of the delay command. Prints the time delta based on using a delay(1000).

### serialio:
Simple character I/O test using the UART. The USB cable is the only cable required. See note in main.c, as program won't work with specific combinations of a board and serial monitor. Adafruit Metro 328 and minicom for example.

### simple:
Demo file from avr-gcc on-line User Manual [Simple Project](https://www.nongnu.org/avr-libc/user-manual/group__demo__project.html), edited specific to ATmega328P. It is well-worth reviewing as it shows how to use an interrupt. The best way to understand it, is to use a scope (Labrador) to view the waveform change.

## Makefile
The examples make use of a great Makefile courtesy of Elliot William's in his book [Make: AVR Programming](https://www.oreilly.com/library/view/make-avr-programming/9781449356484/). I highly recommend the book and used it extensively to understand how to program the ATmega328P (Arduino UNO) from scratch.

[Makefile](https://github.com/hexagon5un/AVR-Programming/blob/ad2512ee6799e75e25e70043e8dcc8122cb4f5ab/setupProject/Makefile)

I also added a line at the beginning of the Makefile for an environment variable called AVR_PORT. If you add:
```bash
export AVR_PORT=/dev/ttyACM0 # replace this port name with the one you are using
```
in your .bashrc or .zshrc file, the Makefile will pick this for serial communications with the Uno. (be sure to *source* or restart after editing the rc file)

Specific lines to be aware of:
```bash
15 LIBDIR = ../../Library
# Assumes using the structure of the git folder, 
# meaning the examples are two layers down from the Library. Adjust accordingly.
25 PROGRAMMER_ARGS = -F -V -P /dev/ttyACM0 -b 115200	
# Assumes the Uno is plugged into a specific USB port, 
# the easiest way to determine the correct one is to use the Arduino IDE 
# and check the Port (Tools -> Port)
```
### Make Commands for Examples
```
# simple command to check syntax, similar to Verify in the Arduino IDE
make
# command to compile/link/load, similar to Upload in the Arduino IDE
make flash
# command to show the size of the code
make size
# command to clear out all the cruft created in compiling/linking/loading
make all_clean
# command to clear out the Library object files *file.o*, sometimes required if changes to Library files aren't appearing to work, uses LIBDIR folder as the folder to clean
make LIB_clean
```

To [install the proper toolchain](https://wellys.com/posts/avr_c_setup/) required to compile the code.

## Serial Solutions
#### In use
* [Simple Serial Communications with AVR libc](https://appelsiini.net/2011/simple-usart-with-avr-libc/) Works well, integrated into avr-gcc to enable using printf, puts, and getchar. Uses polling which is slow and blocking.
#### Reviewing to determine how to use due to its interrupts
* [Peter Fleury AVR Software](http://www.peterfleury.epizy.com/avr-software.html) Works, not integrated into avr-gcc library, so not native. It uses interrupts and buffering so it is fast and non-blocking.

## Multitasking
There are four multitasking examples in the *examples* folder. Only one of them will be incorporated into the Library. The goal of each example is to explore the possible approaches for multitasking. 
* **multifunction** Based on *oneline*, this version which will ultimately be integrated into the AVR_C Library. I will continue to evolve *multifunction* as I have several specific projects which require a particular version of multitasking.
* **oneline** [A Multitasking Kernal in One Line of code](https://www.embedded.com/a-multitasking-kernel-in-one-line-of-code-almost/) The simplest example of round robin multitasking. Only recommended as an simple illustration as to how to multitask using pointers to functions. Highest speed, smallest footprint 466 bytes, minimal scheduling.
* **RR_Scheduler** [AVR Scheduler](https://sites.google.com/site/avrtutorials2/scheduler) This code is very good for understanding the intricacies of multitasking such as scheduling, prioritization and dispatch, I don't see the need for this capabilities at this time. 958 bytes, structured scheduling and solid approach to scheduling.
* **RIOS** [Preemptive Multitasking for the AVR](http://www.cs.ucr.edu/~vahid/rios/rios_avr.htm) My issue with RIOS is that it asks the ISR to be the scheduler, which seems like a lot of code for the ISR to perform. 1368 bytes, uses ISR as the scheduler, and as more options as to scheduling using time slices.

## Possible ISR Conflicts
When you wish to use the Timer/Counters, you might want to use a Timer/Counter ISR which is already defined. If you get an error such as:
```
...multiple definition of `__vector_16'...main.c:148: first defined here
```
The error is identifying a previously defined ISR vector has a conflict. The vector in question from line 148 of main.c is *TIMER0_OVF_vect* which is also defined in tone.c. Two solutions:
1. If you are using the function tone, you must select a different Timer/Counter to use.
2. If you are not using the function tone, you may comment out the specific lines defining the vector (in this case lines 490-493) and define the vector in your code.
3. Use the same timer/counter, however a different vector which provides the timing you need. For instance, if the timing doesn't need to change, consider using TIMER0_COMPA_vect, which fires using a comparison to OCRA. I'll add more detail when I have a well-defined solution.

The currently defined ISR vectors are:
TIMER0_OVF_vect - tone.c line 490
TIMER1_COMPA_vect - sysclock.c line 7
## Sources
I also write about C, MicroPython and Forth programming on microcontrollers at [Wellys](https://wellys.com).

Other sources of information which were helpful:
* [AVR Libc](https://www.nongnu.org/avr-libc/)
* [Arduino in C | Freedom Embedded](https://balau82.wordpress.com/arduino-in-c/)
* [Programming Arduino in "Pure C"](http://audiodiwhy.blogspot.com/2019/01/programming-arduino-in-pure-c-now-were.html)
* [EMBEDDS: AVR Tutorials](https://embedds.com/avr-tutorials/)
* [CCRMA: AVR](https://ccrma.stanford.edu/wiki/AVR#AVR_Microcontrollers)
* [Efundies: AVR](https://efundies.com/category/avr/)
