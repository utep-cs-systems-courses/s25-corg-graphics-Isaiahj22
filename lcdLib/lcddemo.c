/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "buzzer.h"

#define SWITCH BIT3
#define BUTTONS (BIT0 | BIT1 | BIT2 | BIT3)
#define LED_RED BIT0   // P1.0
#define LED_GREEN BIT6 // P1.6
#define LEDS (LED_RED | LED_GREEN)
int movRow = 0;
int secondCount = 0;
int currentColor = 0;
int zero = 60;
int state = 0;

int e = 40;
int g = 30;
int a = 20;
int t = 10;
int s = 0;

void updateLetters(){
  if (zero >= 110){
      zero = 0;
    }else{
      zero += 5;
    }

    if (e >= 110){
      e = 0;
    }else{
      e += 5;
    }

    if (g >= 110){
      g = 0;
    }else{
      g += 5;
    }

    if (a >= 110){
      a = 0;
    }else{
      a += 5;
    }

    if (t >= 110){
      t = 0;
    }else{
      t += 5;
    }

    if (s >= 110){
      s = 0;
    }else{
      s += 5;
    }

}

void printWelcomeScreen(){
  static int flip = 0;
    if(flip){
      //clearScreen(COLOR_BLACK);
    //fillRectangle();
      drawString11x14(25,50,"Welcome",COLOR_WHITE,COLOR_BLACK);
      drawString5x7(10,90,"Press any button to start", COLOR_WHITE,COLOR_BLACK);
      drawString5x7(10,100,"to start", COLOR_WHITE,COLOR_BLACK);
      flip = 0;
    }else{
      //clearScreen(COLOR_WHITE);
      //fillRectangle();
      //drawString11x14(25,50,"Welcome",COLOR_BLACK,COLOR_BLACK);
      drawString5x7(10,90,"Press any button", COLOR_BLACK,COLOR_BLACK);
      drawString5x7(10,100,"to start", COLOR_BLACK,COLOR_BLACK);
      flip++;
    }
  
}

void printPuzzleScreen(){

  fillRectangle(0,4,180,20, COLOR_BLACK);
  fillRectangle(15,30, 45, 45, COLOR_RED);
  fillRectangle(70,30, 45, 45, COLOR_BLUE);
  fillRectangle(15,85, 45, 45, COLOR_YELLOW);
  fillRectangle(70,85, 45, 45, COLOR_GREEN);
    
  drawChar11x14(s,5,'S',COLOR_WHITE,COLOR_BLACK);
  drawChar11x14(t,5,'t',COLOR_WHITE,COLOR_BLACK);
  drawChar11x14(a,5,'a',COLOR_WHITE,COLOR_BLACK);
  drawChar11x14(g,5,'g',COLOR_WHITE,COLOR_BLACK);
  drawChar11x14(e,5,'e',COLOR_WHITE,COLOR_BLACK);
  drawChar11x14(zero,10,state + 48,COLOR_WHITE,COLOR_BLACK);
  updateLetters();

}



void playPattern(int pattern[]){

  if (pattern[currentColor] == 1){
    fillRectangle(15,30, 45, 45, COLOR_WHITE);
    buzzer_set_period(1000);
    }
  if (pattern[currentColor] == 2){
    fillRectangle(70,30, 45, 45, COLOR_WHITE);
    buzzer_set_period(2000);
    }
  if (pattern[currentColor] == 3){
    fillRectangle(15,85, 45, 45, COLOR_WHITE);
    buzzer_set_period(3000);
    }
  if (pattern[currentColor] == 4){
    fillRectangle(70,85, 45, 45, COLOR_WHITE);
    buzzer_set_period(4000);
    }

}
int answer1[5] = {0,0,0,0,0};
int answer2[6] = {0,0,0,0,0,0};
int answer3[7] = {0,0,0,0,0,0,0};
int currentInput = 0;
int puzzle1[5] = {1,3,1,2,4};
int puzzle2[6] = {4,2,2,1,4,3};
int puzzle3[7] = {3,4,2,1,4,2,1};
  
