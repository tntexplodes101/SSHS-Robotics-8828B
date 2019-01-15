#pragma config(Sensor, dgtl5,  green,          sensorLEDtoVCC)
#pragma config(Sensor, dgtl6,  yellow,         sensorLEDtoVCC)
#pragma config(Sensor, dgtl7,  red,            sensorLEDtoVCC)
#pragma config(Motor,  port2,           rightBack,     tmotorVex393_MC29, openLoop, driveRight)
#pragma config(Motor,  port3,           rightFront,    tmotorVex393_MC29, openLoop, driveRight)
#pragma config(Motor,  port4,           leftBack,      tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port5,           leftFront,     tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port7,           liftMotorLeft, tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port8,           liftMotorRight, tmotorVex393_MC29, openLoop, driveRight)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"
#include "8828B_Code_Includes.c"

// Constants defined as multiplier for drive speed when toggle is pressed
#define LOW 0.3
#define HIGH 0.5


void pre_auton()
{

	// Set bStopTasksBetweenModes to false if you want to keep user created tasks
	// running between Autonomous and Driver controlled modes. You will need to
	// manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}



// function defined by user that takes two parameters, left and right. changes value of motor port given in square braces.

task autonomous()
{
	// Remove this function call once you have "real" code.
	AutonomousCodePlaceholderForTesting();
}


task usercontrol()
{
	// holds state of toggle for joystick control.
	bool toggleState;
	// used to prevent rapid flipping of toggleState, set at end of loop
	int prevButtonState;
	//buttonState: set to reference button, prevents value from changing inside loop
	int buttonState;
	while (true)
	{
		SensorValue[yellow] = buttonState = vexRT[Btn8D];

		/* CODE FOR SINGLE JOYSTICK CONTROL W. TOGGLE */
		if (!toggleState)
			move((vexRT[Ch3] * LOW) + (vexRT[Ch4] * LOW), (vexRT[Ch3] * LOW) - (vexRT[Ch4] * LOW));
		else
			move((vexRT[Ch3] * HIGH) + (vexRT[Ch4] * HIGH), (vexRT[Ch3] * HIGH) - (vexRT[Ch4] * HIGH));


		if(vexRT[Btn6D])
			motor[liftMotorLeft] = -50;
		else if(vexRT[Btn6U])
			motor[liftMotorLeft] = 50;
		else
			motor[liftMotorLeft] = 0;


		// reverses state of toggle if button is pressed and wasn't pressed in previous loop.
		if (buttonState && prevButtonState != buttonState)
		{
			SensorValue[red] = toggleState = !toggleState;
		}

		// sets prevButtonState to curr. button state.
		prevButtonState = buttonState;
	}
}
