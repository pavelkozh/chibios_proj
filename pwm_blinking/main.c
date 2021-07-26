/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include <hal_pwm_lld.h>

/*
 * Application entry point.
 */
/*
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {
  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palClearPad(GPIOA, GPIOA_LED_GREEN);
    chThdSleepMilliseconds(500);
    palSetPad(GPIOA, GPIOA_LED_GREEN);
    chThdSleepMilliseconds(500);
  }
}*/

static PWMDriver *pwmDriver= &PWMD1;

static const PWMConfig pwmcfg = {
  1000000,                                    /* 10kHz PWM clock frequency.     */
  1000,                                    /* Initial PWM period 1S.         */
  NULL,                                     /* Period callback.               */
  {
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},          /* CH1 mode and callback.         */
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
  //chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);
  palSetGroupMode(GPIOA,0x0F00,0,PAL_MODE_ALTERNATE(1));
  pwmStart(&PWMD1, &pwmcfg);

  uint32_t count=0;
  uint8_t i=0;
  uint8_t array[4][2]={0,1,1,2,2,3,3,0};
  while (true) {

      pwmEnableChannel(&PWMD1, array[i][0], PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 10000-count));   // duty cycle
      pwmEnableChannel(&PWMD1, array[i][1], PWM_PERCENTAGE_TO_WIDTH(&PWMD1, count));
      if (count<10000)  count+=10;
      else {

          count=0;
          i++;
          if (i > 3) i=0;

      }
      chThdSleepMilliseconds(1);
  }

}
