# Hardware documentation 
The robot consists of a wide variety of sensors, motors, and two processing units: a Raspberry Pi 5 and an Arduino Mega. The base is built on a 4WD RC Smart Car Robot Chassis with an MG996R metal servo and bearing kit, which has been heavily modified to suit our project's requirements. These modifications include custom 3D-printed parts, such as a dedicated camera mount and custom spacers, as well as the replacement of several original components to improve stability, wiring, and sensor integration. All engineering modifications are thoroughly documented and clearly illustrated in the guide provided below.



## List of components
We installed a lot of sensors and other components on the car, here is a list of each of them with a link and some additional notes:
- **Base car** [4WD RC Smart Car Robot Chassis](https://technolab.ps/detail/471)
  - originally 248 x 146 mm, 960 g 695.7 g (including DC and servo motor)
- **Steering servo** [MG996R Metal Servo Bearing Kit](https://technolab.ps/detail/471)
- **DC motor** [DC Gear Motor with Encoder](https://roboticx.ps/product/dc-gear-motor-with-encoder/)
  -  Operating voltage 6V, torque 10 kg*cm max power 3.2A, 210 RPM, 341.2 pulse-per-revolution encoder
- **Single Board Computer** [Raspberry Pi 5 8GB](https://roboticx.ps/product/raspberry-pi-5/?asp_highlight=raspberry&p_asid=3)
  - Operating voltage 5V/5A USB-C, 4 USB ports, 40 pins
- **Microcontroller Arduino Mega 2560** [arduino](https://roboticx.ps/product/arduino-mega-2560-r3-china/)
  - Operating voltage 7-12V, 54 Digital I/O Pins (14 PWM outputs)- 16 Analog Inputs, dimensions: 101.5 x 53.3 mm
- **Motor driver** [L298N](https://roboticx.ps/product/dual-motor-controller-module-l298n/)
  - Max Drive current 2A, control voltage 4.5~5.5V
- **Battery**
  - Bundled with car, 4.2V, 5000mAh,Lithium, [Lithium Ion Battery](https://roboticx.ps/product/lithium-ion-battery-18650-cell-5000mah/)
- **Power switch** [SPST](https://roboticx.ps/product/rocker-switch-on-off-spst/)
- **LiDAR sensor** [TF-Luna](https://roboticx.ps/product/tf-luna-tof-micro-single-point-ranging-lidar/)
  - Dimensions 35 x 21.5  mm, range 0.2-8 m, frequency 100 Hz, accuracy ±2% ,FOV 2°
-  **Laser sensor x2** [VL53L0XV2 Laser](https://roboticx.ps/product/vl53l0xv2-laser-ranging-sensor-time-of-flight-tof/)
  - Dimensions 25 x 10 x 4 mm, 50mm – 1.2m range (default mode), 50mm – 2.2m range (long range mode), frequency 5-33 Hz, accuracy ±2cm ,3.3V
- **Camera** [PixyCam 2.1](https://tribotix.com/product/pixycam2-1/)
  - Built-in color detection algorithm, field of View: 80° horizontal 40° vertical, integrated lights, up to 60 fps

## 3D printed parts stages (designing & printing)

The design and 3D printing of the parts went through four main stages based on our mechanical strategy:

#### 1. Stage one: Printing parts to verify the robot’s ability to move in a straight linear path without deviation.
  
  In this stage, we designed two planned parts (Front pen holder & Rear pen holder) to hold two pens, aiming to verify the robot’s correct movement and to detect any deviation in the servo motor’s motion in order to program it accurately. The first part is fixed at the front of the robot, and the second at its end, precisely at the center, to measure the accuracy of the path and the amount of deviation.
We 3D printed the parts using PLC material, and we drilled and adjusted them ourselves in the engineering workshops at the university.
 
  ![pen holders ](/models/stage_1.png) 
  


#### 2. Stage two: Designing a movable battery holder (slider)
 
  ![Slider ](/models/Slider.jpg) 
  
#### Making the models and Exporting the G-code
  We used *Autodesk Fusion 360* for designing our parts, however any industrial software like *Autodesk Inventor* can be used as well. In the modelling process there are a couple of things to remember:
  
  - Make sure that any measurement given in a documentation seems realistic, **since we have seen mistakes even in official documentations**.
  - Always double check any calculation and check the dimensions relative to multiple sides, because **it may look alright from one side, but may be completely unusable from another**.
  - Use your software's built in physics simulators to check for weak points and **always make sure it can handle the stress it may be placed under**.
  - When done export the file as an **.stl** so that a slicer can use it.
  
  
  ![Example of a sketch in Fusion 360](/models/fusion_sketch.png)
  




## Assembly
First we started from the base car kit, which is the HiWonder Ackermann Intelligent Car. 







## Conclusion
We encountered several challenges during the assembly and planning process, but we are very proud of the work we have accomplished. If you have any questions or encounter any issues, please don't hesitate to contact us at ramanajjar25@gmail.com(Rama) or ahmadabubaker199@gmail.com(Ahmad) through any preferred method.
