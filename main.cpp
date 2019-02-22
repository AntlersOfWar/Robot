#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHServo.h>

//Starting orientation of robot is going to be the default.
bool defaultOrientation = true;

/* TODO: Make sure you have the right number of motors/encoders declared. */
FEHServo main_servo(FEHServo::Servo0);
FEHServo lever_servo(FEHServo::Servo1);

//P3_6, and P3_7 cannot be used for digital encoders.
//fl means front-left, br means back-right.
DigitalEncoder fl_encoder(FEHIO::P0_0);
DigitalEncoder fr_encoder(FEHIO::P0_1);
DigitalEncoder bl_encoder(FEHIO::P0_3);
DigitalEncoder br_encoder(FEHIO::P0_2);

FEHMotor fl_motor(FEHMotor::Motor0, 5.0);
FEHMotor fr_motor(FEHMotor::Motor1, 5.0);
FEHMotor bl_motor(FEHMotor::Motor3, 5.0);
FEHMotor br_motor(FEHMotor::Motor2, 5.0);

AnalogInputPin cds(FEHIO::P0_4);

//Here 'move_forward' means positive movement.
/* IMPORTANT: Our coordinate system for this program is a top-down view of the course, with the starting point as the origin.
 * DDR is in positive X and lever is in positive Y. */
void move_forward(int percent, int counts)
{
    //Reset all encoder counts
    fl_encoder.ResetCounts();
    fr_encoder.ResetCounts();
    bl_encoder.ResetCounts();
    br_encoder.ResetCounts();

    //Set motors to desired percent. Some motors have to turn backwards, so make percent negative.
    //Some motors have to turn backwards depending on the orientation, also.
    if (defaultOrientation) {
    fl_motor.SetPercent(percent);
    br_motor.SetPercent(-1 * percent);
    fr_motor.SetPercent(-1 * percent);
    bl_motor.SetPercent(percent);
    } else {
        fl_motor.SetPercent(percent);
        br_motor.SetPercent(-1 * percent);
        fr_motor.SetPercent(percent);
        bl_motor.SetPercent(-1 * percent);
    }

    //While the average of the left or right encoders is less than theoretical counts,
    //keep running motors
    while((fl_encoder.Counts() + bl_encoder.Counts()) / 2. < counts ||
          (fr_encoder.Counts() + br_encoder.Counts()) / 2. < counts );

    //Turn off motors
    fr_motor.Stop();
    bl_motor.Stop();
    fl_motor.Stop();
    br_motor.Stop();
}

//Same as move_forward, but percents are multiplied by -1 (reverse).
void move_backward(int percent, int counts)
{
    //Reset all encoder counts
    fl_encoder.ResetCounts();
    fr_encoder.ResetCounts();
    bl_encoder.ResetCounts();
    br_encoder.ResetCounts();

    //Set motors to desired percent. Some motors have to turn backwards, so make percent negative.
    //Some motors have to turn backwards depending on the orientation, also.
    if (defaultOrientation) {
    fl_motor.SetPercent(-1 * percent);
    br_motor.SetPercent(percent);
    fr_motor.SetPercent(percent);
    bl_motor.SetPercent(-1 * percent);
    } else {
        fl_motor.SetPercent(-1 * percent);
        br_motor.SetPercent(percent);
        fr_motor.SetPercent(-1 * percent);
        bl_motor.SetPercent(percent);
    }

    //While the average of the left and right encoders is less than theoretical counts,
    //keep running motors
    while((fl_encoder.Counts() + bl_encoder.Counts()) / 2. < counts ||
          (fr_encoder.Counts() + br_encoder.Counts()) / 2. < counts );

    //Turn off motors
    fr_motor.Stop();
    bl_motor.Stop();
    fl_motor.Stop();
    br_motor.Stop();
}

