/*! \file */
/******************************************************************************
 * Final Project: Guitar Hero
 *
 * Description: A guitar hero game.
 * Authors: Josh Schrock and Lisa Sebastian
 * Last-modified: 2/6/2023
 *
 *
*******************************************************************************/
#include "msp.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "csHFXT.h"
#include "csLFXT.h"
#include "pushButtons.h"
#include "guitarButtons.h"
#include "OLED_cmd.h"
#include "lcd.h"
#include "oled_ops.h"
#include "i2c_ops.h"

//clock define
#define CLK_FREQUENCY       48000000    // MCLK using 48MHz HFXT

/* note defines */
#define NOTENONE 90.55
#define NOTEE3 164.81
#define NOTEF3 174.61
#define NOTEF3SHARP 185.00
#define NOTEG3 196.00
#define NOTEG3SHARP 207.65
#define NOTEA3 220.00
#define NOTEA3SHARP 233.08
#define NOTEB3 246.94
#define NOTEC4 261.63
#define NOTEC4SHARP 277.18
#define NOTED4 293.66
#define NOTED4SHARP 311.13
#define NOTEE4 329.63
#define NOTEF4 349.23
#define NOTEF4SHARP 369.99
#define NOTEG4 392.00
#define NOTEG4SHARP 415.30
#define NOTEA4 440.00
#define NOTEA4SHARP 466.16
#define NOTEB4 493.88
#define NOTEC5 523.25
#define NOTEC5SHARP 554.37
#define NOTED5 587.33
#define NOTED5SHARP  622.25
#define NOTEE5  659.25
#define NOTEF5  698.46
#define NOTEF5SHARP  739.99
#define NOTEG5  783.99
#define NOTEG5SHARP  830.61
#define NOTEA5  880.00
#define NOTEA5SHARP  932.33
#define NOTEB5  987.77
#define NOTEC6  1046.50

/* length defines */
#define RESTNOTE 1000
#define SIXTEENNOTE 3096
#define EIGTHNOTE 7192
#define TRIPLETNOTE 10256
#define TQANDEIGTHNOTE 11288
#define QUARTERNOTE 15384
#define DQUARTERNOTE 23576
#define HALFNOTE 31768
#define TEIGTHANDHALFNOTE 39961
#define DHALFNOTE 48152
#define WHOLENOTE 64536

// guitar button defines
#define SINGLENOTETIME 3000

//define game
#define INITHEALTH     100

typedef enum _GuitarButtons {
    BNONE,       /* (0x0) */
    BONE,        /* (0x1) */
    BTWO,        /* (0x2) */
    BTHREE,      /* (0x3) */
    BFOUR        /* (0x4) */
} GuitarButtons;

/* OLED defines */
//unsigned char TEXT_ARR[1024];
//unsigned char LOGO_ARR[1024];

