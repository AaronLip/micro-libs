#pragma ONCE

int debug_assert(int condition, const char* failureMessage);
void blocking_assert(int condition, const char* failureMessage);

void info_out(const char* message);