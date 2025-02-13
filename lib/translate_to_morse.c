#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "translate_to_morse.h"

/*
* isAlphaNumberic -> Determines if a character is in the English alphabet or a number 0-9
*
* @INPUTS:
*   A character... duh
*
* @RETURNS:
*   1: Character is in the alphabet | a number
*   0: Character is NOT in the alphabet | a number
*/

static uint32_t isAlphaNumeric(const uint8_t* c){
    //ASCII stuff, "It just works" - Todd Howard
    if( (*c >= 'A' && *c <= 'Z') ||
        (*c >= 'a' && *c <= 'z') ||
        (*c >= '0' && *c <= '9')) 
        {
            return 1;
        }

        return 0;
}

/*
* toLowercase -> To avoid importing an entire lib for tolower, I have made this.
*   Simply alters a character to be lowercase
* 
* @INPUTS:
*   A character
*       - an alphabetical character
*       - HAS BEEN CHECKED ALREADY
*
* @RETURNS:
*   VOID => Alters character in place to avoid passing by copy
*/

static void toLowercase(uint8_t* c){
    //Check if upper
    if(*c >= 'A' && *c <= 'Z'){
        *c += 32;
    }
}

/*
* characterToIndexHash() -> calculates an index representing the position of a character's Morse code in an array
*
* @INPUTS:
*   Can you guess? That's right! A frog!
*   THIS FROG CAN NOT BE CHANGED
*   FROG HAS BEEN CHECKED AND IS A VALID FROG. 
*   It's name is most likely kermit.
*
* @RETURNS:
*   HASH_ERROR => Default 254. Hashing error.
*   An int between 0 and 35 that corresponds with the character's morse code index in the array.
*
*
* TODO:
*   Figure out a new ret value for hash error.
*   254 should be impossible to reach but I have been wrong before.
*/
static uint8_t characterToIndexHash(const uint8_t* c){
    if(*c >= 'a' && *c <= 'z'){
        /*Maps  a => 0
        *       b => 1
        *       c => 2
        * *     ...
        */
        return (*c - 97);
    } else if (*c >= '0' && *c <= '9'){
        /*
            Numbers start 26 characters in, so a buffer of 26 is added
                '0' - '0' + 26 = 26
                '1' - '0' + 26 = 27
                ...

                26th index of morseCode => '0' in morse code
        */
        return (*c - '0' + 26);
    } else {
        //Dead code. Should never get here since c is already checked, but prelim caution just in-case
        return HASH_ERROR;
    }
}


/*
* charToTranslate() -> Translates a character (usually passed by a helper function) to morse code
* 
* @INPUTS:
*    A character... any character.  
*
* @RETURNS:
*    NULL => Hashing Error   
*    Non-empty string => For when character passed is a valid character
*    Empty String => For when character passed is not a valid character
*/

static uint8_t* charToTranslate(uint8_t* c){

    static uint8_t* morseCode[37] = {
        /*A*/   ".-",   
        /*B*/   "-...",
        /*C*/   "-.-.", 
        /*D*/   "-..",
        /*E*/   ".",
        /*F*/   "..-.",
        /*G*/   "--.",
        /*H*/   "....",
        /*I*/   "..",
        /*J*/   ".---",
        /*K*/   "-.-",
        /*L*/   ".-..",
        /*M*/   "--",
        /*N*/   "-.",
        /*O*/   "---",
        /*P*/   ".--.",
        /*Q*/   "--.-",
        /*R*/   ".-.",
        /*S*/   "...",
        /*T*/   "-",
        /*U*/   "..-",
        /*V*/   "...-",
        /*W*/   ".--",
        /*X*/   "-..-",
        /*Y*/   "-.--",
        /*Z*/   "--..",
        
        /*0*/   "-----",
        /*1*/   ".----",
        /*2*/   "..---",
        /*3*/   "...--",
        /*4*/   "....-",
        /*5*/   ".....",
        /*6*/   "-....",
        /*7*/   "--...",
        /*8*/   "---..",
        /*9*/   "----.",

        /* */   "/",
    };

    //Handles spaces
    if(*c == 32){
        debug("%c in morse code: %s\n", *c, morseCode[36]);
        return morseCode[36];
    }

    //Checks if character passed is valid
    if(!isAlphaNumeric(c)){
        
        debug("\n%c: Character is not alphabetical nor a number 0-9.\n", *c);

        return "";
    }

    debug("   Before: %c\n", *c);

    //Converts the character to lowercase
    toLowercase(c);

    debug("   After: %c\n", *c);

    uint8_t character_index = characterToIndexHash(c);

    debug("   %c index in morseCode: %d\n", *c, character_index);

    debug("   %c in morse code: %s\n", *c, morseCode[character_index]);

    //Error check
    if(character_index == HASH_ERROR){
        fprintf(stderr, "Error hashing character.\n");
        return NULL; 
    }

    return morseCode[character_index];
}

/*
* createNewCharacterNode() -> Creates a new node for the list
* 
* @INPUTS:
*    VOID
*
* @RETURNS:
*   NULL => Memory error
*   ptr to node => Successful
*/
static CharacterList* createNewCharacterNode(void){
    CharacterList* new_node = calloc(1, sizeof(*new_node));

    if(!new_node){
        fprintf(stderr, "Memory allocation for new_node.\n");
        return NULL;
    }
    
    return new_node; 
}

