#include <stdio.h>
#include <math.h>


double mag(double x, double y, double z);
int close_to(double tolerance, double point, double value);
int falling(double mag);
double milToSec(int t);
double metersFell(int deltaT);
double percentError(double actual, double expected);

int main(void){
	int t, tInitial, tFinal; 
	double gx, gy, gz;
	
	int prevT = -1;
	
	//Variables for original method
	int initFall = -1;
	int lastFall = -1;
	
	//Variables for new method
	int fallCompT = 0;
	int prevFallCompT = -1;
	double vel, prevVel, pos, prevPos;
	vel = 0;
	prevVel = 0;
	pos = 0;
	prevPos = 0;
	
	
	
	printf("Joshua Arceo\njarceo\nOk, I'm now receiving data.\nI'm waiting");
	
	int inpCount = 0;
	int isFalling = 0;
	
	while(1){
		
	scanf("%d, %lf, %lf, %lf", &t, &gx, &gy, &gz);
	// printf("T: %d Mag: %lf\n", t, mag(gx, gy, gz));
	
	inpCount++;
	
	isFalling = falling(mag(gx,gy,gz));
	
	//AirResistanceCompensation
	if(isFalling){
		if(prevFallCompT == -1) {
			prevFallCompT = t;
		}
		else{
		
		fallCompT = t;
		vel = prevVel + (9.8 * (1-mag(gx,gy,gz))) * (milToSec(fallCompT-prevFallCompT));
		// printf("Vel: %lf\n",vel);
		
		pos = prevPos + (vel * (milToSec(fallCompT-prevFallCompT)));
		
		prevPos = pos;
		prevFallCompT = fallCompT;
		prevVel = vel;
		}
	}
	
	if(isFalling){
		if(initFall == -1){
		printf("\n\tI'm falling");
		initFall = t;
		}
		lastFall = t;
		// printf("Falling\n");
		if(inpCount%10==0) printf("!");
	}
	
	else{
		if(inpCount%10 == 0 && initFall == - 1) printf(".");
	}
	
	if(lastFall!=-1 && !close_to(5, t, lastFall)){
		// printf("No longer falling\n");
		break;
	}

	
	//Used only when using sample data.
	//Breaks the loop when no more sample data and object has never fallen
	if(prevT == t) break;
	prevT = t;
	
	
	fflush(stdout);
	}
	
	// printf("Delta T %d\n", lastFall-initFall);
	
	// printf("init: %d\nFinal: %d", initFall, lastFall);
	int deltaT = lastFall-initFall;
	
	double oldMethodDist = metersFell(deltaT);
	double oldMethodTime = milToSec(deltaT);
	
	printf("\n\t\tOuch! I fell %.3lf meters in %.3lf seconds.", oldMethodDist, oldMethodTime);
	
	printf("\n\nCompensating for air resistance, the fall was %lf\nThis is %.2lf%% less than computed before.", pos, percentError(pos, oldMethodDist));
	
	return 0;
}

double percentError(double actual, double expected){
	return fabs((actual - expected) / expected) * 100;
}

//Returns the magnitude of 3 values
double mag(double x, double y, double z){
	return sqrt( (x * x) + (y * y) + (z * z));
}

//returns 1 if a value is within tolerance range of a point 
int close_to(double tolerance, double point, double value){
	return fabs(point-value)<tolerance;
}

int falling(double mag){
	return !close_to(.1, 1.0, mag) && mag<1.0;
}

double milToSec(int t){
	return t/1000.0;
}

double metersFell(int deltaT){
	double seconds = milToSec(deltaT);
	return 4.9*seconds*seconds;
}