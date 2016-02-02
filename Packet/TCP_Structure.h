#pragma once

typedef struct _S_PT_SENSOR_READY
{
	BOOL READY;
} S_PT_SENSOR_READY;

typedef struct _S_PT_SENSOR_DATA
{
	CHAR UserID[3];
	CHAR Type[3];
	CHAR Value0[3];
	CHAR Value1[3];
} S_PT_SENSOR_DATA;

typedef struct _S_PT_END
{
} S_PT_END;

