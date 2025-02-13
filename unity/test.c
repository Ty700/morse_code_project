#include "unity.h"
#include "translate_to_morse.h"
#include "blink_led.h"

void setUp() {
    // set stuff up here
}

void tearDown() {  
}

/* Alphabet only test */
void test_AlphabeticInput(void) {
    uint8_t phrase[] = "Hello";
    uint8_t* output = phraseToTranslate(phrase);

    uint8_t* expected = ".... . .-.. .-.. ---";

    TEST_ASSERT_EQUAL_STRING(expected, output);
}

/* Test function for numeric input */
void test_NumericInput(void) {
    uint8_t phrase[] = "123";
    uint8_t* output = phraseToTranslate(phrase);
    
    // 123
    char* expected = ".---- ..--- ...--";

    TEST_ASSERT_EQUAL_STRING(expected, output);
}

/* Test function for mixed alphanumeric input */
void test_MixedAlphanumericInput(void)
{
    uint8_t phrase[] = "abc123";
    uint8_t* output = phraseToTranslate(phrase);

    // abc123
    char *expected = ".- -... -.-. .---- ..--- ...--";

    TEST_ASSERT_EQUAL_STRING(expected, output);
}

/* Test function for mixed case input */
void test_MixedCaseAlphanumericInput(void) {
    uint8_t phrase[] = "AbC123";
    uint8_t* output = phraseToTranslate(phrase);

    char *expected = ".- -... -.-. .---- ..--- ...--";

    TEST_ASSERT_EQUAL_STRING(expected, output);
}

/* Test function for words with spaces */
void test_CanHandleSpaces(void) {   
    uint8_t phrase[] = "Hello World";
    uint8_t* output = phraseToTranslate(phrase);

    char *expected = ".... . .-.. .-.. --- / .-- --- .-. .-.. -..";

    TEST_ASSERT_EQUAL_STRING(expected, output);
}

/* Test function for non-representable binary data */
void test_NonRepresentableBinaryData(void) {
    uint8_t phrase[] = {0x00, 0xFF, 0x55, 0x7F}; // Example binary data
    uint8_t* output = phraseToTranslate(phrase);

    TEST_ASSERT_EQUAL_STRING("", output);
}

/* Time Test */
void test_MorseTiming(void) {   
    uint8_t phrase[] = "SOS";
    uint32_t sleep_stats[2] = {0, 0};

    uint8_t* translation = phraseToTranslate(phrase); 
    blink_led(translation, sleep_stats);

    // Expected values
    int expected_delays = 17;
    int expected_delay_time = 27;

    // Assert the number of delay calls
    TEST_ASSERT_EQUAL(expected_delays, sleep_stats[0]);

    // Assert the total duration of delays
    TEST_ASSERT_EQUAL(expected_delay_time, sleep_stats[1]);
}

/*****************************
 * 
 * Decided to make multiple edge case tests.
 * 
 * Might have missed some possibilites for edge cases.
 * 
 * If so lmk and I can add them
 * 
 * ***************************
*/

void test_AlphabetEdgeCase(void){

    uint8_t phrase[] = "HEKASFASGIOAGNJIOASJGOIAJGIOAJVAIOSGJGOIAJGIOAJAIGOJ";

    uint8_t* output = phraseToTranslate(phrase);

    uint8_t* expected = ".... . -.- .- ... ..-. .- ... --. .. --- .- --. -. .--- .. --- .- ... .--- --. --- .. .- .--- --. .. --- .- .--- ...- .- .. --- ... --. .--- --. --- .. .- .--- --. .. --- .- .--- .- .. --. --- .---";

    TEST_ASSERT_EQUAL_STRING(expected, output);

}

void test_AlphabetAndSpacesEdgeCase(void){

    uint8_t phrase[] = "                 H E K A S F A S G I O A G N J I O A S J G O I A J G I O A J V A I O S G J G O I A J G I O A J A I G O J ";

    uint8_t* output = phraseToTranslate(phrase);

    uint8_t* expected = ".... / . / -.- / .- / ... / ..-. / .- / ... / --. / .. / --- / .- / --. / -. / .--- / .. / --- / .- / ... / .--- / --. / --- / .. / .- / .--- / --. / .. / --- / .- / .--- / ...- / .- / .. / --- / ... / --. / .--- / --. / --- / .. / .- / .--- / --. / .. / --- / .- / .--- / .- / .. / --. / --- / .---";

    TEST_ASSERT_EQUAL_STRING(expected, output);

}

