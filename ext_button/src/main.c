#include "ch.h"
#include "hal.h"
#include "callbacks.h"

int main(void) {

    halInit();
    chSysInit();
    palSetPadMode(GPIOA,GPIOA_LED_GREEN,PAL_MODE_OUTPUT_PUSHPULL);
    palEnablePadEventI(GPIOB,GPIOB_ARD_D4,PAL_EVENT_MODE_FALLING_EDGE);
    palSetPadCallback(GPIOB, GPIOB_ARD_D4, buttonCallback, NULL);
    while (1) {

        chThdSleepMilliseconds(1);

    }
}
