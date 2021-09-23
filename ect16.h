#define ECT_ENABLE TSCR1 |= 0x80;
#define ECT_DISABLE TSCR1 &= (~0x80);

volatile void ect_init() {

    return;
}

#undef ECT_ENABLE
#undef ECT_DISABLE