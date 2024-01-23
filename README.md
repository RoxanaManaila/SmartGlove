# SmartGlove
Hello there!

First, I want to start by saying that this project is one of my most interesting and beautiful projects that I ever worked on and it is very special to me.
For anyone reading this, you need to know how it all started and what this is all about in order to fully understand the project.

So here it goes:
The project itself represents a device in form of a smart glove, consisting of both hardware and software. The main focus of the device is helping people with deaf-mute disabilities communicate 
more easily and in a natural way with the people that do not know sign language. It can also have another functionality, for example, it can be used to study sign language and understand it better 
and easier, being of much help to those who want to learn sign language at home.
It should be noted that the prototipe only recognises the letters of the sign language alphabet. 
In order for all of this to work and make sense, I also created and android app that receives the information from the arduino's microcontroller via bluetooth communication and displays the translation on the phone screen
making it possible to write words and even phrases just by signing the letters. I also created and added two auxdiliar gestures for introducing the space between words and also, for allowing the possibility to delete letters
that have already been signed and recognised, making it easier to manage possible mistakes.
In the making of the source code for the recognition, there was an experimental process that I used in order to obtain the information from the sensors that allowed me to discover the proper values that I needed in order for the 
translation to actually be as precise as possible and work.

Hardware components:
-basic fabric glove
-5x flex sensors (one for each finger)
-1x MPU6050 module (I2C communication) - this was placed on the back of the palm
-1x Arduino Nano with Atmega328P microcontroller
-1x bluetooth module HC05 (serial communication)
-5x 10kOhm resistance 
-wires
-battery

The repository contains:
-the sourse code for the hardware
-the source code for the android app

NOTE. The glove was designed for the right hand.
