#include "ch.h"
#include "hal.h"

void buttonCallback(void *arg){

	(void)arg;
	palToggleLine(LINE_LED_GREEN);

}
