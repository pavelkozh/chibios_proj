#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <string.h>

#define MPU6050_ADDR 0x69 //pin AD0 = 1
#define GYRO_ADDR 0x6B //pin SDO = 1

static const SerialConfig sd_st_cfg = {
  .speed = 38400,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};

static SerialDriver            *debug_serial = &SD2;
static BaseSequentialStream *debug_stream = NULL;

void debug_stream_init( void )
{
    sdStart( debug_serial, &sd_st_cfg );
    palSetPadMode( GPIOA, 9, PAL_MODE_ALTERNATE(7) );
    palSetPadMode( GPIOA, 10, PAL_MODE_ALTERNATE(7) );

    debug_stream = (BaseSequentialStream* )debug_serial;
}

void dbgprintf( const char* format, ... )
{
    if ( !debug_stream )
        return;

    va_list ap;
    va_start(ap, format);
    chvprintf(debug_stream, format, ap);
    va_end(ap);
}


static I2CDriver *i2cDriver = &I2CD1;
//static SerialDriver *SDdriver = &SD2;

static const I2CConfig i2cconfig = {

		OPMODE_I2C,			//i2c mode
		400000,				//400 kHz
		FAST_DUTY_CYCLE_16_9  //

};

/*static const SerialConfig sdconf = {
  .speed = SERIAL_DEFAULT_BITRATE,//default speed = 38400
  .cr1 = 0,
  .cr2 = USART_CR2_STOP_1,
  .cr3 = 0
};*/


uint8_t init_array[2]={0x20,0x0F};//normal mode
uint8_t xyz_reg[1]={0x28|(1<<7)};//auto increment mode

uint8_t output_data[6]={};
uint16_t xyz_data[3]={};

int main(void) {


    halInit();
    chSysInit();

    palSetPadMode(GPIOB,GPIOB_PIN7,PAL_MODE_ALTERNATE(4));//SDA
    palSetPadMode(GPIOB,GPIOB_ARD_D10,PAL_MODE_ALTERNATE(4));//SCL

	debug_stream_init();

	dbgprintf("Start\r\n");

    i2cStart(i2cDriver, &i2cconfig);

    //sensor init
    i2cMasterTransmitTimeout(i2cDriver, GYRO_ADDR, init_array, 2, NULL, 0, TIME_INFINITE);

    while (1) {

    	i2cMasterTransmitTimeout(i2cDriver, GYRO_ADDR, xyz_reg, 1, output_data, 6, TIME_INFINITE);
    	xyz_data[0] = output_data[0] | (output_data[1] << 8 );// x data
    	xyz_data[1] = output_data[2] | (output_data[3] << 8 );// y data
    	xyz_data[2] = output_data[4] | (output_data[5] << 8 );// z data
        dbgprintf("x = %d, y = %d, z = %d\r\n",
        		xyz_data[0],xyz_data[1],xyz_data[2]);
        chThdSleepMilliseconds(1000);

    }
}