/* song defines */
#define NOTECNT 239
//const uint16_t noteArray[NOTECNT] =   {NOTEG4,      NOTENONE,  NOTED4,    NOTEG4,      NOTENONE,  NOTED4,      NOTEG4,    NOTED4,    NOTEG4,    NOTEB4,    NOTED5,      NOTENONE,  NOTEC5,      NOTENONE,  NOTEA4,    NOTEC5,      NOTENONE,  NOTEA4,    NOTEC5,    NOTEA4,    NOTEF4SHARP, NOTEA4,    NOTED4,      NOTENONE,    NOTEG4,    NOTENONE,  NOTEG4,  NOTEB4,    NOTEA4,    NOTEG4,    NOTEG4,    NOTEF4SHARP, NOTEF4SHARP, NOTEG4,    NOTEC5,    NOTEF4SHARP, NOTEA4,    NOTEG4,    NOTEG4,  NOTEB4,    NOTEA4,    NOTEG4,    NOTEG4,    NOTEF4SHARP, NOTEF4SHARP, NOTEA4,    NOTEC5,    NOTEF4SHARP, NOTEG4,    NOTEG4,    NOTEG4,    NOTEF4SHARP, NOTEE4,    NOTEF4SHARP, NOTEG4,    NOTEG4,    NOTEB4,    NOTEA4,    NOTEG4,    NOTEA4,    NOTEB4,    NOTEB4,    NOTED5,    NOTEC5,    NOTEB4,    NOTEC5,    NOTED5,      NOTENONE,    NOTED4,   NOTEE4,   NOTED4,    NOTEC4,     NOTEC4,      NOTEC4,    NOTEB3,     NOTEB3,      NOTEB3,    NOTEA3,     NOTEA3,      NOTEG3,    NOTEF3SHARP, NOTEE3,    NOTEF3SHARP, NOTEG3,    NOTENONE,  NOTEA3,    NOTENONE,  NOTEB3,    NOTENONE,  NOTENONE};
//const uint16_t lengthArray[NOTECNT] = {QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE, QUARTERNOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, QUARTERNOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE, TIENOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   TIENOTE,     EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE, TIENOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   TIENOTE,     EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE, QUARTERNOTE, HALFNOTE, HALFNOTE, EIGHTNOTE, SIXTIENOTE, QUARTERNOTE, EIGHTNOTE, SIXTIENOTE, QUARTERNOTE, EIGHTNOTE, SIXTIENOTE, QUARTERNOTE, EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE,   EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, EIGHTNOTE, QUARTERNOTE};
//const uint16_t buttonArray[NOTECNT] = {BTWO,        BNONE,      BONE,      BTWO,      BNONE,     BONE,      BTWO,      BONE,      BTWO,      BTHREE,      BFOUR,      BNONE,       BTHREE,         BNONE,     BTWO,     BTHREE,       BNONE,     BTWO,     BTHREE,     BNONE,     BNONE,       BNONE,     BNONE,       BNONE,       BNONE,     BNONE,     BNONE,   BNONE,     BNONE,     BNONE,     BNONE,     BNONE,       BNONE,       BNONE,     BNONE,     BNONE,        BNONE,    BNONE,     BNONE,   BNONE,     BNONE,     BNONE,     BNONE,     BNONE,        BNONE,       BNONE ,   BNONE,      BNONE,      BNONE,     BNONE};
const uint16_t noteArray[NOTECNT] = {NOTEE3, NOTENONE, NOTEE3, NOTENONE, NOTEE3, NOTENONE, NOTEF4, NOTEG4, NOTEG4, NOTEG4, NOTEF4, NOTEE4, NOTEF4, NOTEE4, NOTEC4, NOTENONE, NOTENONE, NOTEC4, NOTEC4, NOTED4, NOTED4, NOTED4, NOTENONE, NOTEF4, NOTEG4, NOTEG4, NOTEG4, NOTEF4, NOTEE4, NOTEF4, NOTEE4, NOTEC4, NOTENONE, NOTEA3, NOTEA3, NOTEA3, NOTEF4, NOTEF4, NOTEF4, NOTEE4, NOTEC4, NOTENONE, NOTEG5, NOTEG5, NOTEG5, NOTEG5, NOTEG5, NOTEG5, NOTEF5, NOTEF5, NOTEF5, NOTEE5, NOTEC5, NOTENONE, NOTEC5, NOTEC5, NOTEC5, NOTEC5, NOTEC5, NOTEC5, NOTED5, NOTED5, NOTED5, NOTENONE, NOTEE5, NOTEF5, NOTEG5, NOTEG5, NOTEG5, NOTEG5, NOTEF5, NOTEE5, NOTEF5, NOTEE5, NOTEE5, NOTEE5, NOTENONE, NOTENONE, NOTEC5, NOTEC5, NOTEC5, NOTEE5, NOTEF5, NOTEE5, NOTEC5, NOTEC5, NOTENONE, NOTEG5, NOTEG5, NOTEG5, NOTEG5, NOTEF5, NOTEE5, NOTEF5, NOTEE5, NOTEC5, NOTENONE, NOTEA4, NOTEC5, NOTEC5, NOTEC5, NOTED5, NOTED5, NOTENONE, NOTEE5, NOTEF5, NOTEG5, NOTEG5, NOTEG5, NOTEG5, NOTEF5, NOTEE5, NOTEF5, NOTEE5, NOTEC5, NOTENONE, NOTENONE, NOTEG4, NOTEA4, NOTEC5, NOTEC5, NOTEF5, NOTEE5, NOTEC5, NOTEC5, NOTENONE, NOTEF4, NOTEF4, NOTEG4, NOTEG4, NOTEG4, NOTEG4, NOTEG4, NOTENONE, NOTED4, NOTEE4, NOTEF4, NOTEE4, NOTEE4, NOTENONE, NOTEE4, NOTED4, NOTEC4, NOTEC4, NOTEC4, NOTENONE, NOTEC4, NOTED4, NOTED4, NOTED4, NOTENONE, NOTEE4, NOTEF4, NOTEG4, NOTEG4, NOTEG4, NOTEG4, NOTEF4, NOTEE4, NOTEF4, NOTEE4, NOTEE4, NOTEE4, NOTENONE, NOTEE4, NOTEF4, NOTEF4, NOTEF4, NOTEF4, NOTEF4, NOTENONE, NOTEC4, NOTEC4, NOTEC4, NOTENONE, NOTEG5, NOTEG5, NOTEG5, NOTEG5, NOTEG5, NOTEG5, NOTEG5, NOTEF5, NOTEF5, NOTEE5, NOTEE5, NOTENONE, NOTEE5, NOTED5, NOTEC5, NOTEC5, NOTEC5, NOTEC5, NOTEC5, NOTED5, NOTED5, NOTED5, NOTENONE, NOTEE5, NOTEF5, NOTEG5, NOTEG5, NOTEG5, NOTEG5, NOTEF5, NOTEF5, NOTEF5, NOTEE5, NOTEE5, NOTEE5, NOTEE5, NOTEE5, NOTEF5, NOTEF5, NOTEF5, NOTEF5, NOTEC5, NOTEC5, NOTEC5, NOTENONE, NOTEC5, NOTEB4, NOTEA4, NOTEA4, NOTEA4, NOTEB4, NOTEC5, NOTEB4, NOTEA4, NOTENONE, NOTEA4, NOTEA4, NOTEB4, NOTEC5, NOTEB4, NOTEA4SHARP, NOTEA4, NOTEG4, NOTENONE};
const uint16_t lengthArray[NOTECNT] = {EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, QUARTERNOTE, HALFNOTE, QUARTERNOTE, QUARTERNOTE, DQUARTERNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, HALFNOTE, DHALFNOTE, DQUARTERNOTE, EIGTHNOTE, HALFNOTE, EIGTHNOTE, EIGTHNOTE, HALFNOTE, DHALFNOTE, EIGTHNOTE, EIGTHNOTE, QUARTERNOTE, WHOLENOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, DQUARTERNOTE, HALFNOTE, EIGTHNOTE, EIGTHNOTE, DQUARTERNOTE, EIGTHNOTE, QUARTERNOTE, EIGTHNOTE, QUARTERNOTE, TQANDEIGTHNOTE, DHALFNOTE, EIGTHNOTE, QUARTERNOTE, QUARTERNOTE, EIGTHNOTE, EIGTHNOTE, TQANDEIGTHNOTE, DQUARTERNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, HALFNOTE, HALFNOTE, QUARTERNOTE, QUARTERNOTE, EIGTHNOTE, EIGTHNOTE, QUARTERNOTE, HALFNOTE, EIGTHNOTE, EIGTHNOTE, DHALFNOTE, QUARTERNOTE, EIGTHNOTE, EIGTHNOTE, DQUARTERNOTE, EIGTHNOTE, QUARTERNOTE, QUARTERNOTE, EIGTHNOTE, DQUARTERNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, HALFNOTE, QUARTERNOTE, EIGTHNOTE, EIGTHNOTE, QUARTERNOTE, QUARTERNOTE, EIGTHNOTE, TQANDEIGTHNOTE, EIGTHNOTE, DQUARTERNOTE, WHOLENOTE, DHALFNOTE, EIGTHNOTE, DQUARTERNOTE, EIGTHNOTE, TEIGTHANDHALFNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, QUARTERNOTE, TQANDEIGTHNOTE, DHALFNOTE, EIGTHNOTE, DQUARTERNOTE, EIGTHNOTE, TEIGTHANDHALFNOTE, EIGTHNOTE, TEIGTHANDHALFNOTE, DHALFNOTE, EIGTHNOTE, EIGTHNOTE, QUARTERNOTE, QUARTERNOTE, EIGTHNOTE, TQANDEIGTHNOTE, DQUARTERNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, QUARTERNOTE, DHALFNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, QUARTERNOTE, TQANDEIGTHNOTE, EIGTHNOTE, EIGTHNOTE, HALFNOTE, DHALFNOTE, WHOLENOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, QUARTERNOTE, HALFNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, QUARTERNOTE, DHALFNOTE, EIGTHNOTE, EIGTHNOTE, QUARTERNOTE, EIGTHNOTE, QUARTERNOTE, EIGTHNOTE, QUARTERNOTE, QUARTERNOTE, EIGTHNOTE, TEIGTHANDHALFNOTE, DHALFNOTE, EIGTHNOTE, EIGTHNOTE, DQUARTERNOTE, EIGTHNOTE, QUARTERNOTE, DQUARTERNOTE, QUARTERNOTE, EIGTHNOTE, TRIPLETNOTE, TRIPLETNOTE, TRIPLETNOTE, QUARTERNOTE, HALFNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, HALFNOTE, QUARTERNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, WHOLENOTE, WHOLENOTE, QUARTERNOTE, QUARTERNOTE, EIGTHNOTE, DQUARTERNOTE, QUARTERNOTE, HALFNOTE, EIGTHNOTE, EIGTHNOTE, QUARTERNOTE, EIGTHNOTE, DQUARTERNOTE, QUARTERNOTE, EIGTHNOTE, EIGTHNOTE, EIGTHNOTE, QUARTERNOTE, EIGTHNOTE, HALFNOTE, HALFNOTE, EIGTHNOTE, EIGTHNOTE, DHALFNOTE, QUARTERNOTE, QUARTERNOTE, QUARTERNOTE, EIGTHNOTE, QUARTERNOTE, EIGTHNOTE, HALFNOTE, QUARTERNOTE, QUARTERNOTE, TRIPLETNOTE, TRIPLETNOTE, TRIPLETNOTE, HALFNOTE, QUARTERNOTE, QUARTERNOTE, HALFNOTE, EIGTHNOTE, EIGTHNOTE, DQUARTERNOTE, EIGTHNOTE, EIGTHNOTE, TEIGTHANDHALFNOTE, HALFNOTE, QUARTERNOTE, QUARTERNOTE, HALFNOTE, EIGTHNOTE, TEIGTHANDHALFNOTE, EIGTHNOTE, EIGTHNOTE, QUARTERNOTE, WHOLENOTE, HALFNOTE, EIGTHNOTE, TEIGTHANDHALFNOTE, EIGTHNOTE, EIGTHNOTE, WHOLENOTE, DQUARTERNOTE, EIGTHNOTE, HALFNOTE, WHOLENOTE};
const uint16_t buttonArray[NOTECNT] = {BNONE, BNONE, BNONE, BNONE, BNONE, BNONE, BONE, BTWO, BTWO, BTWO, BONE, BTHREE, BFOUR, BTHREE, BTWO, BNONE, BNONE, BONE, BONE, BTWO, BTWO, BTWO, BNONE, BTHREE, BFOUR, BFOUR, BFOUR, BTHREE, BTWO, BTHREE, BTWO, BONE, BNONE, BONE, BONE, BONE, BFOUR, BFOUR, BFOUR, BTHREE, BONE, BNONE, BFOUR, BFOUR, BFOUR, BFOUR, BFOUR, BFOUR, BTHREE, BTHREE, BTHREE, BTWO, BONE, BNONE, BONE, BONE, BONE, BONE, BONE, BONE, BTWO, BTWO, BTWO, BNONE, BONE, BTWO, BTHREE, BTHREE, BTHREE, BTHREE, BTWO, BONE, BTWO, BONE, BONE, BONE, BNONE, BNONE, BONE, BONE, BONE, BTHREE, BFOUR, BTHREE, BONE, BONE, BNONE, BFOUR, BFOUR, BFOUR, BFOUR, BTHREE, BTWO, BTHREE, BTWO, BONE, BNONE, BONE, BTHREE, BTHREE, BTHREE, BFOUR, BFOUR, BNONE, BTWO, BTHREE, BFOUR, BFOUR, BFOUR, BFOUR, BTHREE, BTWO, BTHREE, BTWO, BONE, BNONE, BNONE, BONE, BTWO, BTHREE, BTHREE, BFOUR, BTHREE, BTWO, BTWO, BNONE, BTHREE, BTHREE, BFOUR, BFOUR, BFOUR, BFOUR, BFOUR, BNONE, BONE, BTWO, BFOUR, BTHREE, BTHREE, BNONE, BTHREE, BTWO, BONE, BONE, BONE, BNONE, BONE, BTWO, BTWO, BTWO, BNONE, BTWO, BTHREE, BFOUR, BFOUR, BFOUR, BFOUR, BTHREE, BTWO, BTHREE, BTWO, BTWO, BTWO, BNONE, BTWO, BTHREE, BTHREE, BTHREE, BTHREE, BTHREE, BNONE, BONE, BONE, BONE, BNONE, BFOUR, BFOUR, BFOUR, BFOUR, BFOUR, BFOUR, BFOUR, BTHREE, BTHREE, BTWO, BTWO, BNONE, BTHREE, BTWO, BONE, BONE, BONE, BONE, BONE, BTWO, BTWO, BTWO, BNONE, BTWO, BTHREE, BFOUR, BFOUR, BFOUR, BFOUR, BTHREE, BTHREE, BTHREE, BTWO, BTWO, BTWO, BTWO, BTWO, BTHREE, BTHREE, BTHREE, BTHREE, BONE, BONE, BONE, BNONE, BFOUR, BTHREE, BTWO, BTWO, BTWO, BTHREE, BFOUR, BTHREE, BTWO, BNONE, BTWO, BTWO, BTHREE, BFOUR, BTHREE, BFOUR, BTWO, BONE, BNONE};

