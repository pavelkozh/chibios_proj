#include "ch.h"
#include "hal.h"
#include <string.h>

#define MPU6050_ADDR 0x69 //pin AD0 = 1

static I2CDriver *i2cDriver = &I2CD1;
static SerialDriver *SDdriver = &SD2;

static const I2CConfig i2cconfig = {

		OPMODE_I2C,			//i2c mode
		400000,				//400 kHz
		FAST_DUTY_CYCLE_2,  //FAST_DUTY_CYCLE_16_9

};

static const SerialConfig sdconf = {
  .speed = SERIAL_DEFAULT_BITRATE,//default speed = 38400
  .cr1 = 0,
  .cr2 = USART_CR2_STOP_1,
  .cr3 = 0
};


uint8_t transmitt_buf[1]={0x75};
uint8_t receive_buf[1]={0};

int main(void) {


    halInit();
    chSysInit();

    palSetPadMode(GPIOB,GPIOB_PIN7,PAL_MODE_ALTERNATE(4));//SDA PB7
    palSetPadMode(GPIOB,GPIOB_ARD_D10,PAL_MODE_ALTERNATE(4));//SCL PB6
	palSetPadMode(GPIOA,GPIOA_LED_GREEN,PAL_MODE_OUTPUT_PUSHPULL);
	palClearLine(LINE_LED_GREEN);

    sdStart(SDdriver, &sdconf);
    i2cStart(i2cDriver, &i2cconfig);

    i2cflags_t flag=255;

    msg_t mes = i2cMasterTransmitTimeout(i2cDriver, MPU6050_ADDR, transmitt_buf, 1, receive_buf, 1, TIME_INFINITE);
    sdPut(SDdriver,mes+0x30);
    sdPut(SDdriver,' ');
    flag=i2cGetErrors(i2cDriver);

    sdPut(SDdriver,flag+0x30);
    sdPut(SDdriver,'\n');
    sdPut(SDdriver,'\r');
    if (receive_buf[0] == 0x68)  palSetLine(LINE_LED_GREEN);

    while (1) {

    	//palTogglePad(GPIOA,GPIOA_LED_GREEN);
        chThdSleepMilliseconds(500);

    }
}

