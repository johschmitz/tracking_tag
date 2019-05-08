#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define debug_print(...) printf(__VA_ARGS__)
#else
#define debug_print(...) do {} while (0)
#endif

#endif /* DEBUG_H */