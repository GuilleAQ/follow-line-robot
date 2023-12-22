# Follow Line Arduino-ESP

<div align="center">
<img width=600px src="https://github.com/GuilleAQ/follow-line-robot/blob/main/resources/figures/Screenshot%20from%202023-12-22%2023-20-29.png" alt="explode"></a> 
</div>

<h3 align="center"> Visual Follow Line </h3>

<div align="center">
<img width=100px src="https://img.shields.io/badge/status-finished-brightgreen" alt="explode"></a>
<img width=100px src="https://img.shields.io/badge/license-Apache-orange" alt="explode"></a>
</div>


## Table of Contents
- [Introduction](#Introduction)
- [FollowLine Arduino Code](#FollowLine-Arduino-Code)
- [ESP code](#ESP-code)
- [Operating video](#Operating-video)
- [Usage](#Usage)


## Introduction
### Overview
FollowLine is an Arduino program designed for line-following robots. It integrates FreeRTOS for efficient task management and uses various sensors and actuators. The program includes functionalities for line detection, obstacle detection, and dynamic LED signaling.

There is continuous communication between the Arduino and the ESP, which is then used to send messages from the ESP to an MQTT server.

### Hardware Requirements
- Arduino Board
- Ultrasonic Sensor (Pins: TRIG_PIN, ECHO_PIN)
- Infrared Sensors (Pins: PIN_ITR20001_LEFT, PIN_ITR20001_MIDDLE, PIN_ITR20001_RIGHT)
- Motor Driver (Pins: PIN_Motor_STBY, PIN_Motor_AIN_1, PIN_Motor_PWMA, PIN_Motor_BIN_1, PIN_Motor_PWMB)
- RGB LED (Pin: PIN_RBGLED)
- ESP Board
(ELEGOO Kit de Coche Robótico)

### Software Libraries
- Arduino_FreeRTOS.h
- FastLED.h
- Servo.h
- SoftwareSerial.h
- Adafruit_MQTT.h
- Adafruit_MQTT_Client.h
- WiFi.h

## FollowLine Arduino Code
### Key Features
- Line Following: Uses infrared sensors to detect and follow a line.
- Obstacle Detection: Employs an ultrasonic sensor to detect obstacles.
- LED Feedback: Indicates robot status using an RGB LED.
- PD Controller: Implements a Proportional-Derivative controller for precise movement control.
- Task-based Architecture: Utilizes FreeRTOS for managing concurrent tasks.

### Variable Definitions
- TRIG_PIN, ECHO_PIN: Ultrasonic sensor pins.
- PIN_ITR20001_*: Infrared sensor pins.
- PIN_Motor_*: Motor control pins.
- PIN_RBGLED: RGB LED pin.
- VEL: Base velocity.
- Control messages and PD controller variables.

### Main Functions
- straight, right, left: Movement functions.
- calculatePD: PD controller for line tracking.
- adjustBaseVelocity: Dynamically adjusts base velocity.
- follower: Main line-following logic.
- ultrasonic: Function for ultrasonic sensor readings.
- linePercentage: Calculates and sends line tracking percentage.

### Tasks
- TaskLineFollower: Manages line-following behavior.
- TaskLedBlink: Controls LED based on robot status.
- TaskUltrasonicSensor: Handles ultrasonic sensor readings.

### Setup & Loop
- setup(): Initializes sensors, motors, LED, and tasks.
- loop(): Empty - functionality is handled by FreeRTOS tasks.


### Summary of FollowLine Code for Arduino
- The FollowLine code is designed for line-following robots using Arduino and FreeRTOS. Below is a summary of the main functions and tasks in the code:

#### Functions
- Straight: Moves the robot in a straight line by adjusting the speeds of the left and right motors.
- Right: Turns the robot to the right, stopping the right motor and activating the left motor.
- Left: Turns the robot to the left, stopping the left motor and activating the right motor.
- CalculatePD: Implements the PD (Proportional-Derivative) controller for line tracking, calculating motor speeds based on the values from the infrared sensors.
- AdjustBaseVelocity: Adjusts the base velocity of the robot based on the error calculated by the PD controller.
- Follower: Main function for line following. Reads the infrared sensors and adjusts the robot movement according to the line conditions.
- Ultrasonic: Gets and processes ultrasonic sensor readings for obstacle detection.
- LinePercentage: Calculates and sends the line tracking percentage, based on the infrared sensor readings.

#### Tasks
- TaskLineFollower: Executes the follower function continuously, managing the main line-following behaviour of the robot.
- TaskLedBlink: Controls the RGB LED, changing its colour according to the robot's status (detecting line, encountering obstacle, etc.).
- TaskUltrasonicSensor: Manages the ultrasonic sensor reading, regularly updating the distance to detected obstacles.

#### Additional Functionalities
The code also includes initial settings in the setup function to prepare the pins and communications, as well as an empty loop function, as the FreeRTOS tasks handle the main logic of the program.
Various definitions are used to configure pins, PD controller parameters, and thresholds for line tracking and obstacle detection.
Serial communication is used to send data and receive commands, which is essential for interaction with other devices such as an ESP32.

## ESP code
### Description
The code for the ESP is designed so that when it connects to the MQTT server, it sends a message through serial communication to the Arduino to start. From there, as it receives messages from the Arduino, it will handle them and send them to the server.

### code explanation
I have divided the code in three classes:
#### ESP.ino
First one ESP.ino is the main program, witch will be flashed on ESP board.
In setup, send to Arduino to start its movement, then handles different types of input messages from Arduino (like END_LAP, OBSTACLE_DETECTED, LINE_LOST, etc.).
For each message type, it prepares a corresponding JSON message and sends it to the MQTT broker using the mqtt_manager object.

#### WiFi_config (hpp & cpp)
Encapsulates the WiFi configuration to makes it easier(even more) to use in main program. Only you have to call the constructor depending on your wifi conexion (particular or organization)
The first constructor is for standard WiFi connections, accepting SSID and password.
The second constructor is for enterprise/organization networks, accepting additional parameters like EAP (Extensible Authentication Protocol) credentials.

From this class you can use two public Methods:
- init_WiFi_conexion(): Initializes the WiFi connection.
- print_ip_rssi(): Prints the IP address and RSSI (Received Signal Strength Indicator) of the WiFi connection.

#### MQTT (hpp & cpp)
Encapsulates the MQTT comunication. The constructor accepts a WiFiClient reference, MQTT server address, port number, and base topic for publishing. You can use this constructor with the following public methods:
- connect(): Attempts to connect to the MQTT server.
- send_to_broker(const String&): Publishes a message to the MQTT broker.

#### key highlights
- Flexibility: for WiFi, the class supports both standard and enterprise WiFi networks, making it versatile for different environments.
- Ease of Use: The MQTT class provides a simplified interface for connecting to an MQTT server and publishing messages, abstracting the complexities of MQTT protocol handling.

  ## Usage
- Ensure all hardware components are connected to the specified pins.
- Define a ssid and psswd in WiFi constructor.
- Define an adress and server port to connect and a topic to publish 
- Upload the code to an Arduino board from follow_line.ino and Upload the code from ESP.ino to ESP board.
- The robot will start when ESP indicates it (when connects to server).

## Operating video
https://youtu.be/hJbbl05LIb4