bool buttonLine1[4] = {false, false, false, false};
bool buttonLine2[4] = {false, false, false, false};
bool buttonLine3[4] = {false, false, false, false};
bool buttonExpected[4] = {false, false, false, false};

/* globals */
int noteIndex = 0;
int score = 0;
int health = INITHEALTH;
bool updateLCDflag = true;
char buffer[4];

bool setButtonsNextIndex = false;
bool endButtonsNextIndex = false;

void updateLCD(int score, int health);
void gameOverLCD(int score, int health);
void startLCD(int score, int health);
void updateOLED();

/**
 * main.c
 */
void main(void)
{
    volatile bool playing = false;

    /* Stop Watchdog timer */
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    /* Sets MCLK and SMCLK to 48MHz HFXT */
    configHFXT();
    configLFXT();
    BUTTONS_init();
    GB_init();
    // configures pins and delay library
    configLCD(CLK_FREQUENCY);
    initLCD();

    P1->SEL0 &= ~BIT0;                      // Set LED1 pin to GPIO function
    P1->SEL1 &= ~BIT0;
    P1->OUT &= ~BIT0;                       // Clear LED1 to start
    P1->DIR |= BIT0;                        // Set P1.0/LED1 to output

    i2c_init();
    setupOLED();
    display_on();
    display_clear();
    display_on();

//    Display_Logo(1,1,0);
//    Display_Logo(2,1,2);
//    Display_Logo(4,1,4);

    /* Configure GPIO for speaker */
    P2->DIR |= BIT4;            // set P2.4 as output
    P2->SEL0 |= BIT4;           // P2.4 set to TA0.1
    P2->SEL1 &= ~BIT4;

    //initialize timers
    timersINIT();
    /* Configure global interrupts and NVIC */
    // Enable TA1 overflow and TA1CCR2 compare interrupt
    NVIC->ISER[0] |= 1 << TA1_N_IRQn;


    __enable_irq();                 // Enable global interrupt

    while(1)
    {
        if(BUTTON2_pressed()) {
            if(playing){
                TIMER_A0->CTL &= ~0x0010;
                TIMER_A1->CTL &= ~0x0020;
                TIMER_A2->CTL &= ~0x0020;
                BUTTON2_debounce();
                playing = false;
            }else{
                BUTTON2_debounce();
                TIMER_A0->CTL |= 0x0010;
                TIMER_A1->CTL |= 0x0020;
                TIMER_A2->CTL |= 0x0020;
                playing = true;
                startLCD(score, health);
            }
        }
        // what to do if playing
        if (playing) {
            if (setButtonsNextIndex) {
                // Update Buttons
                if(buttonArray[noteIndex + 1] != BNONE) {
                    buttonExpected[buttonArray[(noteIndex + 1) % NOTECNT] - 1] = true;
                }
                int i;
                for(i = 0; i < 4; i++) {
                    buttonLine1[i] = false;
                    buttonLine2[i] = false;
                    buttonLine3[i] = false;
                }

                if(buttonArray[noteIndex + 2] != BNONE) {
                    buttonLine3[buttonArray[(noteIndex + 2) % NOTECNT] - 1] = true;
                }
                if(buttonArray[noteIndex + 3] != BNONE) {
                    buttonLine2[buttonArray[(noteIndex + 3) % NOTECNT] - 1] = true;
                }
                if(buttonArray[noteIndex + 4] != BNONE) {
                    buttonLine1[buttonArray[(noteIndex + 4) % NOTECNT] - 1] = true;
                }
                setButtonsNextIndex = false;
            }
            if (GB1_pressed()) {
                if(buttonExpected[0]) {
                    buttonExpected[0] = false;
                    score++;
                } else {
                    health--;
                }
                updateLCDflag = true;
                GB1_debounce();
            }
            if (GB2_pressed()) {
                if(buttonExpected[1]) {
                    buttonExpected[1] = false;
                    score++;
                } else {
                    health--;
                }
                updateLCDflag = true;
                GB2_debounce();
            }
            if (GB3_pressed()) {
                if (buttonExpected[2]) {
                    buttonExpected[2] = false;
                    score++;
                } else {
                    health--;
                }
                updateLCDflag = true;
                GB3_debounce();
            }
            if (GB4_pressed()) {
                if (buttonExpected[3]) {
                    buttonExpected[3] = false;
                    score++;
                } else {
                    health--;
                }
                updateLCDflag = true;
                GB4_debounce();
            }
            if (updateLCDflag) {
                updateLCD(score, health);
            }

            updateOLED();

            if (health <= 0) {
                gameOverLCD(score, health);
                TIMER_A0->CTL &= ~0x0010;
                TIMER_A1->CTL &= ~0x0020;
                TIMER_A2->CTL &= ~0x0020;
                playing = false;

                health = INITHEALTH;
                score = 0;

                noteIndex = 0;
                GB_init();
                timersINIT();
            }
        }
    }
}

