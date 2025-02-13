set(TRANSLIB_SRC 
    "translate_to_morse.c"
    "devmem.c"
    "blink_led.c")

set(TRANSLIB_HEADER
    "translate_to_morse.h"
    "cv1800b.h"
    "devmem.h"
    "blink_led.h"
    )

add_library(${TRANSLATELIB} STATIC ${TRANSLIB_SRC} ${TRANSLIB_HEADER})

target_include_directories(${TRANSLATELIB} PUBLIC "./")