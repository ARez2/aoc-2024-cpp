#pragma once

#ifdef DEBUG
#define DBG
#else
#define DBG for (; 0;)
#endif