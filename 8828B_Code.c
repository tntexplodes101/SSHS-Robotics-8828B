#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    modeSelect,     sensorPotentiometer)
#pragma config(Sensor, dgtl1,  liftEnc,        sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  lowerLimit,     sensorTouch)
#pragma config(Sensor, dgtl5,  green,          sensorLEDtoVCC)
#pragma config(Sensor, dgtl6,  yellow,         sensorLEDtoVCC)
#pragma config(Sensor, dgtl7,  red,            sensorLEDtoVCC)
#pragma config(Sensor, dgtl8,  solenoid,       sensorDigitalOut)
#pragma config(Sensor, dgtl12, currentDriver,  sensorTouch)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port2,           rightBack,     tmotorVex393_MC29, openLoop, reversed, driveRight, encoderPort, I2C_1)
#pragma config(Motor,  port3,           rightFront,    tmotorVex393_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port4,           leftBack,      tmotorVex393_MC29, openLoop, driveLeft, encoderPort, I2C_2)
#pragma config(Motor,  port5,           leftFront,     tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port7,           liftMotorTop,  tmotorVex393_MC29, openLoop, reversed, driveLeft)
#pragma config(Motor,  port8,           liftMotorBottom, tmotorVex393_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port9,           claw,          tmotorVex393_MC29, openLoop, reversed)
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
#define LOW 0.35
#define HIGH 0.7
// max height of lift in counts, relate to degrees.
#define MAXHEIGHT 675
// these constants are used to multiply the turning speeds in autonomous for testing,
// code is tested with red as the baseline but works with blue as well.
#define BLUEALLI -1
#define REDALLI 1



void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks
	// running between Autonomous and Driver controlled modes. You will need to
	// manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	SensorValue[green] = SensorValue[currentDriver];

}

/*
AUTONOMOUS
==========================
drives forward,
turns towards the platform,
drives forward onto it
==========================
*/


task autonomous()
{
	// integer used to invert effect of turning in program
	//so same code segment can be used for opposite side of arena.

	int teamMult = 0;
	int startPoint;
	// 0 to break point for r2
	if(SensorValue[modeSelect] < 344)
	{

		startPoint = 0;
		teamMult = 0;
	}
	// r2
	else if(SensorValue[modeSelect] < 777)
	{
		writeDebugStreamLine("r2");
		teamMult = REDALLI;
		startPoint = 2;
	}
	// r1
	else if(SensorValue[modeSelect] < 1230)
	{
		writeDebugStreamLine("r1");
		teamMult = REDALLI;
		startPoint = 1;
	}
	// b1
	else if (SensorValue[modeSelect] < 1675)
	{
		writeDebugStreamLine("b1");
		teamMult = 0;
	}

	// b2
	else if (SensorValue[modeSelect] < 2200)
	{
		writeDebugStreamLine("b2");
		teamMult = BLUEALLI;
		startPoint = 1;
	}
	else if (SensorValue[modeSelect] < 2850)
	{
		teamMult = BLUEALLI;
		startPoint = 2;
	}

	/* START OF DRIVE CODE */
	switch (startPoint)
	{
	case 1:
		moveDistance(1850);
		wait1Msec(200);
		turn(-90 * teamMult);
		wait1Msec(200);
		moveDistance(1900);
		break;

		//drives in reverse towards the flag
	case 2:
		moveDistance(-3000);
		break;
	}
}


task usercontrol()
{
	// holds state of toggle for joystick control.
	bool toggleState;
	// used to prevent rapid flipping of toggleState, set at end of loop
	int prevButtonState;
	//buttonState: set to reference button, prevents value from changing inside loop
	int buttonState;


	/* CODE FOR SAM */
	if(!SensorValue[currentDriver])
	{
		while (true)
		{
			SensorValue[yellow] = buttonState = vexRT[Btn8D];

			/* SINGLE JOYSTICK CONTROL W. TOGGLE */
			if (!toggleState)
				move((vexRT[Ch3] * LOW) + (vexRT[Ch4] * LOW), (vexRT[Ch3] * LOW) - (vexRT[Ch4] * LOW));
			else
				move((vexRT[Ch3] * HIGH) + (vexRT[Ch4] * HIGH), (vexRT[Ch3] * HIGH) - (vexRT[Ch4] * HIGH));

			// claw is controlled by left and right on right joystick value divided by 2.

			motor[claw] = vexRT[Ch1]/2;
			SensorValue[solenoid] = vexRT[Btn8L];

			if(vexRT[Btn6U] && SensorValue[liftEnc] < MAXHEIGHT)
				lift(127);
			else if(vexRT[Btn6D] && !SensorValue[lowerLimit])
				lift(-65);
			else
				lift(0);
			if(vexRT[Btn6U] && SensorValue[liftEnc] < MAXHEIGHT)
				lift(127);
			else if(vexRT[Btn6D] && !SensorValue[lowerLimit])
				lift(-65);
			else
				lift(0);

			if (SensorValue[lowerLimit])
				SensorValue[liftEnc] = 0;


			// reverses state of toggle if button is pressed and wasn't pressed in previous loop.
			if (buttonState && prevButtonState != buttonState)
			{
				SensorValue[red] = toggleState = !toggleState;
			}

			// sets prevButtonState to curr. button state.
			prevButtonState = buttonState;
		}

	}



	/* CODE FOR OWEN */
	else
	{
		while (true)
		{
			SensorValue[yellow] = buttonState = vexRT[Btn8D];

			/* CODE FOR DUAL JOYSTICK CONTROL W. TOGGLE */
			if (!toggleState)
				move(vexRT[Ch3] * LOW, vexRT[Ch2] * LOW);
			else
				move(vexRT[Ch3] * HIGH, vexRT[Ch2] * HIGH);

			// In this variant, the claw is controlled by the accelerometer in the X direction.
			// May have to add deadzone in the future for ease of use.
			clawDeadzone(vexRT[AccelX]/2, 7);

			SensorValue[solenoid] = vexRT[Btn8L];



			if(vexRT[Btn6U]  && SensorValue[liftEnc] < MAXHEIGHT)
				lift(127);
			else if(vexRT[Btn6D] && !SensorValue[lowerLimit])
				lift(-70);
			else
				lift(0);
			if (SensorValue[lowerLimit])
				SensorValue[liftEnc] = 0;

			// reverses state of toggle if button is pressed and wasn't pressed in previous loop.
			if (buttonState && prevButtonState != buttonState)
			{
				SensorValue[red] = toggleState = !toggleState;
			}

			// sets prevButtonState to curr. button state.
			prevButtonState = buttonState;
		}
	}
}
