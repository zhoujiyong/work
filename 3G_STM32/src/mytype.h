#ifndef __MYTYPE_H__
#define __MYTYPE_H__

#include "stm32lib\\stm32f10x.h"
#include "stdio.h"

typedef unsigned char uchar;
typedef unsigned int uint; 

#define FINGER_PAGE_RESET 0
#define READER_FINGER_EN  0
#define UHF_READER_EN  1
#define LOCAL_MOTOR_CTL_EN 0
#define REMOTE_MOTOR_CTL_EN 1

/*  求两个数据的绝对差值  */
#define DIFF(a, b)    ((a)<=(b) ?(b-a):(a-b))

//#define IOT_DEBUG  /*  调试开关  */
#ifdef IOT_DEBUG
    /*  print debug message only if debug message type is enabled...  */
    #  define IOT_DEBUGF(x) do {   printf##x;  } while(0)
#else 
    #  define IOT_DEBUGF(x) 
#endif /* IOT_DEBUG */

#endif
