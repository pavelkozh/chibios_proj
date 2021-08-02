#include "ch.h"
#include "hal.h"
#include "functions.h"

/*
 * Application entry point.
 */

static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {
  (void)arg;
  chRegSetThreadName("blinker");
  while (1) {

	  blinkingGreenLED();
  }
}

static PWMDriver *pwmDriver= &PWMD1;

static const PWMConfig pwmcfg = {
  1000000,                                    /* 10kHz PWM clock frequency.     */
  1000,                                    /* Initial PWM period 1S.         */
  NULL,                                     /* Period callback.               */
  {
   {PWM_OUTPUT_ACTIVE_HIGH | PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH, NULL},          /* CH1 mode and callback.         */
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},             /* CH2 mode and callback.         */
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},             /* CH3 mode and callback.         */
   {PWM_OUTPUT_ACTIVE_HIGH, NULL}              /* CH4 mode and callback.         */
  },
  0,                                        /* Control Register 2.            */
  0                                         /* DMA/Interrupt Enable Register. */
};

int main(void) {

  halInit();
  chSysInit();
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);
  //palSetGroupMode(GPIOA,0x0F80,0,PAL_MODE_ALTERNATE(1));
  palSetPadMode(GPIOA, GPIOA_ARD_D7,PAL_MODE_ALTERNATE(1));//CH1
  palSetPadMode(GPIOA, GPIOA_ARD_D8,PAL_MODE_ALTERNATE(1));//CH2
  palSetPadMode(GPIOA, GPIOA_ARD_D2,PAL_MODE_ALTERNATE(1));//CH3
  palSetPadMode(GPIOA, 11U ,PAL_MODE_ALTERNATE(1));//CH4
  palSetPadMode(GPIOA, GPIOA_ARD_D11,PAL_MODE_ALTERNATE(1));//CH1/1N (complementary channel)

  palSetPadMode(GPIOA, GPIOA_LED_GREEN,PAL_MODE_OUTPUT_PUSHPULL);
  pwmStart(pwmDriver, &pwmcfg);

  uint32_t count=0;
  uint8_t i=0;
  uint8_t array[4][2]={0,1,1,2,2,3,3,0};
  while (1) {

      pwmEnableChannel(pwmDriver, array[i][0], PWM_PERCENTAGE_TO_WIDTH(pwmDriver, 10000-count));   // duty cycle
      pwmEnableChannel(pwmDriver, array[i][1], PWM_PERCENTAGE_TO_WIDTH(pwmDriver, count));
      if (count<10000)  count+=10;
      else {

          count=0;
          i++;
          if (i > 3) i=0;

      }
      chThdSleepMilliseconds(1);
  }

}
