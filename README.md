# embedded-systems worksheets explained

# MUST ADD IMAGES NEXT!!

This repository contains the work folders for this project. Here is a brief explanation of what I did for each worksheet. I did this setting up my own VM of Ubuntu linux to make things easier ! (I used AI a few times through the worksheets, but mostly to fix bugs I couldn't seem figure out myself. ex. often putting small chunks of code in main.c which was wrong and confusing the system.)

Worksheet 2

We used the "arm-none-eabi" toolchain to compile and link code for the Cortex-M target.
Then built ELF binaries and compared them with x86 binaries.
Inspected object files and executables using "file" and "objdump".
Made minor changes to the Makefile to work on my VM.

Worksheet 3

We added a separate LED driver "led.c" and "led.h" to avoid putting the GPIO code in "main.c" (hardware code should be separated from application logic code).
Added some functions to control the LED (ex. on, off, toggle).
Added support for a second LED to show code reuse.
Tried to implement a simple button input using GPIO and pull up resistors (pull up resistors supposedly prevent floating inputs).

Worksheet 4

Firstly added a UART driver module similar to adding the LED driver previously "com.c" "com.h"
Needed help changing USART1 for serial communication.
Changed the orgiinal "printf()" and "getchar()" lines to use the UART driver.
Verified output by repeatedly printing messages over the serial port.
Then added a minimal "syscalls.c" file to support libc functions like "printf".
Needed help implementing required system calls ""_write", "_read", "_sbrk" so newlib can work without an operating system (learned later because it is a "bare-metal" system).
Demonstrated the operations by repeatedly printing "Hello world" using "printf".

Next I did attempt implementing the further Credit tasks but struggled to get things working smoothly. Part of the Quiz was added, but I was having some problems with newlib (I got it to work initially then broke it and couldn't fix it again.).