void timersINIT() {
    /* Configure Timer_A0 and CCRs */
    // Set Period in CCR0 register
    TIMER_A0->CCR[0] = (6005000 / (noteArray[noteIndex] + 1)) - 1;
    // Set high pulse-width in CCR1 register (determines duty cycle)
    TIMER_A0->CCR[1] = ((6005000 / (noteArray[noteIndex]) + 1) / 2) - 1;
    // Configure CCR1 for Compare mode with Set/Reset output
    //          - sets when Timer_A0 == CCR1
    //          - resets when Timer_A0 == CCR0
    TIMER_A0->CCTL[1] = 0x60;
    // Configure Timer_A0 in Up Mode with source SMCLK prescale 8:1
    //      Tick rate will be (48MHz/8) with rollover at value in CCR0

    TIMER_A0->CTL = 0x02C4;

    /* Configure Timer_A1 and CCRs */
    // Set initial period in CCR2 register. This assumes timer starts at 0
    TIMER_A1->CCR[1] = lengthArray[noteIndex] - SINGLENOTETIME;
    // Configure CCR2 for Compare mode with interrupt enabled (no output mode - 0)
    TIMER_A1->CCTL[1] = 0x0010;

    TIMER_A1->CCR[2] = lengthArray[noteIndex];
    // Configure CCR2 for Compare mode with interrupt enabled (no output mode - 0)
    TIMER_A1->CCTL[2] = 0x0010;

    // Set initial period in CCR3 register. This assumes timer starts at 0
    TIMER_A1->CCR[3] = lengthArray[noteIndex] + RESTNOTE;
    // Configure CCR3 for Compare mode with interrupt enabled (no output mode - 0)
    TIMER_A1->CCTL[3] = 0x0010;

    // Set initial period in CCR4 register. This assumes timer starts at 0
    TIMER_A1->CCR[4] = SINGLENOTETIME;
    // Configure CCR3 for Compare mode with interrupt enabled (no output mode - 0)
    TIMER_A1->CCTL[4] = 0x0010;

    // Configure Timer_A1 in Continuous Mode with source ACLK prescale 1:1 and
    //  interrupt enabled
    //      Tick rate will be 32kHz with rollover at 0xFFFF
    TIMER_A1->CTL = 0x0106;
}

