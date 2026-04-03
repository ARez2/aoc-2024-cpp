int test = 1;

#ifdef DEBUG
#define DBG
#else
#define DBG for (; 0;)
#endif