void __interrupt_vec(WDT_VECTOR) WDT()
{
  static int test = 0;
  
  secondCount++;
  
  //if (secondCount >= 250){ 

    switch(state){
      //Welcome screen, Click any button to start.
    case 0:
      if(secondCount >= 250){
	buzzer_set_period(0);
	printWelcomeScreen();
	secondCount = 0;
	}
      break;

      //Puzzle 1, at any point click button 0 to restart.
    case 1:
      
      
      if (secondCount >= 250){
	printPuzzleScreen();
	if(currentColor < 5){
	  buzzer_set_period(0);
	  playPattern(puzzle1);
	}
	if (currentColor >= 5){
	  buzzer_set_period(0);

	}
	currentColor += 1;
	secondCount = 0;

      }
      break;


      //Puzzle 2
    case 2:
      if (secondCount >= 250){
	printPuzzleScreen();
	if(currentColor < 6){
	  buzzer_set_period(0);
	  playPattern(puzzle2);
	}
	if (currentColor >= 6){
	  buzzer_set_period(0);

	}
	currentColor += 1;
	secondCount = 0;

      }
      break;


      //Puzzle 3
    case 3:
      if (secondCount >= 250){
	printPuzzleScreen();
	if(currentColor < 7){
	  buzzer_set_period(0);
	  playPattern(puzzle3);
	}
	if (currentColor >= 7){
	  buzzer_set_period(0);

	}
	currentColor += 1;
	secondCount = 0;

      }
      break;

      //You Win, Click button 0 to restart
    case 4:
      buzzer_set_period(0);
      break;

      //You Lost, Restart?
    case 5:
      buzzer_set_period(0);
      break;


    default:
      break;

    }
}


