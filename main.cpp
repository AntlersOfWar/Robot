#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>

bool defaultOrientation = true;

/* TODO: Make sure you have the right number of motors/encoders declared. */
FEHServo main_servo(FEHServo::Servo0);
FEHServo lever_servo(FEHServo::Servo1);

//P3_6, and P3_7 cannot be used for digital encoders.
DigitalEncoder fl_encoder(FEHIO::P0_0);
DigitalEncoder fr_encoder(FEHIO::P0_1);
DigitalEncoder bl_encoder(FEHIO::P0_2);
DigitalEncoder br_encoder(FEHIO::P0_3);

FEHMotor fl_motor(FEHMotor::Motor0, 5.0);
FEHMotor fr_motor(FEHMotor::Motor1, 5.0);
FEHMotor bl_motor(FEHMotor::Motor2, 5.0);
FEHMotor br_motor(FEHMotor::Motor3, 5.0);

//Here 'move_forward' means positive movement.
//We're setting (0,0) to be starting point, DDR to be in positive X, and lever to be in positive Y.
void move_forward(int percent, int counts)
{
    //Reset all encoder counts
    fl_encoder.ResetCounts();
    fr_encoder.ResetCounts();
    bl_encoder.ResetCounts();
    br_encoder.ResetCounts();

    //Set motors to desired percent. Some motors have to turn backwards, so make percent negative.
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

    //While the average of the left and right encoders is less than theoretical counts,
    //keep running motors
    while((fl_encoder.Counts() + bl_encoder.Counts()) / 2. < counts || (fr_encoder.Counts() + br_encoder.Counts()) / 2. < counts );

    //Turn off motors
    fr_motor.Stop();
    bl_motor.Stop();
    fl_motor.Stop();
    br_motor.Stop();
}

//Same as move_forward, but percents are multiplied by -1.
void move_backward(int percent, int counts)
{
    //Reset all encoder counts
    fl_encoder.ResetCounts();
    fr_encoder.ResetCounts();
    bl_encoder.ResetCounts();
    br_encoder.ResetCounts();

    //Set motors to desired percent. Some motors have to turn backwards, so make percent negative.
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
    while((fl_encoder.Counts() + bl_encoder.Counts()) / 2. < counts || (fr_encoder.Counts() + br_encoder.Counts()) / 2. < counts );

    //Turn off motors
    fr_motor.Stop();
    bl_motor.Stop();
    fl_motor.Stop();
    br_motor.Stop();
}

/* TODO: Make sure to find out what angle the servo turns between orientation. */
//Starting orientation
void setVerticalOrientation() {
    main_servo.SetDegree(0.);
    defaultOrientation = true;
}

void setHorizontalOrientation() {
    main_servo.SetDegree(90.);
    defaultOrientation = false;
}

void pushLever(float degree) {
    lever_servo.SetDegree(degree);
}

/* TODO: Find correct number of counts and determine ideal motor speed. */
void start() {
    move_forward(20, 50);
    Sleep(250);

    fl_encoder.ResetCounts();
    fr_encoder.ResetCounts();
    bl_encoder.ResetCounts();
    br_encoder.ResetCounts();

    fr_motor.SetPercent(20);
    bl_motor.SetPercent(-20);
    fl_motor.SetPercent(-20);
    br_motor.SetPercent(20);

    while((fl_encoder.Counts() + bl_encoder.Counts()) / 2. < 140 || (fr_encoder.Counts() + br_encoder.Counts()) / 2. < 140 );

    fr_motor.Stop();
    bl_motor.Stop();
    fl_motor.Stop();
    br_motor.Stop();

}

int main()
{
    int motor_percent = 25; //Input power level here
    int expected_counts = 567; //Input theoretical counts here

    float x, y; //for touch screen

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);
    LCD.WriteLine("Touch the screen to start");
    while(!LCD.Touch(&x,&y));
    while(LCD.Touch(&x,&y));

    start();
    Sleep(500);
    //May went to make sure it's practically perfectly aligned with Y axis.
    move_forward(20, 1500);
    Sleep(500);

    setHorizontalOrientation();
    Sleep(500);

    move_forward(20, 100);
    Sleep(500);

    setVerticalOrientation();
    Sleep(500);

    move_forward(20, 1000);
    Sleep(500);

    pushLever(110.);
    Sleep(500);

    /* TODO: Have code for going back to starting position. Just reverse. */


    /*
    LCD.Write("Theoretical Counts: ");
    LCD.WriteLine(expected_counts);
    LCD.Write("Motor Percent: ");
    LCD.WriteLine(motor_percent);
    LCD.Write("Actual LE Counts: ");
    LCD.WriteLine(left_encoder.Counts());
    LCD.Write("Actual RE Counts: ");
    LCD.WriteLine(right_encoder.Counts());
    */
}

