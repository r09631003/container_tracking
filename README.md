# IoT Final Project: Container tracking
Here are some brief explaination of our container tracking system. The detailed structure and method can be seen in the PPT.

# Background
In response to the spread of the COVID-19, the dock workers work in turn to prevent the virus from spreading. This measure leaded to a shortage of manpower at container port. However, the demands for goods have not decreased because of the epidemic. And that is the reason of the port congetion in worldwide which causes the insufficient supply in the market and the surging shipping cost

# Goal
Using the hardwares provided by and the methods taught in NTUEE course "Introduction to the Internet of Things" to build an IoT container tracking system.
The target groups of the system are dock workers and drivers in the freight industry. They can find the specific containers efficiently by using the web GUI in our system and quickly empty the unloading area. As the result, there will be an extra spaces for the next ship to unload. 

# System structure
![image](https://github.com/r09631003/container_tracking/blob/main/system_structure.png)

- The code for GPS Endnode  
GPS_NODE.ino
- The code for Gateway(LoRa Server)  
GPSLoRaServer.ino 

# Reference
GPS code
https://randomnerdtutorials.com/guide-to-neo-6m-gps-module-with-arduino/

GPS Library
https://github.com/mikalhart/TinyGPSPlus

Arduino Lowpower.h Library
https://github.com/rocketscream/Low-Power

LoRa Library(RadioHead)
https://github.com/kenbiba/RH-RF95
