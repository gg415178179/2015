#ifndef	_TIMER_H_
#define	_TIMER_H_

#include "sys.h"
#include "types.h"

#define KA_SEND_PERIOD    2 //in second. send Keepalive in every KA_SEND_PERIOD seconds
#define NO_DATA_PERIOD    10//in second. if no data exchange between server and client time is longer than NO_DATA_PERIOD, 



void TIM3_Config(u16 arr,u16 psc);
void TIM6_Config(void);


#endif		

