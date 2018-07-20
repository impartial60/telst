/*
 * devicedio.h
 *
 *  Created on: 17 окт. 2016 г.
 *      Author: dolin
 */

#ifndef DEVICEDIO_H_
#define DEVICEDIO_H_

#include <stdint.h>
#include <string>
#include <signal.h>

#include <MEN/men_typs.h>
#include <MEN/mdis_api.h>
#include <MEN/usr_oss.h>
#include <MEN/z17_drv.h>


//static int dio_sigCount = 0;

//static void  SignalHandler1(u_int32 sig)
//{
//	if (sig == UOS_SIG_USR1)
//		++dio_sigCount;
//};


// работа с устройством SA15 через MDIS

class device_dio {
public:
	device_dio();
	virtual ~device_dio();

	// открыть устройство
	int32_t		open(std::string v_dev_name); // например z17_1

	// инициализация по умолчанию
	int32_t		init_def();

	void 		delay(const uint32_t time_msec);

	// установка в 0 указанных в маске битов
	int32_t		bits_mask_clear(const uint32_t v_mask);

	// установка в 1 указанных в маске битов
	int32_t		bits_mask_set(const uint32_t v_mask);

	// установка output bits
	int32_t 	bits_write(const uint32_t v_value);

	// чтение input bits
	int32_t 	bits_read(int32_t* v_value);

	// програм. направления работы выводов. 1 - выход, 0 - вход
	// value = 0x1f ==>	program lower five ports as outputs, others as inputs
	int32_t 	bits_direction(const int32_t v_value);

	int32_t		signal_set(const int v_sig_num);

	//Z17_IRQ_SENSE (0xFFFF generate interrupts for all channels on rising and falling edge)
	// 				(0x5400 [01 01 01 00 00 00 00 00] generate interrupts on rising edge of inputs)
    int32_t		irq_mode_channels(const int32_t v_mode);

	// SIGUSR1, SIGUSR2
	// void (*sigset(int sig, void (*disp)(int)))(int);

	// установка сигнала на прерывания устройства.  void (__MAPILIB *sigHandler)(u_int32 sigCode)
	int32_t		irq_set_signal(int sig_num, void (__MAPILIB *sigHandler)(u_int32 sigCode));

	// включить прерывания для устройства 0 -- выключить, 1 -- включить
	// SG enable global device interrupt
	int32_t		irq_enable(const uint32_t v_value);

	//
	// int32_t
	// сервисная функция -- преобразует значение в строку двоичной системы.
	static std::string  sv_bits_to_string(const uint32_t v_value, const int nrBits);

	static std::string  sv_print_error(char* v_message);




protected:
	MDIS_PATH  	m_device;
	int			m_signum; // номер используемого для работы с прерываниями сигнала




};

/*-----------------------------------------+
|  TYPEDEFS                                |
+-----------------------------------------*/
/* structure for the Z17_BLK_IRQLAT_X setstat/getstat */

/*
typedef struct {
	 in
	u_int32 outPort;     output port (0..31)
	u_int32 inPort;      input port (0..31)
	u_int32 irqs2fire;   number of interrupts to fire
	 out
	u_int32 tickRate;    tick rate from OSS_TickRateGet
	u_int32 startTick;   start tick count
	u_int32 stopTick;    stop tick count
	u_int32 irqsRcved;   received IRQs
} Z17_BLK_IRQLAT;
*/

/* structure for the Z17_BLK_DEBOUNCE_TIME setstat/getstat */

/*
typedef struct {
	 in
	u_int32 portMask;    port mask (0x0..0xffffffff)
	 in/out
	u_int32 timeUs;   	 time in us (in 50us steps)
} Z17_BLK_DEBTIME;
*/

/*-----------------------------------------+
|  DEFINES                                 |
+-----------------------------------------*/
/** \name Z17 specific Getstat/Setstat standard codes
 *  \anchor getstat_setstat_codes
 */
/**@{*/
/*
#define Z17_SET_PORTS           M_DEV_OF+0x00    *<   S: Set IO ports without affecting others - 8 bit mask - bit 0 for gpio[0]
#define Z17_CLR_PORTS           M_DEV_OF+0x01    *<   S: Clear IO ports without affecting others - 8 bit mask - bit 0 for gpio[0]
#define Z17_DIRECTION           M_DEV_OF+0x02    *< G,S: Get/Set direction of all ports - 8bit - bit 0 for gpio[0] - value 0 is in - 1 is out
#define Z17_IRQ_SENSE           M_DEV_OF+0x03    *< G,S: Get/set IRQ sense mode for models Z34/Z37 - 16 bit - and for model Z127 - 32 bit - bit 0 and 1 for gpio[0] - 0 no, 1 rising, 2 falling, 3 both edges
#define Z17_OPEN_DRAIN          M_DEV_OF+0x04    *< G,S: Get/set open drain mode - 8bit - bit 0 for gpio[0] - value 0 is dis - 1 is enable
#define Z17_DEBOUNCE            M_DEV_OF+0x05    *< G,S: Get/set debouncing of all ports - 8bit - bit 0 for gpio[0] - value 0 is dis - 1 is enable
#define Z17_SET_SIGNAL          M_DEV_OF+0x06    *<   S: Set signal sent on IRQ
#define Z17_CLR_SIGNAL          M_DEV_OF+0x07    *<   S: Uninstall signal
#define Z17_IRQ_LAST_REQUEST    M_DEV_OF+0x08    *< G  : Get last IRQ request - 8bit - bit 0 for gpio[0] - value 1 is requested - clears the last request
#define Z17_IRQ_SENSE_16TO31    M_DEV_OF+0x09    *< G,S: Get/set IRQ sense mode only for model Z127 - 32 bit - bit 0 and 1 for gpio[16] - 0 no, 1 rising, 2 falling, 3 both edges
#define Z17_TOG_PORTS           M_DEV_OF+0x0a    *<   S: Toggle IO ports without affecting others - 8/32 bit mask - bit 0 for gpio[0]
#define Z17_TOG_HIGH            M_DEV_OF+0x0b    *<   S: Toggle phase high - default 1000ms
#define Z17_TOG_LOW             M_DEV_OF+0x0c    *<   S: Toggle phase low - default 1000ms
*/

/* Z17 specific Getstat/Setstat block codes (for test purposes) */
/*
#define Z17_BLK_IRQLAT_START    M_DEV_BLK_OF+0x00	   S: Initialize IRQ latency test
#define Z17_BLK_IRQLAT_RESULT   M_DEV_BLK_OF+0x01	 G  : Get result of IRQ latency test
#define Z17_BLK_DEBOUNCE_TIME   M_DEV_BLK_OF+0x02	 G,S: Get/set debounce time (us) for specified port(s)
													      Get: specify only one port
													      Set: specify one ore more ports
*/




#endif /* DEVICEDIO_H_ */
