# Software documentation 
This document aims to present all the software solutions we have developed—whether through experiments, testing, or iterative development—leading up to the final software implementation. It also outlines the logic and methodology we used to analyze and solve the project challenges by breaking them down into their simplest components.

The system relies primarily on the Raspberry Pi 5, which functions as the "brain" of the robot. It is mainly responsible for analyzing and processing images captured by the camera, utilizing Computer Vision (CV) techniques, as well as Machine Learning (ML) and Artificial Intelligence (AI) to enable the robot to make intelligent decisions based on visual and environmental data.

## Sensors and motors communication protocols
The different communication protocols used between the components of the robot and their hierarchy:
- Computer
  - Raspberry Pi `SSH & VNC & micro-HDMI `
    - Ardunio Maga `USB `
      - DC Motor with endcoder `Interrupt`
      - Motor driver `PWM` 2x`Digital`
      - Servo `PWM`
      - 3x VL53L0X Sensor `I2C` + `Digital Output (XSHUT)`
    - Camera `CSI-2`

## Components and Connections Simulation
We designed and modified these parts to match the needs of our WRO project, using Fritzing based on our specific requirements. The design process was guided by our strategy of fully simulating all components, connections, and code before implementation. Therefore, creating these parts was an essential step in our plan to ensure a smooth transition from theoretical design to practical application[Parts](https://github.com/sara-imad06/WRO-Designed-Components.git).

## Visualizer 
## Programming computer & Code editor
This can be either a laptop or a PC. We tested everything on a laptop running Windows 11.We used Visual Studio Code [VScode](https://code.visualstudio.com/download), we used several extinstions like :[Python](https://marketplace.visualstudio.com/items?itemName=ms-python.python), [Ardunio](https://marketplace.visualstudio.com/items?itemName=lintangwisesa.arduino),[Ardunio](https://marketplace.visualstudio.com/items?itemName=moozzyk.Arduino), [Java](https://marketplace.visualstudio.com/items?itemName=SonderMX.java-kits)

## Coding Langueges 
## Arduino Mega
The [Arduino Integrated Development Environment](https://www.arduino.cc/en/software/)  was our choice to develop our first stage (testing code) files were in**file.ino** extinstion. In later stages we moved into VS code.

## Raspberry Pi 5
### Setup
The Raspberry Pi 5 should have Raspberry Pi OS 64 bit downloaded. You will need to install a few Python libraries using the **`pip`** tool. Pip most likely got installed along with Python but just in case it can be downloaded [here](https://pip.pypa.io/en/stable/installation/).
### Installing libraries
- `pip install rpi-lgpio`
  - **GPIO** pin manager library for Raspberry Pi 5
- `pip install serial`
  - **Serial** communication library
- `pip install smbus2`
  - **I2C** communication library
- `pip install rpi-TM1638`
  - [TMBoard](https://thilaire.github.io/missionBoard/TM163x/) panel library
  
## Testing Code (Stage 1)
At this stage, we developed an experimental code based on the basic wiring, with the main goal of testing all wheels, motors, and controllers. This stage was divided into three main parts:

### 1. Adjusting the servo motor angle
During this process, the center angle was set to 90 degrees, with a deviation range of ±10 degrees  
[Code](/src/servo_test_code.java)

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

## Open challenge
## Obstacle challenge
## Framework - functions


