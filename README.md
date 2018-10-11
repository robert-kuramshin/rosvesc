Simple ROS + VESC testing code for Arduino. 
  
VESC library used:  
https://github.com/SnowmanTackler/SamVESC  
  
Make sure to install rosserial first:  
http://wiki.ros.org/rosserial

To Run:  
1. Upload code to Arduno  
2. Start roscore in a terminal with `roscore`  
3. Start a serial node in a new terminal with `rosrun rosserial_python serial_node.py /dev/ttyUSB0` (replace ttyUSB0 if needed)  
4. Follow instructions in rosvesc.ino for usage  
