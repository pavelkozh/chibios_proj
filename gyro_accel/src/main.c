#include "ch.h"
#include "hal.h"
#include <chprintf.h>
#include <string.h>

#define MPU6050_ADDR 0x69 //pin AD0 = 1
#define GYRO_ADDR 0x6B //pin SDO = 1
#define ACCEL_ADDR 0x18 //pin SDO = 0

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


uint8_t gyro_init[2]={0x20,0x0F};//normal mode, 250dps
uint8_t accel_init[2]={0x20,0x77};//normal mode, 400 Hz, +-2g
uint8_t xyz_reg[1]={0x28|(1<<7)};//auto increment mode
uint16_t gyro_data[3]={};
uint16_t accel_data[3]={};
float gyro_dps[3];
float accel_g[3];

int main(void) {


    halInit();
    chSysInit();

    palSetPadMode(GPIOB,GPIOB_PIN7,PAL_MODE_ALTERNATE(4));//SDA
    palSetPadMode(GPIOB,GPIOB_ARD_D10,PAL_MODE_ALTERNATE(4));//SCL

	debug_stream_init();

	dbgprintf("Start\r\n");

    i2cStart(i2cDriver, &i2cconfig);

    //gyro init
    i2cMasterTransmitTimeout(i2cDriver, GYRO_ADDR, gyro_init, 2, NULL, 0, TIME_INFINITE);

    //accelerometer init
    i2cMasterTransmitTimeout(i2cDriver, ACCEL_ADDR, accel_init, 2, NULL, 0, TIME_INFINITE);

    while (1) {

    	i2cMasterTransmitTimeout(i2cDriver, GYRO_ADDR, xyz_reg, 1, (uint8_t *)gyro_data, 6, TIME_INFINITE);
    	i2cMasterTransmitTimeout(i2cDriver, ACCEL_ADDR, xyz_reg, 1, (uint8_t *)accel_data, 6, TIME_INFINITE);
    	for(uint8_t i = 0; i < 3; i++)
    	{
    	    gyro_dps[i] = gyro_data[i] * 0.00875;
    	}

    	for(uint8_t j = 0; j < 3; j++)
    	{
    	    accel_g[j] = accel_data[j] * 0.004;
    	}

    	dbgprintf("dps: x = %f, y = %f, z = %f\r\n",
    			gyro_dps[0],gyro_dps[1],gyro_dps[2]);

    	dbgprintf("g: x = %f, y = %f, z = %f\r\n",
    			accel_g[0],accel_g[1],accel_g[2]);

    	dbgprintf("\r\n");
        chThdSleepMilliseconds(1000);

    }
}