void __interrupt_vec(PORT2_VECTOR) Port2_ISR() {

  if (P2IFG & BIT0){
    switch(state){
    case 0:
      clearScreen(COLOR_BLACK);
      state = 1;
      break;

    case 1:
      buzzer_set_period(1000);
      fillRectangle(15,30, 45, 45, COLOR_WHITE);
      if (puzzle1[currentInput] != 1){
	state = 4;
	clearScreen(COLOR_RED);
      }
      currentInput++;
      if (currentInput == 5){
	state = 2;
	clearScreen(COLOR_BLACK);
	currentInput = 0;
	currentColor = 0;
	buzzer_set_period(0);
      }
      break;


    case 2:
      buzzer_set_period(1000);
      fillRectangle(15,30, 45, 45, COLOR_WHITE);
      if (puzzle2[currentInput] != 1){
	state = 4;
	clearScreen(COLOR_RED);
      }
      currentInput++;
      if (currentInput == 6){
	state = 3;
	clearScreen(COLOR_BLACK);
	currentInput = 0;
	currentColor = 0;
	buzzer_set_period(0);
      }
      break;

    case 3:
      buzzer_set_period(1000);
      fillRectangle(15,30, 45, 45, COLOR_WHITE);
      if (puzzle3[currentInput] != 1){
	state = 4;
	clearScreen(COLOR_RED);
      }
      currentInput++;
      if (currentInput == 7){
	state = 5;
	clearScreen(COLOR_GREEN);
	currentInput = 0;
	currentColor = 0;
	buzzer_set_period(0);
      }
      break;

    case 4:
      clearScreen(COLOR_BLACK);
      state = 1;
      currentInput = 0;
      currentColor = 0;
      break;

    case 5:
      clearScreen(COLOR_BLACK);
      currentInput = 0;
      currentColor = 0;
      state = 1;
      break;

    default:
      clearScreen(COLOR_BLUE);
      break;
    }
  }

  if (P2IFG & BIT1) {

    switch(state){
    case 0:
      clearScreen(COLOR_BLACK);
      state = 1;
      break;
      
    case 1:
      buzzer_set_period(2000);
      fillRectangle(70,30, 45, 45, COLOR_WHITE);
      if (puzzle1[currentInput] != 2){
	state = 4;
	clearScreen(COLOR_RED);
      }
      currentInput++;
      if (currentInput == 5){
	state = 2;
	clearScreen(COLOR_BLACK);
	currentInput = 0;
	currentColor = 0;
	buzzer_set_period(0);
      }
      break;

    case 2:
      buzzer_set_period(2000);
      fillRectangle(70,30, 45, 45, COLOR_WHITE);
      if (puzzle2[currentInput] != 2){
	state = 4;
	clearScreen(COLOR_RED);
      }
      currentInput++;
      if (currentInput == 6){
	state = 3;
	clearScreen(COLOR_BLACK);
	currentInput = 0;
	currentColor = 0;
	buzzer_set_period(0);
      }
      break;

    case 3:
      buzzer_set_period(2000);
      fillRectangle(70,30, 45, 45, COLOR_WHITE);
      if (puzzle3[currentInput] != 2){
	state = 4;
	clearScreen(COLOR_RED);
      }
      currentInput++;
      if (currentInput == 7){
	state = 5;
	clearScreen(COLOR_GREEN);
	currentInput = 0;
	currentColor = 0;
	buzzer_set_period(0);
      }
      break;

    case 4:
      clearScreen(COLOR_BLACK);
      state = 1;
      currentInput = 0;
      currentColor = 0;
      break;

    case 5:
      clearScreen(COLOR_BLACK);
      state = 1;
      currentInput = 0;
      currentColor = 0;
      break;

    default:
      clearScreen(COLOR_BLUE);
      break;
      
    }
  }

    

  if (P2IFG & BIT2) {
    

    switch(state){
    case 0:
      
      break;
      
    case 1:
      buzzer_set_period(3000);
      fillRectangle(15,85, 45, 45, COLOR_WHITE);
      if (puzzle1[currentInput] != 3){
	state = 4;
	clearScreen(COLOR_RED);
      }
      currentInput++;
      if (currentInput == 5){
	state = 2;
	clearScreen(COLOR_BLACK);
	currentInput = 0;
	currentColor = 0;
	buzzer_set_period(0);
      }
      break;

    case 2:
      buzzer_set_period(3000);
      fillRectangle(15,85, 45, 45, COLOR_WHITE);
      if (puzzle2[currentInput] != 3){
	state = 4;
	clearScreen(COLOR_RED);
      }
      currentInput++;
      if (currentInput == 6){
	state = 3;
	clearScreen(COLOR_BLACK);
	currentInput = 0;
	currentColor = 0;
	buzzer_set_period(0);
      }
      break;

    case 3:
      buzzer_set_period(3000);
      fillRectangle(15,85, 45, 45, COLOR_WHITE);
      if (puzzle3[currentInput] != 3){
	state = 4;
	clearScreen(COLOR_RED);
      }
      currentInput++;
      if (currentInput == 7){
	state = 5;
	clearScreen(COLOR_GREEN);
	currentColor = 0;
	currentInput = 0;
	buzzer_set_period(0);
      }
      break;

    case 4:
      clearScreen(COLOR_BLACK);
      state = 1;
      currentInput = 0;
      currentColor = 0;
      break;

    case 5:
      clearScreen(COLOR_BLACK);
      state = 1;
      currentInput = 0;
      currentColor = 0;
      break;

    default:
      clearScreen(COLOR_BLUE);

      break;

    }
    
  }


  if (P2IFG & BIT3) {

    switch(state){
    case 0:
      clearScreen(COLOR_BLACK);
      state = 1;
      break;
      
    case 1:
      buzzer_set_period(4000);
      fillRectangle(70,85, 45, 45, COLOR_WHITE);
      if (puzzle1[currentInput] != 4){
	state = 4;
	clearScreen(COLOR_RED);
      }
      currentInput++;
      if (currentInput == 5){
	state = 2;
	clearScreen(COLOR_BLACK);
	currentInput = 0;
	currentColor = 0;
	buzzer_set_period(0);
      }
      break;

    case 2:
      buzzer_set_period(4000);
      fillRectangle(70,85, 45, 45, COLOR_WHITE);
      if (puzzle2[currentInput] != 4){
	state = 4;
	clearScreen(COLOR_RED);
      }
      currentInput++;
      if (currentInput == 6){
	state = 3;
	clearScreen(COLOR_BLACK);
	currentInput = 0;
	currentColor = 0;
	buzzer_set_period(0);
      }
      break;

    case 3:
      buzzer_set_period(4000);
      fillRectangle(70,85, 45, 45, COLOR_WHITE);
      if (puzzle3[currentInput] != 4){
	state = 4;
	clearScreen(COLOR_RED);
      }
      currentInput++;
      if (currentInput == 7){
	state = 5;
	clearScreen(COLOR_GREEN);
	currentColor = 0;
	currentInput = 0;
	buzzer_set_period(0);
      }
      break;

    case 4:
      clearScreen(COLOR_BLACK);
      state = 1;
      currentInput = 0;
      currentColor = 0;
      break;

    case 5:
      clearScreen(COLOR_BLACK);
      state = 1;
      currentInput = 0;
      currentColor = 0;
      break;

    default:

      break;


    }

    
  }


  P2IFG &= ~BUTTONS;
}

void button_init(){

  P1REN |= SWITCH;		/* enables resistors for switches */
  P1IE |= SWITCH;		/* enable interrupts from switches */
  P1OUT |= SWITCH;		/* pull-ups for switches */
  P1DIR &= ~SWITCH;		/* set switches' bits for input */

  
  P2REN |= BUTTONS;
  P2IE |= BUTTONS;
  P2OUT |= BUTTONS;
  P2DIR &= ~BUTTONS;


}

void
__interrupt_vec(PORT1_VECTOR) Port_1(){
  if (P1IFG & SWITCH) {
    
    if(state > 0){
      buzzer_set_period(0);
      clearScreen(COLOR_BLACK);
      state = 0;
      currentColor = 0;
      currentInput = 0;
    }else{
      clearScreen(COLOR_BLACK);
      state = 1;
    }
  }
  P1IFG &= ~SWITCH;
}


int
main()
{
  configureClocks();
  lcd_init();

  
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLACK);

  short rowMiddle = screenHeight/2;
  short colMiddle = screenWidth/2;
  short row, col;
  buzzer_init();
  button_init();
  enableWDTInterrupts();
  or_sr(0x18);
}
