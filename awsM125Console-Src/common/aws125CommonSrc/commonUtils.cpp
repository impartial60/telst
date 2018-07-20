/*
#include <cstdlib>
#include <stdint.h>
#include <inttypes.h>

#include <stdio.h>
#include <cstring>
#include <iostream>

#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "../common/snrServiceInfo.h"
*/

float convert360angle2PlusMinus180(float angle360)
{
    float angle180 = angle360;

    if(angle180 < - 180)
        angle180 += 360.0;

    if(angle180 > 180)
        angle180 -= 360.0;

    //qDebug()<<"                               deltaAzimuthPlusMinus180: dltAz360:"<<dltAz360<<"dltAz180:"<<dltAz180;

    return angle180;
}
