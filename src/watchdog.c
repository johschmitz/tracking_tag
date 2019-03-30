#include "watchdog.h"
#include "stm8s.h"

void iwdg_enable() {
    /* enable IWDG */
    IWDG_KR = IWDG_KEY_ENABLE;

    /* enable write access */
    IWDG_KR = IWDG_KEY_ACCESS;

    /* prescaler = 256 */
    IWDG_PR |= 6;

    /* timeout = 1.02s */
    IWDG_RLR = 0xFF;

    /* reload watchdog counter */
    IWDG_KR = IWDG_KEY_REFRESH;
}

inline void iwdg_refresh() {
    IWDG_KR = IWDG_KEY_REFRESH;
}