void updateOLED() {
    unsigned char line1[] = "   /  |   |  \\  ";
    unsigned char line2[] = "  /   |   |   \\ ";
    unsigned char line3[] = " /    |   |    \\";
    unsigned char line4[] = " _    _   _    _ ";
    // line 1
    if (buttonLine1[0]) {
        line1[3] = 'O';
    }
    if (buttonLine1[1]) {
        line1[6] = 'O';
    }
    if (buttonLine1[2]) {
        line1[10] = 'O';
    }
    if (buttonLine1[3]) {
        line1[13] = 'O';
    }
    draw_line(1,1,line1);

    // line 2
    if (buttonLine2[0]) {
        line2[2] = 'O';
    }
    if (buttonLine2[1]) {
        line2[6] = 'O';
    }
    if (buttonLine2[2]) {
        line2[10] = 'O';
    }
    if (buttonLine2[3]) {
        line2[14] = 'O';
    }
    draw_line(2,1,line2);

    // line 3
    if (buttonLine3[0]) {
        line3[1] = 'O';
    }
    if (buttonLine3[1]) {
        line3[6] = 'O';
    }
    if (buttonLine3[2]) {
        line3[10] = 'O';
    }
    if (buttonLine3[3]) {
        line3[15] = 'O';
    }
    draw_line(3,1,line3);

    // line 4
    if (buttonExpected[0]) {
        line4[1] = 'O';
    }
    if (buttonExpected[1]) {
        line4[6] = 'O';
    }
    if (buttonExpected[2]) {
        line4[10] = 'O';
    }
    if (buttonExpected[3]) {
        line4[15] = 'O';
    }
    draw_line(4,1,line4);
    write_display(getTextArr());
}

