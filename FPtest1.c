

void navigate( float positionX, float positionY){
	int TOL= 1;
	//navigate to X encoder coordinate with tolerance of 1 degree
	do{
		while( nMotorEncoder[ motorA]< positionX)
			motor[ motorA]= 10;
		while( nMotorEncoder[ motorA]> positionX)
			motor[ motorA]= -10;

	} while( abs( nMotorEncoder[ motorA]- positionX)> TOL);
motor[motorA]= 0;

	//navigate to Y encoder coordinate with tolerance of 1 degree
	do {
		while( nMotorEncoder[ motorB]< positionY)
			motor[ motorB]= 20;
		while( nMotorEncoder[ motorB]> positionY)
			motor[ motorB]= -20;
	} while( abs( nMotorEncoder[ motorB]- positionY)> TOL);

	motor[motorA]= motor[ motorB]= 0;
}

void zero(){
	//only the motion in the y direction
	do{
	while( nMotorEncoder[ motorB]< 0)
			motor[ motorB]= 10;
	while( nMotorEncoder[ motorB]> 0)
			motor[ motorB]= -10;

	motor[motorB]=0;

	} while( SensorValue[S2]== 0); // do this loop until touch sensor is hit
}



void swipe(){
	navigate( 0, 50);
	//stylus lowered
	motor[ motorC]= 10;
	while( nMotorEncoder[ motorC]< 42){}
	motor[ motorC]= 0;
	wait1Msec(300);

	//swipe
	motor[ motorB]= 5;
	wait1Msec(100);
	motor[ motorB]= 10;
	wait1Msec(100);
	motor[ motorB]= 15;
	wait1Msec(100);
	motor[ motorB]= 20;
	wait1Msec(100);
	motor[ motorB]= 25;  // increment speed of motor B, so it accelerates slowly 
	motor[motorC] = 0;
	wait1Msec(1500);
	motor[ motorB]= 0;
	wait1Msec(100);

	//stylus raised
	motor[ motorC]= -10;
	while( nMotorEncoder[ motorC]> 0){}
	nMotorEncoder[ motorC]= 0;
	motor[ motorC]= 0;

	navigate(0,0);
}

void click(){  // rotates stylus motor to tap screen
	motor[ motorC]= 30;
	while( nMotorEncoder[ motorC]< 40){}
	motor[ motorC]= -30;
	while( nMotorEncoder[ motorC]> 0){} 
	nMotorEncoder[ motorC]= 0;
	motor[ motorC]= 0;
}

void closeText(){  // presses button on screen to close text message notification before unlocking phone
	navigate( -20, 210);
	click();
	navigate(0,0);
}

void playSong(int & song){  // Plays song based on colour sensed by colour sensor
	navigate( -80, 80);
	click();
	wait1Msec(1500);

	if (song != 0){
		if (song == 1) // green colour sensed
			navigate( -24, 115);
		else if (song == 2)  // red colour sensed 
			navigate( -2, 115);
		else // yellow colour sensed
			navigate( 10, 115);
		click();
		displayString( 2, "Playing song %d", song); 
	}
	else
		displayString( 2, "Not a valid colour."); //  if neither of above colours are sensed, play no song
}

int senseTextColour(){
	wait1Msec(1000);
	if (SensorValue[S1] > 10 &&SensorValue[S1] < 25 )
		return 1;
	else if (SensorValue[S1] > 30 && SensorValue[S1] < 38)
		return 2;
	else if (SensorValue[S1] > 40 && SensorValue[S1] < 48)
		return 3;
	else
		return 0;
}

void navHome(){
	navigate( HOMEx, HOMEy);
	click();
	navigate(-24,170);
	click();
	navigate( HOMEx, HOMEy);
}

void openText(){   // opens WhatsApp app on phone
	navigate( -80, 0);
	click();
}



bool detectText(){
	int val= 0;
	while( val< 10)
		val= SensorValue[S1];  // do nothing when screen is off
	displayString(0, "Text just came");  // when screen turns on, display that a text has arrived
	return true;
}

task main{
	const float HOMEx= 0; // variable for origin on X axis
	const float HOMEy= 0; // variable for origin on Y axis		
	
	SensorType[S1]= sensorColorNxtNONE;  
	SensorType[S2]= sensorTouch;
	nMotorEncoder[motorA]= 0;  
	nMotorEncoder[motorB]= 0;
	nMotorEncoder[motorC]= 0; // reset motor encoders to 0 each time program is run

	int song= 0;
	while( nNxtButtonPressed== -1){
		detectText();
		closeText();
		swipe();
		openText();
		navigate(0,0);
		song = senseTextColour();
		displayString(1, "%d", song);
		wait1Msec(5000);
		navHome();
		wait1Msec(5000);
		playSong(song);
		navHome();
	 	zero();  // use touch sensor to ensure stylus position is back at origin
		clearTimer(T1);
		while( time1(T1)< 20000 && nNxtButtonPressed== -1){}  // wait for 20 seconds to restart program, or manually do it by pressing button on NXT
	}
}
