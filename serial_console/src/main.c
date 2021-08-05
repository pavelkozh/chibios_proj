#include "ch.h"
#include "hal.h"
#include <string.h>

uint8_t text_buff[50]={};
uint8_t* ptr=text_buff;

static SerialDriver *SDdriver = &SD2;

static const SerialConfig sdconf = {
  .speed = SERIAL_DEFAULT_BITRATE,//default speed = 38400
  .cr1 = 0,
  .cr2 = USART_CR2_STOP_1,
  .cr3 = 0
};

static THD_WORKING_AREA(waReadText, 128);
static THD_FUNCTION(readText, arg) {

  (void)arg;
  while (1)
  {
	sdRead(SDdriver,ptr,1);
	sdWrite(SDdriver,ptr,1);
	chThdSleepMilliseconds(1);

  }
}

int main(void) {

	int8_t* str_pointer="Enter symbol:\r\n";

    halInit();
    chSysInit();
    sdInit();
    sdStart(SDdriver,&sdconf);
    chThdCreateStatic(waReadText, sizeof(waReadText), NORMALPRIO+1, readText, NULL);
    palSetPadMode(GPIOA,GPIOA_LED_GREEN,PAL_MODE_OUTPUT_PUSHPULL);
    sdWrite(SDdriver,str_pointer,strlen(str_pointer));

    while (1) {

    	palToggleLine(LINE_LED_GREEN);
        chThdSleepMilliseconds(500);

    }
}
