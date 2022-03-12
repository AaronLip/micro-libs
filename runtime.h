#ifndef _RUNTIME_H_
#define _RUNTIME_H_

int debug_assert(int condition, const char* failureMessage);
void blocking_assert(int condition, const char* failureMessage);

void info_out(const char* message);

#endif