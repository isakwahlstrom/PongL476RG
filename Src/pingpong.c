/**
******************************************************************************
@brief Pingpong statemachine for Pingpong-program
@file pingpong.c
@author Bengt Molin
@version 1.0
@date 12-August-2019
@brief Functions and structures for program Pingpong
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "pingpong.h"
#include "stdbool.h"
#include "main.h"
#include "pingpong_functions.h"

/* Define states for state machine*/
typedef enum
{ Start,
MoveRight,
MoveLeft,
ShowPoints,
Finished
} states;
static states State, NextState;
void lala(void){
	bool ButtonPressed; // To remember that button is pressed
	uint32_t Varv, Speed; // Ball speed
	uint8_t Led = 0; // LED nr
	uint8_t L_points = 0;
	uint8_t R_points = 0;
	int start = 1;
	int v;
	int serve=0;
	int round=0;
	State= Start; // Initiate State to Start
	NextState= Start;
	Speed= 500000; // Number of loops
	/* Infinite loop */
	while (1){
		State = NextState;
		switch(round) {
			case 0: v = 400; break;
			case 1: v = 350; break;
			case 2: v = 300; break;
			case 3: v = 250; break;
			case 4: v = 200; break;
			case 5: v = 150; break;
			case 6: v = 100; break;
			case 7: v = 50;  break;
			default: break;

		}


		switch (State) // State machine
		{
			case Start:
			Led_on(0); // Turn off all LEDs
			if (L_hit() == true) // L serve
			{
					serve=1;
					Led = 1;
					NextState= MoveRight;


				while ( L_hit() == true); // wait until button is released
			}

			else
				if ( R_hit() == true) // R serve
			{			serve=2;
						Led = 8;
						NextState= MoveLeft;


				while ( R_hit() == true  ); // wait until button is released
			}
			else
				NextState = Start; // Stay in Start state
			break;
			case MoveRight:
			{

				Led_on(Led);
				Varv = Speed;
				while( Varv != 0 )
				{
					if ( R_hit() ) ButtonPressed = true; // R hit
					Varv--;
				}
				if ( ButtonPressed ) // R pressed
				{
					if ( Led == 8 ) // and LED8 activa
					{
						NextState=MoveLeft; // return ball
						Led=7;
					}
					else { // hit to early
						if(start==1) {
							NextState=Start;
							start = 0;
						} else {
							L_points++;
							if(L_points==4) {
								NextState=Finished;
							} else {
								NextState = ShowPoints;
							}
						}

					}
				}
				else
				{
					if ( Led == 9 ) {// no hit or to late
						if(start==1) {
							NextState=Start;
							start = 0;
						} else {
							L_points++;
							if(L_points==4) {
								NextState=Finished;
							} else {
								NextState = ShowPoints;
							}
						}
					}
					else
						NextState = MoveRight; // ball continues to move right
				}
				if ( !ButtonPressed ) {Led++; HAL_Delay(v);}// prepare to turn next LED on
					ButtonPressed=false;
				}

				break;
			case MoveLeft:
			{
				Led_on(Led);
				Varv = Speed;
				while(Varv != 0)
				{
					if ( L_hit() ) ButtonPressed = true; // L hit
					Varv--;
				}
				if ( ButtonPressed ) // L pressed
				{
					if ( Led == 1 ) // and LED1 active
					{
						NextState=MoveRight; // return ball
						Led=2;
					}
					else {
						 if(start==1) {
							NextState = Start;
							start = 0;
						 } else {
							 R_points++;
							 if(R_points==4) {
								 NextState=Finished;
							} else {
								NextState = ShowPoints;
							}
						 }
					}
				}
				else
				{
					if ( Led == 0 ){ // no hit or to late
						if(start==1) {
							NextState = Start;
							start = 0;
						 } else {
							 R_points++;
							 if(R_points==4) {
								 NextState=Finished;
							 } else {
								 NextState = ShowPoints;
							 }
						}

					}
					else
						NextState = MoveLeft; // ball continues to move left
				}
				if ( !ButtonPressed ) { Led--; HAL_Delay(v); }// prepare to turn next LED on
				ButtonPressed=false;
				break;
			}
			case ShowPoints: Blink(); HAL_Delay(300);  Blink(); HAL_Delay(500);  reset();  round++; Show_points(L_points,R_points); HAL_Delay(1000); reset(); NextState = Start;


			break;
			case Finished: Blink(); HAL_Delay(300); reset(); Show_points(L_points,R_points); HAL_Delay(5000); reset(); NextState = Start; round=0; start = 1; L_points=0; R_points=0; break;
			default:
				break;

		}
	}
}
