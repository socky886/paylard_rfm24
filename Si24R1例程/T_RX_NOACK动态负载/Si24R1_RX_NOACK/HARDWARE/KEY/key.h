#ifndef _KEY_H
#define _KEY_H

#include "sys.h"

sbit KEY = P2^5;

void KEY_Init(void);
uchar KEY_Scan(uchar mode);

#endif



