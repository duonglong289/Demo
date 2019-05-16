**1. Install C library on the Raspberry Pi:**

Step 1: Check that wiringPi is not already installed. In a terminal, run:

	$ gpio -v

If you get something, then you have it already installed. If not, carry on:

Bước 2: To obtain WiringPi using GIT :

	$ git clone git://git.drogon.net/wiringPi

Bước 3: If you have already used the clone operation for the first time, then :

	$ cd wiringPi

	$ ./build

The new build script will compile and install it all for you – it does use the sudo command at one point, so you may wish to inspect the script before running it. 

**2. Communication between Dynamixel and Raspberry Pi:**

Unlike other servos, the Dynamixel does not respond to PWM signals, but a slightly more complicated protocol of instructions for reading and writing onto its memory. This communication happens over a half-duplex UART port, using only one wire for both sending and receiving.
What this means is that we need to build a small circuit that converts full-duplex into half-duplex.

- Using IC 74HC126 and 74HC125 (or 74HC04)

<img src="https://i.imgur.com/7RktWcv.jpg">

