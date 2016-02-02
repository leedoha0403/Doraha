#pragma once

inline VOID READ_PT_SENSOR_READY(BYTE *buffer, S_PT_SENSOR_READY &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadBOOL(&parameter.READY);
}

inline VOID READ_PT_SENSOR_DATA(BYTE *buffer, S_PT_SENSOR_DATA &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadBytes((BYTE*) parameter.UserID, sizeof(CHAR) * 3);
	Stream->ReadBytes((BYTE*) parameter.Type, sizeof(CHAR) * 3);
	Stream->ReadBytes((BYTE*) parameter.Value0, sizeof(CHAR) * 3);
	Stream->ReadBytes((BYTE*) parameter.Value1, sizeof(CHAR) * 3);
}

inline VOID READ_PT_END(BYTE *buffer, S_PT_END &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);
}

