#include "cv1800b.h"
#include "devmem.h"
#include "blink_led.h"
#include "translate_to_morse.h"

#include <stdint.h>
#include <stdio.h>

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define LOW (0)
#define HIGH (1 << 24)

#ifndef HOST
    /* Sets LED GPIO2 to output */
    static void setGPIOMode()
    {
        /* Sets SWPORTA to output */
        volatile uint32_t* swporta_ddr_value = &GPIO2->SWPORTA_DDR;

        //Reads current value in reg
        uint32_t val = dev_mem_read((unsigned long)swporta_ddr_value);

        debug("Intializing gpio: current val %u\n", val);

        /*Sets 24th bit to high*/
        val |= HIGH;
        debug("Intializing gpio: updated val %u\n", val);

        //Write new value into reg => GPIO is now ouput
        dev_mem_write((unsigned long)swporta_ddr_value, val);
    }

    /*
    *   Handles Unmaps and such. 
    */
    static void freeResources()
    {
        /* Powers off LED when exe is stopped */
        volatile unsigned* swporta_dr_value = &GPIO2->SWPORTA_DR;

        dev_mem_write((unsigned long)swporta_dr_value, LOW);

        /* Resets GPIO to valdefault input mode */
        volatile unsigned* const swporta_ddr_value = &GPIO2->SWPORTA_DDR;

        /* Current_mode = current value in SWPORTA_DDR reg */
        uint32_t current_mode = dev_mem_read((unsigned long)swporta_ddr_value);

        /* Need to reset bit */
        current_mode &= ~(HIGH);
        
        debug("Reset gpio: update val %u\n", current_mode);

        /* Write mode back into register */
        dev_mem_write((unsigned long)swporta_ddr_value, current_mode);
    }

    /*
    *   Function that puts Milk back to a default state
    */
    static void handle_sigint(int sig)
    {   
        debug("Interupt caught: %d\n", sig);
        freeResources();

        exit(0);
    }

    /*
    * dit_blink() -> Blink logic for a dit     
    *
    * @INPUTS
    *   VOID
    *
    * @RETURNS:
    *   VOID
    *
    */
    static void dit_blink(volatile uint32_t* reg){
        
        /* ON */
        dev_mem_write((unsigned long)reg, HIGH);
        debug("\nTURNING ON\n");
        sleep(1);

        /* OFF */
        dev_mem_write((unsigned long)reg, LOW);
        debug("\nTURNING OFF\n");
    }

    /*
    * dit_blink() -> Blink logic for a dit     
    *
    * @INPUTS
    *   VOID
    *
    * @RETURNS:
    *   VOID
    *
    */
    static void dot_blink(volatile uint32_t* reg){

        /* ON */
        dev_mem_write((unsigned long)reg, HIGH);
        debug("TURNING ON\n");
        
        sleep(3);

        /* OFF */
        dev_mem_write((unsigned long)reg, LOW);
        debug("TURNING OFF\n");
    }

#endif /* HOST */

/**
* blink_led() -> Translates a phrase to morse code
* 
* @input:
*    - Morse Code char array
*
* @param:
*    VOID => Array elements are replaced in-place
*
*   TODO: ADD IRQ FOR NEW ENGLISH PHRASE???
*/

#define CALL_AMOUNT     0
#define SLEEP_SECONDS   1

extern void blink_led(uint8_t* code, uint32_t sleep_stats[]){
    
    #ifdef MORSE_DEBUG
        printf("Blink_led recieved: ");
        uint32_t codeLength = 0;
        while(code[codeLength] != '\0'){
            printf("%c", code[codeLength]);
            codeLength += 1;
        }
        printf("\n");
    #endif

    #ifndef HOST
        signal(SIGINT, handle_sigint);
        setGPIOMode();  
        volatile uint32_t* swporta_dr_value = &GPIO2->SWPORTA_DR;
    #endif /* HOST */

    while(*code != '\0'){
        
        debug("\nCurrent Morse: %c\n", *code);

        switch(*code){

            /* DIT Blink Logic */
            case('.'):
                debug("ON: 1 Time Unit\n");
                sleep_stats[CALL_AMOUNT]++;
                sleep_stats[SLEEP_SECONDS]++;

                #ifndef HOST
                    dit_blink(swporta_dr_value);
                #endif /* HOST */

                /* Too tired to explain */
                if(*(code + 1) == ' ' || *(code + 1) == '\0'){
                    break;
                } else {
                    debug("OFF: 1 Time Unit\n");
                    
                    #ifndef HOST
                        sleep(1);
                    #endif /*HOST*/
                    
                    sleep_stats[CALL_AMOUNT]++;
                    sleep_stats[SLEEP_SECONDS]++;
                }
                break;

            /* DOT Blink Logic */
            case('-'):
                debug("ON: 3 Time Unit\n");
                sleep_stats[CALL_AMOUNT]++;
                sleep_stats[SLEEP_SECONDS] += 3;

                #ifndef HOST
                    dot_blink(swporta_dr_value);
                #endif /* HOST */
                
                if(*(code + 1) == ' ' || *(code + 1) == '\0'){
                    break;
                } else {
                    debug("OFF: 1 Time Unit\n");

                    #ifndef HOST
                        sleep(1);
                    #endif /*HOST*/
                    
                    sleep_stats[CALL_AMOUNT]++;
                    sleep_stats[SLEEP_SECONDS]++;
                }

                break;

            /* WORD SPACE LOGIC */
            case('/'):

                if(*(code + 1) == '\0'){
                    break;
                }

                debug("WORD SPACE: 7 Time Unit\n");
                sleep_stats[CALL_AMOUNT]++;
                sleep_stats[SLEEP_SECONDS] += 7;                
                
                #ifndef HOST
                    sleep(7);
                #endif

                break;

            /* LETTER SPACE LOGIC */
            case(' '):

                if(*(code + 1) == '/' || *(code - 1) == '/' || *(code + 1) == '\0'){
                    break;
                }

                debug("LETTER SPACE: 3 Time Unit\n");
                sleep_stats[CALL_AMOUNT]++;
                sleep_stats[SLEEP_SECONDS] += 3;

                #ifndef HOST
                    sleep(3);
                #endif
                
                break;

            default:
                break;
        }
        code++;
    }

    debug("Sleep Call Amount: %u\nSleep Total Time: %u\n",
        sleep_stats[CALL_AMOUNT], sleep_stats[SLEEP_SECONDS]);
}