# Software documentation 
This document aims to present all the software solutions we have developed—whether through experiments, testing, or iterative development—leading up to the final software implementation. It also outlines the logic and methodology we used to analyze and solve the project challenges by breaking them down into their simplest components.

The system relies primarily on the Raspberry Pi 5, which functions as the "brain" of the robot. It is mainly responsible for analyzing and processing images captured by the camera, utilizing Computer Vision (CV) techniques, as well as Machine Learning (ML) and Artificial Intelligence (AI) to enable the robot to make intelligent decisions based on visual and environmental data.

In parallel, the system includes an Arduino Mega, which serves as the main controller for all motor operations and directional control. It executes movement commands received from the Raspberry Pi and directly manages the DC motors, servos, and motor driver modules to ensure accurate and responsive navigation.


## Components and Connections Simulation
We designed and modified these parts to match the needs of our WRO project, using Fritzing based on our specific requirements. The design process was guided by our strategy of fully simulating all components, connections, and code before implementation. Therefore, creating these parts was an essential step in our plan to ensure a smooth transition from theoretical design to practical application[Parts](https://github.com/sara-imad06/WRO-Designed-Components.git),these parts were created as part of the software work to support the hardware work and make it easeir, and all components and their uses can be found in the Hardware Documentation.

## Visualizer 

## Programming computer & Code editor
This can be either a laptop or a PC. We tested everything on a laptop running Windows 11.We used Visual Studio Code [VScode](https://code.visualstudio.com/download), we used several extinstions like :[Python](https://marketplace.visualstudio.com/items?itemName=ms-python.python), [Ardunio](https://marketplace.visualstudio.com/items?itemName=lintangwisesa.arduino),[Ardunio](https://marketplace.visualstudio.com/items?itemName=moozzyk.Arduino), [Java](https://marketplace.visualstudio.com/items?itemName=SonderMX.java-kits)

You need to install Python on your device with a version that is compatible with your system. We also installed some specific Python libraries:
`pip install pyserial`<br>
`pip install numpy matplotlib`<br>




## Coding Langueges 
At the beginning of the development process, we used Java during the testing phase to experiment with and verify individual code segments. Once we confirmed that each part was working correctly, we assembled the [full code]() in C++, incorporating some C-based syntax and functions, as it is faster and easier to work with when controlling the Arduino and its motors. 

For the Raspberry Pi 5, the code was written in Python, which is the most suitable and widely supported language for working with cameras and communicating with the Pi. To enable communication between the Raspberry Pi (running Python) and the Arduino (running C++), we implemented a [communication code]() over a USB serial connection.


## Arduino Mega
The [Arduino Integrated Development Environment](https://www.arduino.cc/en/software/)  was our choice to develop our first stage (testing code) files were in**file.ino** extinstion. In later stages we moved into VS code.

## Raspberry Pi 5
## Testing Code 
At this stage, we developed an experimental code based on the basic wiring, with the main goal of testing all wheels, motors, and controllers. This stage was divided into three main parts:

### 1. Adjusting the servo motor angle
During this process, the center angle was set to 90 degrees, with a deviation range of ±10 degrees  
[Code](/src/servo_test_code.java)
``` Java
int centerAngle = 90;
int maxDeviation = 10; 

 void turn(int degreesFromCenter) {
    degreesFromCenter = constrain(degreesFromCenter, -maxDeviation, maxDeviation);
    servo.write(centerAngle + degreesFromCenter);
```

#### Classes  
`DCMotor`  
To control the motor speed and direction.

`Steering`  
To control the servo angle with limits.  
`Steering` is currently empty — to add automatic or manual control later.

---

### 2. Working on it simultaneously with the mechanical aspect
The DC motor runs continuously forward, while the servo automatically turns every 5 seconds in a loop: Left → Center → Right → repeat.  
[Code](/src/test.java)

#### Functions  
`setup()`  
Initializes motor and servo pins, starts forward movement, centers the servo, and begins Serial communication.

`loop()`  
Repeatedly changes the steering direction every 5 seconds and prints the current state to the Serial Monitor with center angle = 90, Max to Right = 125, Max to Left = 55.


### 3.
### 4. 


## Open challenge
## Obstacle challenge
## Framework - functions