void gameOverLCD(int score, int health) {
    clearDisplay();
    int i;

    printChar('G');
    printChar('A');
    printChar('M');
    printChar('E');
    printChar(' ');
    printChar('O');
    printChar('V');
    printChar('E');
    printChar('R');
    commandInstruction(SET_CURSOR_MASK | 0x40);
    snprintf(buffer, 4, "%d     ", score);
    printChar('S');
    printChar('c');
    printChar('o');
    printChar('r');
    printChar('e');
    printChar(':');
    printChar(' ');
    for (i = 0; i < 3; i++) {
        printChar(buffer[i]);
    }
    updateLCDflag = false;
}

void updateLCD(int score, int health) {
    commandInstruction(SET_CURSOR_MASK | 0x07);

    int i;
    snprintf(buffer, 4, "%d     ", score);
    for (i = 0; i < 3; i++) {
        printChar(buffer[i]);
    }

    commandInstruction(SET_CURSOR_MASK | 0x48);
    snprintf(buffer, 4, "%d     ", health);
    for (i = 0; i < 3; i++) {
        printChar(buffer[i]);
    }

    updateLCDflag = false;
}

void startLCD(int score, int health) {
    clearDisplay();
    int i;
    snprintf(buffer, 4, "%d     ", score);
    printChar('S');
    printChar('c');
    printChar('o');
    printChar('r');
    printChar('e');
    printChar(':');
    printChar(' ');
    for (i = 0; i < 3; i++) {
        printChar(buffer[i]);
    }

    commandInstruction(SET_CURSOR_MASK | 0x40);
    snprintf(buffer, 4, "%d     ", health);
    printChar('H');
    printChar('e');
    printChar('a');
    printChar('l');
    printChar('t');
    printChar('h');
    printChar(':');
    printChar(' ');
    for (i = 0; i < 3; i++) {
        printChar(buffer[i]);
    }

    updateLCDflag = false;
}

