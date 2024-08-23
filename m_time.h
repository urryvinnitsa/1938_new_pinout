//!**************************************************
//! file                           : m_time.h
//!**************************************************

#ifndef __m_time_H__
#define __m_time_H__
#include "hard_config.h"
void fnInitTime(void);
extern volatile sFlags_ sFlags;
#define IN_BUF_SIZE     256
extern volatile unsigned char  inbuf[IN_BUF_SIZE];
#endif //__m_time_H__