void test_NumberEdgeCase(void){

    uint8_t phrase[] = "0123456789109876543210";

    uint8_t* output = phraseToTranslate(phrase);

    uint8_t* expected = "----- .---- ..--- ...-- ....- ..... -.... --... ---.. ----. .---- ----- ----. ---.. --... -.... ..... ....- ...-- ..--- .---- -----";

    TEST_ASSERT_EQUAL_STRING(expected, output);

}

void test_NumberAndSpacesEdgeCase(void){

    uint8_t phrase[] = "0 1 2 3 4 5 6 7 8 9 9 8 7 6 5 4 3 2 1 0";

    uint8_t* output = phraseToTranslate(phrase);

    uint8_t* expected = "----- / .---- / ..--- / ...-- / ....- / ..... / -.... / --... / ---.. / ----. / ----. / ---.. / --... / -.... / ..... / ....- / ...-- / ..--- / .---- / -----";

    TEST_ASSERT_EQUAL_STRING(expected, output);

}

void test_AlphaAndNumberEdgeCase(void){

    uint8_t phrase[] = "HEKASFASGIOAGNJIOASJGOIAJ0123456789109876543210";

    uint8_t* output = phraseToTranslate(phrase);

    uint8_t* expected = ".... . -.- .- ... ..-. .- ... --. .. --- .- --. -. .--- .. --- .- ... .--- --. --- .. .- .--- ----- .---- ..--- ...-- ....- ..... -.... --... ---.. ----. .---- ----- ----. ---.. --... -.... ..... ....- ...-- ..--- .---- -----";

    TEST_ASSERT_EQUAL_STRING(expected, output);

}

void test_AlphaAndNumberAndSpacesEdgeCase(void){

    uint8_t phrase[] = "H E K A S F A S G I O A G N J I O A S J G O I A J 0 1 2 3 4 5 6 7 8 9 1 0 9 8 7 6 5 4 3 2 1 0             ";

    uint8_t* output = phraseToTranslate(phrase);

    uint8_t* expected = ".... / . / -.- / .- / ... / ..-. / .- / ... / --. / .. / --- / .- / --. / -. / .--- / .. / --- / .- / ... / .--- / --. / --- / .. / .- / .--- / ----- / .---- / ..--- / ...-- / ....- / ..... / -.... / --... / ---.. / ----. / .---- / ----- / ----. / ---.. / --... / -.... / ..... / ....- / ...-- / ..--- / .---- / -----";

    TEST_ASSERT_EQUAL_STRING(expected, output);

}

void test_LeadingSpacesTest(void){
    uint8_t phrase[] = "         Hello";

    uint8_t* output = phraseToTranslate(phrase);

    uint8_t* expected = ".... . .-.. .-.. ---";

    TEST_ASSERT_EQUAL_STRING(expected, output);
}

void test_TrailingSpacesTest(void){
    uint8_t phrase[] = "Hello         ";

    uint8_t* output = phraseToTranslate(phrase);

    uint8_t* expected = ".... . .-.. .-.. ---";

    TEST_ASSERT_EQUAL_STRING(expected, output);
}

void test_LeadingAndTrailingSpacesTest(void){
    uint8_t phrase[] = "                  Hello         ";

    uint8_t* output = phraseToTranslate(phrase);

    uint8_t* expected = ".... . .-.. .-.. ---";

    TEST_ASSERT_EQUAL_STRING(expected, output);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_AlphabeticInput);
    RUN_TEST(test_NumericInput);
    RUN_TEST(test_MixedAlphanumericInput);
    RUN_TEST(test_CanHandleSpaces);
    RUN_TEST(test_MixedCaseAlphanumericInput);
    RUN_TEST(test_NonRepresentableBinaryData);
    RUN_TEST(test_MorseTiming);
    RUN_TEST(test_AlphabetEdgeCase);
    RUN_TEST(test_AlphabetAndSpacesEdgeCase);
    RUN_TEST(test_NumberEdgeCase);
    RUN_TEST(test_NumberAndSpacesEdgeCase);
    RUN_TEST(test_AlphaAndNumberEdgeCase);
    RUN_TEST(test_AlphaAndNumberAndSpacesEdgeCase);
    RUN_TEST(test_LeadingSpacesTest);
    RUN_TEST(test_TrailingSpacesTest);
    RUN_TEST(test_LeadingAndTrailingSpacesTest);
    UNITY_END();
}