/* TODO: Make sure to find out what angle the servo turns between orientations. */
//Sets robot's wheel orientation so it can move in the direction of the Y-axis
void setVerticalOrientation() {
    main_servo.SetDegree(0.);
    defaultOrientation = true;
}

//Sets robot's wheel orientation so it can move in the direction of the X-axis
void setHorizontalOrientation() {
    main_servo.SetDegree(90.);
    defaultOrientation = false;
}

//Controls the lever servo's angle.
void pushLever(float degree) {
    lever_servo.SetDegree(degree);
}

/* TODO: Find correct number of counts and determine ideal motor speed. */
//Start function, which will move the robot from the start position and into a correct orientation.
void start() {
    move_forward(20, 50);
    Sleep(250);

    fl_encoder.ResetCounts();
    fr_encoder.ResetCounts();
    bl_encoder.ResetCounts();
    br_encoder.ResetCounts();

    //Turns the robot to the left about the general center point of the robot.
    fr_motor.SetPercent(20);
    bl_motor.SetPercent(-20);
    fl_motor.SetPercent(-20);
    br_motor.SetPercent(20);

    while((fl_encoder.Counts() + bl_encoder.Counts()) / 2. < 140 ||
          (fr_encoder.Counts() + br_encoder.Counts()) / 2. < 140 );

    fr_motor.Stop();
    bl_motor.Stop();
    fl_motor.Stop();
    br_motor.Stop();

}

int main()
{
    /* TODO: These are dummy values, put actual values in. */
    //int motor_percent = 25; //Input power level here
    //int expected_counts = 567; //Input theoretical counts here

    float x, y; //for touch screen

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Touch the screen to start");
    while(!LCD.Touch(&x,&y));
    while(LCD.Touch(&x,&y));

    /*start();
    Sleep(500);
    //Want to make sure it's practically perfectly aligned with Y axis after start()
    //Move forward a little bit past the ramp.
    move_forward(20, 1500);
    Sleep(500);

    setHorizontalOrientation();
    Sleep(500);
*/
    //Move to the right a bit. Line up with the lever.
    //move_forward(20, 800);
        fl_encoder.ResetCounts();
        fr_encoder.ResetCounts();
        bl_encoder.ResetCounts();
        br_encoder.ResetCounts();
    while (true) {
        LCD.Clear();
        LCD.WriteLine(fr_encoder.Counts());
        LCD.WriteLine(fl_encoder.Counts());
        LCD.WriteLine(bl_encoder.Counts());
        LCD.WriteLine(br_encoder.Counts());
    }
    Sleep(2000);
    /*fl_encoder.ResetCounts();
    fr_encoder.ResetCounts();
    bl_encoder.ResetCounts();
    br_encoder.ResetCounts();
    LCD.Write("Initial BRE Counts: ");
    LCD.WriteLine(br_encoder.Counts());
    LCD.Write("Initial BLE Counts: ");
    LCD.WriteLine(bl_encoder.Counts());
    br_motor.SetPercent(-50.0);
    bl_motor.SetPercent(50.0);
    while (br_encoder.Counts() < 200 && bl_encoder.Counts() < 200) {
       LCD.WriteLine(br_encoder.Counts());
    }
    br_motor.Stop();
    bl_motor.Stop();
    Sleep(2000); */

    /*setVerticalOrientation();
    Sleep(500);

    //Move up toward the lever.
    move_forward(20, 1000);
    Sleep(500);

    pushLever(110.);
    Sleep(500); */

    /* TODO: Have code for going back to starting position. Just reverse. */

    LCD.Write("Actual BLE Counts: ");
    LCD.WriteLine(bl_encoder.Counts());
    LCD.Write("Actual BRE Counts: ");
    LCD.WriteLine(br_encoder.Counts());
    LCD.Write("Actual FLE Counts: ");
    LCD.WriteLine(fl_encoder.Counts());
    LCD.Write("Actual FRE Counts: ");
    LCD.WriteLine(fr_encoder.Counts());
}
