#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>

FEHMotor mymotor(FEHMotor::Motor0, 5.0);

void main()
{
    while(true){
          mymotor.SetPercent(10);
    }
}