/*
* freeMorseCodeList() -> Frees a given list TAIL
* 
* @INPUTS:
*    -A linked-list TAIL
*
* @RETURNS:
*   1 => Passed a NULL pointer
*   0 => Successful
*/

static int32_t freeMorseCodeList(CharacterList* tail){
    if(tail == NULL){
        fprintf(stderr, "You passed a NULL head. RIP.\n");
        return 1;
    }

    while(tail != NULL){
        CharacterList* delete = tail;
        tail = tail->prev;
        free(delete);
    }

    return 0;
}


/*
* phraseToTranslateHelper() -> Translates a phrase to morse code
* 
* @INPUTS:
*    -A phrase from phraseToTranslate
*
* @RETURNS:
*   Head of a linked-list => For when phrase passed is valid
*   NULL => Either Memory alloc or hash error
*/

static CharacterList* phraseToTranslateHelper(uint8_t* phrase){
    CharacterList* head = NULL;
    CharacterList* current = NULL;

    while(*phrase != '\0'){
        debug("\nCURRENT PHRASE: %c\n", *phrase);

        CharacterList* new_character = createNewCharacterNode();
        
        /* Recursive NULL check BECAUSE C DOESN'T SUPPORT EXECEPTIONS FOR SOME REASON */
        if(!new_character){
            return NULL;
        }

        uint8_t* character_in_morse = charToTranslate(phrase);

        /* ANOTHER RECURSIVE NULL RETURN FOR HASHING */
        if(!character_in_morse){
            return NULL;
        }

        new_character->character = *phrase;
        new_character->morse_translation = character_in_morse;

        if(head == NULL){
            head = new_character;
            head->prev = NULL;
            current = new_character;
        } else {
            current->next = new_character;
            new_character->prev = current;
            current = current->next;
        }
        
        phrase++;
    }
    return head;
}

/*
* phraseToTranslate() -> Translates a phrase to morse code
* 
* @INPUTS:
*    -A phrase from main
*
* @RETURNS:
*    NULL => Hashing Error | Memory Alloc err
*    Non-empty string   => For when phrase passed is valid
* 
*   Designing this so each character is a node in a linked list.
*   Why? Because I thought the project was too eas- fun!
*   I wanted to get around the buffer limit really.
*   But there are other ways of doing this? Yeah... but do they involve linked lists?
*   Yes? Well, shut up okay.
*/

extern uint8_t* phraseToTranslate(uint8_t* phrase){
    //Counts phrase length
    uint32_t phrase_length = 0;
    
    while(phrase[phrase_length] != '\0'){
            phrase_length += 1;
    }

    if(!phrase_length){
        debug("Passed empty string.\n");
        return "";
    }

    CharacterList* head = phraseToTranslateHelper(phrase);

    /* Recursive NULL check BECAUSE C DOESN'T SUPPORT EXCEPTIONS FOR SOME REASON */
    if(!head){
        //Either hashing or mem allocation. stderr will display which
        return NULL;
    }

    debug("\nEnglish Phrase Length: %d\n", phrase_length);

    uint32_t amount_of_spaces = phrase_length;
    phrase_length *= 5; /* Worse case is each character is a number */
    phrase_length += amount_of_spaces * 3; /* Extra amount needed for spaces */
    phrase_length += 1; /* For the NULL */

    debug("Allocating %d characters for max morse code length.\n", phrase_length);
    //Allocate memory for what we will be returning
    uint8_t* ret = calloc(phrase_length, sizeof(*ret));
    uint32_t ret_size = 0;

    if(!ret){
        fprintf(stderr, "Error: Memory alloc for ret.\n");
        return NULL;
    }

    //Iterate through each node
    uint32_t seen_first_alpha = 0;

    while(1){    
        
        /* If not a hashing error */
        if(head->morse_translation != NULL){
            /* Tracks current dit, dot, or */
            uint32_t cur_morse_len = 0;

            /* 
            * Omits all beginning spaces
            */

            if(head->character == ' ' && !seen_first_alpha){
                goto next;
            } else {
                seen_first_alpha = 1;
            }
            
            /*
            *   Omits all ending spaces
            */

            if(head->character == ' '){
                uint32_t only_spaces = 0;
                
                CharacterList* trav = head;
                /* Once we come across a space, see if there is only spaces left. */
                while(trav != NULL){
                    if(trav->character != ' '){
                        only_spaces = 1;
                        break;
                    } else {
                        trav = trav->next;
                    }
                }

                if(!only_spaces){
                    /* A goto? *shivers* */
                    goto next;
                }
            }

            /* Copy the dit, dot, or / to the returning string */
            while(head->morse_translation[cur_morse_len] != '\0'){
                ret[ret_size++] = head->morse_translation[cur_morse_len++];
            }

            /* Add a space between each morse code word */
            ret[ret_size++] = ' ';
        } else {
            return "";
        }

        next: 
            if(head->next == NULL){
                ret[ret_size - 1] = '\0';
                break;
            } else {
                head = head->next;
            }
    }

    debug("Reallocing %d bytes -> %d bytes\n", phrase_length, ret_size - 1);

    ret = realloc(ret, sizeof(*ret) * (ret_size - 1));

    #ifdef MORSE_DEBUG
        printf("Returning: %d characters.\n", ret_size);
    #endif /* MORSE_DEBUG */

    /* Free them resources. Hope this doesn't return anything*/
    if(freeMorseCodeList(head)){
        fprintf(stderr, "Error: Free.\n");
    }

    /* Return string */
    return ret;

}