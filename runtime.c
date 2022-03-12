#include <hidef.h>
#include "derivative.h"

#include "runtime.h"
#include "sci.h"

#include <string.h>

static int ENABLE_INFO = 0;
static int INITIALIZED = 0;

int debug_assert(int condition, const char* failureMessage) {
    if (INITIALIZED) {
        //(void) SCI0_Init(20E6, 9600);
        INITIALIZED = 1;
    }
    if (!condition) SCI0_WriteString(failureMessage);
    
    return condition;
}

void blocking_assert(int condition, const char* failureMessage) {
    if (condition) return;

    SCI0_WriteString(failureMessage);
    HALT;
}

void info_out(const char* message) {
    if (!ENABLE_INFO) return;

    {
        char output[0xff] = "[INFO] ";
        (void) strcat(output, message);  // consider strncat or strcat_s
        SCI0_WriteString(output);
    }
}