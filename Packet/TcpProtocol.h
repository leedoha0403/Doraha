#pragma once
#include "gHeader.h"

typedef enum TCP_PROTOCOL
{
	PT_VERSION	= 0x1000000,

#ifdef _INCLUDE_DATABASE
	
#endif
	PT_SENSOR_READY,
	/*
	BOOL			: READY
	*/
	PT_SENSOR_DATA,
	/*
	CHAR		:	UserID[3]
	CHAR		:	Type[3]
	CHAR		:	Value0[3]
	CHAR		:	Value1[3]
	*/
	PT_END,
	/*
	*/
};