/* Don't apply external power to motor shield 
 * while connecting Arduino to computer's USB Socket.
 * It will damage Arduino and motor shield, both.
*/
#include <AFMotor.h>
#include <Servo.h>

/**** global declaration ****/
AF_DCMotor LM(4); // Left dc motor connected to M4 socket
AF_DCMotor RM(3); // Right dc motor connected to M3 socket
Servo i; // servo object
int trigger = 8; // for US sensor
int echo = 9; // for US sensor
float time = 0; // for US sensor
float distance = 0; // for US sensor
int angle = 0; // for servo

void setup() 
{
  // attach servo motor
  i.attach(10); // internally Servo motor-1 is connected to pin-10 of Arduino
  
  //Set initial speed of the motor & stop
  LM.setSpeed(100);
  RM.setSpeed(100);
  LM.run(RELEASE);
  RM.run(RELEASE);
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);

  // control servo initially
  i.write(0);

  // check servo sweeping from 0-180 and back
  for(angle=0;angle<180;angle++)
  {
    i.write(angle);
    delay(50);
  }
  delay(2000);
  
  for(angle=180;angle>=0;angle--)
  {
    i.write(angle);
    delay(50);
  }
  delay(2000);
  // servo motion test complete
}

void loop() 
{
  digitalWrite(trigger,LOW);
  delayMicroseconds(1);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(1);
  digitalWrite(trigger,LOW);
  delayMicroseconds(1);

  // echo received
  time=pulseIn(echo,HIGH);

  // formula to calculate the distance
  // distance is calculated in cm
  distance=((time/2)/29.155);   

/***** Control Robot Trolley *****/

if(distance > 10)
{
  // Go Forward
  LM.run(FORWARD);
  RM.run(FORWARD);
  // sweep servo constantly
  for(angle=0;angle<180;angle++)
  {
    i.write(angle);
    delay(50);
  }
  delay(100);
  
  for(angle=180;angle>=0;angle--)
  {
    i.write(angle);
    delay(100);
  }
  delay(100);

  i.read();
}

if(distance <= 12)
{
  // Stop servo at the position where obstacle detected
  i.write(i.read());

  // Stop trolley
  LM.run(RELEASE);
  RM.run(RELEASE);
  delay(1000);

  // Trolley goes backward
  LM.run(BACKWARD);
  RM.run(BACKWARD);
  delay(1000);

  // Stop trolley
  LM.run(RELEASE);
  RM.run(RELEASE);
  delay(1000);

  // if obstacle is on left turn right
  if(i.read()<90)
  {  
    // Turn trolley right
    LM.run(FORWARD);
    RM.run(RELEASE);
    delay(500);
  }

  // if obstacle is on right turn left
  if(i.read()>90)
  {  
    // Turn trolley left
    LM.run(RELEASE);
    RM.run(FORWARD);
    delay(500);
  }
} // "if" closed
} // loop closed

/***** Extra code to accelerate and decelerate motors automatically *****/
/* Below given coede is suggested for more additions in your project */
/* Remove "/*" and "*/" while using the following code
  
/*
  // Acceleration from 0 to max
  for(i=0;i<=255;i++)
  {
    LM.setSpeed(i);
    RM.setSpeed(i);
    delay(10);
  )
  motor.run(RELEASE);
  delay(1000);
      
  // Decelerate from max to 0
  for(i=255; i!=0; i--) 
  {
    LM.setSpeed(i);
    RM.setSpeed(i);
    delay(10);
  }
  motor.run(RELEASE);
  delay(1000);

*/