/* Timer_A1 and CCRx (except CCR0) interrupt service routine */
void TA1_N_IRQHandler(void)
{
    /* Check if interrupt triggered by timer overflow */
    if(TIMER_A1->CTL & TIMER_A_CTL_IFG)
    {
        // Clear timer overflow flag
        // clear flag in TA1CTL
        TIMER_A1->CTL = TIMER_A1->CTL & 0xFFFE;
    }

    /* Check if interrupt triggered by CCR2 */
    if(TIMER_A1->CCTL[1] & TIMER_A_CCTLN_CCIFG)
    {
        /* add note changing code (update compare registers) */
        setButtonsNextIndex = true;
        // Clear CCR2 compare interrupt flag
        // clear flag in TA1CCTL2
        TIMER_A1->CCTL[1] = TIMER_A1->CCTL[1] & 0xFFFE;
    }

    /* Check if interrupt triggered by CCR2 */
    if(TIMER_A1->CCTL[2] & TIMER_A_CCTLN_CCIFG)
    {
        /* add note changing code (update compare registers) */
        TIMER_A0->CTL &= ~0x0010;
        TIMER_A0->CCTL[1] &= ~0x0004;

        // Clear CCR2 compare interrupt flag
        // clear flag in TA1CCTL2
        TIMER_A1->CCTL[2] = TIMER_A1->CCTL[2] & 0xFFFE;
    }

    /* Check if interrupt triggered by CCR3 */
    if(TIMER_A1->CCTL[3] & TIMER_A_CCTLN_CCIFG)
    {
        noteIndex = (noteIndex + 1) % NOTECNT;
        /* add note changing code (update compare registers) */
        TIMER_A0->CTL |= 0x0010;
        TIMER_A0->CCR[0] = (6005000 / (noteArray[noteIndex] + 1)) - 1;
        TIMER_A0->CCR[1] = ((6005000 / (noteArray[noteIndex]) + 1) / 2) - 1;


        // Clear CCR2 compare interrupt flag
        // clear flag in TA1CCTL2
        TIMER_A1->CCTL[3] = TIMER_A1->CCTL[3] & 0xFFFE;
        // Schedule next interrupt interval
        TIMER_A1->CCR[1] = (TIMER_A1->CCR[3] + lengthArray[noteIndex] - SINGLENOTETIME) % 0xFFFF;
        TIMER_A1->CCR[2] = (TIMER_A1->CCR[3] + lengthArray[noteIndex]) % 0xFFFF;
        if(buttonArray[noteIndex] != BNONE) {
            TIMER_A1->CCR[4] = (TIMER_A1->CCR[3] + (((lengthArray[noteIndex] / 2) > (SINGLENOTETIME)) ? (lengthArray[noteIndex] / 2) : (SINGLENOTETIME))) % 0xFFFF;
            //Enable timer interrupt
            TIMER_A1->CCTL[4] = 0x0010;
        }
        TIMER_A1->CCR[3] = (TIMER_A1->CCR[3] + lengthArray[noteIndex] + RESTNOTE) % 0xFFFF;
    }

    /* Check if interrupt triggered by CCR4 */
    if(TIMER_A1->CCTL[4] & TIMER_A_CCTLN_CCIFG)
    {
        // Update Buttons
        int i;
        for (i=0; i < 4; i++) {
            if (buttonExpected[i]) {
                health--;
                buttonExpected[i] = false;
            }
        }
        updateLCDflag = true;

        // Clear CCR4 compare interrupt flag and turn off interrupt
        TIMER_A1->CCTL[4] = 0x0000;

    }
}
