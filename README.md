# CyborgDistro
Read about the project here: https://tandonp.wordpress.com/2014/09/01/cyborg-backpack/

The Cyborg Distro is an open source project that allows anyone to build an AWESOME multi-robot, multi-sensor cyborg. The Cyborg Distro software runs on Raspbian and interfaces with Android phones. Here is a proto Cyborg of the Distro (https://tandonp.wordpress.com/2014/09/01/cyborg-backpack/). This cyborg has multiple robots -- many arms, grippers, etc. Additionally, it supports many sensor modalities such as sonar and camera. C and Java API libraries are provided with the Distro to control different sensors.

--Cyborg Hardware Specs--

-2 Dagu 6DOF Arms

-SSC32 Servo Controller w/ custom enclosure

-Raspberry Pi B w/ portable USB battery and Adafruit Case

-2 Ultrasonic sensors on the sides of the backpack serve to detect obstacles and your phone beeps if you get close to something. This helps you protect the extra arms from damage.

-Tekkeon External Battery for powering the servo controller and rasp pi

--Cyborg Software Specs--

The repository contains four directories.

RealTimeSystem - Python Scripts and Compiled Jars for Running the System. Includes the Python CameraServer and SonarServer for automating these USB sensors. Looking at this folder is the best way to get off the ground and get a running Cyborg system.
See CyborgSys.sh for details for the components that need to be run (and flip out components you may not have + add more).

ServoControlServer - Server that runs on Raspberry Pi to control Servo Motors. Interacts with Android App over Sockets to allow user to send commands to the robot for their phone.

CyborgApp - Android App Project that allows a user to control the Cyborg. The Android App supports real-time visualization of the incoming sensor data. It allows the user to control and send commands to the servo motors to move the arms.

Sensor API - C APIs for various popular off the shelf sensors. This is derived from my blog post (https://tandonp.wordpress.com/2014/10/11/python-sensor-libraries/). You can easily use Python CTypes to automate obtaining data from the sensors in high level python. In fact, the entire higher level logic of the system is written in Python and Java at the end of the day, but we can also control the low level polling details of sensors and actuators in C if we need to.






