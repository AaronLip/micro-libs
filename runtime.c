#include <hidef.h>
#include "derivative.h"

#include "runtime.h"
#include "sci.h"

#include <string.h>

static int INITIALIZED = 0;

int debug_assert(int condition, const char* failureMessage) {
    if (INITIALIZED) {
        (void) SCI0_Init(20E6, 9600);
        INITIALIZED = 1;
    }
    if (!condition) {
        SCI0_WriteString("[ASSERTION WARNING] ");
        SCI0_WriteString(failureMessage);
    }
    
    return condition;
}

void blocking_assert(int condition, const char* failureMessage) {
    if (condition) return;

    SCI0_WriteString("[ASSERTION ERROR] ");
    SCI0_WriteString(failureMessage);
    HALT;
}

void info_out(const char* message) {
    SCI0_WriteString("[INFO] ");
    SCI0_WriteString(message);
}