/*
 * devicedio.cpp
 *
 *  Created on: 17 окт. 2016 г.
 *      Author: dolin
 */

#include "devicedio.h"

#include <stdio.h>
#include <string.h>

#include "devicedio.h"

// MDIS API
#include <MEN/men_typs.h>
#include <MEN/mdis_api.h>
#include <MEN/usr_oss.h>
#include <MEN/z17_drv.h>


device_dio::device_dio() :
		m_device(-1),
		m_signum(-1)
{
	// TODO Auto-generated constructor stub
    printf("devicedio.cpp Metka 1 %s: \n", __FUNCTION__);

}

device_dio::~device_dio()
{
	// TODO Auto-generated destructor stub
	printf("%s: \n", __FUNCTION__);
}


int32_t device_dio::open(std::string v_dev_name)
{

	m_device = M_open(v_dev_name.c_str());
    printf("devicedio.cpp Metka 2 %s: Res=%d DevName=%s\n", __FUNCTION__, m_device, v_dev_name.c_str());
	if (m_device < 0 )
	{
        printf("open error: device %s - %d\n", v_dev_name.c_str(), m_device );
	}
	return m_device;
}

int32_t device_dio::init_def()
{
    // program lower five ports as outputs, others as inputs
	M_write(m_device, 0x00);
	M_setstat(m_device, Z17_DIRECTION, 0x1f);

    // generate interrupts on rising edge of inputs
	M_setstat(m_device, Z17_IRQ_SENSE, 0x5400);

    // SG enable global device interrupt
	M_setstat(m_device, M_MK_IRQ_ENABLE, TRUE);
}

void device_dio::delay(const uint32_t time_msec)
{
	UOS_Delay(time_msec);
}

int32_t device_dio::bits_mask_clear(const uint32_t v_mask)
{
	int32_t Res;
	Res = M_setstat(m_device, Z17_CLR_PORTS, v_mask);
    printf("device.cpp bits_mask clear %s: Res=%d Value=%d\n", __FUNCTION__, Res, v_mask);

}


int32_t device_dio::bits_mask_set(const uint32_t v_mask)
{
	int32_t Res;
	Res = M_setstat(m_device, Z17_SET_PORTS, v_mask);
    printf("device.cpp bits_mask_set %s: Res=%d Value=%d\n", __FUNCTION__, Res, v_mask);
	return Res;
}

int32_t device_dio::bits_write(const uint32_t v_value)
{
	int32_t Res;
	Res = M_write(m_device, v_value);
    //printf("%s: Res=%d Value=%d\n", __FUNCTION__, Res, v_value);
	return Res;
}

int32_t device_dio::bits_read(int32_t* v_value)
{
	int32_t Res;
	int32 v_data;

	Res =  M_read(m_device, &v_data);
    // printf("%s: super--->  Value=%d\n", __FUNCTION__, v_data);
	*v_value=v_data;
    //printf("%s: Res=%d Value=%d\n", __FUNCTION__, Res, *v_value);
	return Res;
}

std::string device_dio::sv_bits_to_string(const uint32_t v_value, const int nrBits)
{
	char buffer[100];
	char* s;
	s = buffer;

	u_int32 bit;
	int i;

	*s = '\0';

	bit = 1 << (nrBits-1);

	for (i = 0; i < nrBits; ++i, bit >>= 1)
		strcat(s, v_value & bit ? "1 " : "0 ");

	return s;
	// return  "Hello ";
}

int32_t device_dio::irq_mode_channels(const int32_t v_mode)
{
	int32_t Res;
    Res = M_setstat(m_device, Z17_IRQ_SENSE, (INT32_OR_64) v_mode);
    printf("%s: Res=%d mode=%d\n", __FUNCTION__, Res, v_mode);
	return Res;
}

int32_t device_dio::irq_set_signal(int sig_num, void (__MAPILIB *sigHandler)(u_int32 sigCode))
{
    // install signal which will be received at change of input ports
	UOS_SigInit(sigHandler);
	UOS_SigInstall(sig_num);
	return M_setstat(m_device, Z17_SET_SIGNAL, sig_num);
}

int32_t device_dio::bits_direction(const int32_t v_value)
{
	int32_t Res;
	Res = M_setstat(m_device, Z17_DIRECTION, v_value);
    printf("device.cpp bits_direction %s: Res=%d Value=%d\n", __FUNCTION__, Res, v_value);
	return Res;
}

int32_t device_dio::irq_enable(const uint32_t v_value)
{
    // SG enable global device interrupt
	int32_t Res;
	Res = M_setstat(m_device, M_MK_IRQ_ENABLE, v_value);
    printf("device.cpp irq_enabled %s: Res=%d Value=%d\n", __FUNCTION__, Res, v_value);
	return Res;
}

std::string device_dio::sv_print_error(char* v_message)
{
	printf("*** can't %s: %s\n", v_message, M_errstring(UOS_ErrnoGet()));
}

