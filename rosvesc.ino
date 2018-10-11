/*  Robert Kuramshin
 *
 *  rosserial Subscriber, Publisher + VESC
 *  Commands (Publish std_msgs/Byte to "/vesc"):
 *              (data = 1): Get Motor RPM (subscribe to "/rpm")
 *              (data = 2): Spin at 6000rpm
 *              (data = 3): Brake
 *              
 *  EX: rostopic pub vesc std_msgs/Byte 3 --once
 *      rostopic echo /rpm
 */

#include <ros.h>
#include <std_msgs/Byte.h>
#include <std_msgs/Float32.h>
#include <SamVESC.h>

//-------------------------------------ROS-------------------------------------
ros::NodeHandle nh;
std_msgs::Float32 response;

volatile int8_t message = 0;
volatile bool message_read = true;
int rpm = 0;

void messageCb(const std_msgs::Byte &msg)
{
  message = msg.data;
  message_read = false;
}

ros::Publisher chatter("rpm", &response);
ros::Subscriber<std_msgs::Byte> sub("vesc", &messageCb);

//-------------------------------------VESC------------------------------------
SamVESC vesc1(115200);
struct bldcMeasure vesc_reading;
float speed = 0;
float current = 0;
float voltage = 0;

//-------------------------------------SETUP-----------------------------------
void setup()
{
  //ROS
  nh.initNode();
  nh.advertise(chatter);
  nh.subscribe(sub);

  //VESC
  vesc1.Attach(Serial3);
}

//-------------------------------------LOOP------------------------------------
void loop()
{

  //Check if ROS message available
  if (!message_read)
  {
    message_read = true;
    switch (message)
    {
    case 1: //GET SPEED
      vesc1.StartGetValue();

      if (vesc1.EndGetValue(vesc_reading))
      {
        speed = vesc_reading.rpm;
        current = vesc_reading.avgInputCurrent;
        voltage = vesc_reading.inpVoltage;
      }
      response.data = speed;
      chatter.publish(&response);
      break;
    case 2: //GO FORWARD
      rpm = 6000;
      break;
    case 3: //STOP
      rpm = 0;
      break;
    }
  }

  //Update RPM
  if (rpm == 0)
  {
    vesc1.SetCurrentBrake(3);
    vesc1.SetRPM(0);
  }
  else
  {
    vesc1.SetRPM(rpm);
  }

  nh.spinOnce();
}
