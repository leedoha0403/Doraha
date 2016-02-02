#pragma once

inline DWORD WRITE_PT_SENSOR_READY(BYTE *buffer, S_PT_SENSOR_READY &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteBOOL(parameter.READY);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_SENSOR_DATA(BYTE *buffer, S_PT_SENSOR_DATA &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteBytes((BYTE*) parameter.UserID, sizeof(CHAR) * 3);
	Stream->WriteBytes((BYTE*) parameter.Type, sizeof(CHAR) * 3);
	Stream->WriteBytes((BYTE*) parameter.Value0, sizeof(CHAR) * 3);
	Stream->WriteBytes((BYTE*) parameter.Value1, sizeof(CHAR) * 3);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_END(BYTE *buffer, S_PT_END &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_SENSOR_READY(BYTE *buffer, BOOL ready)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteBOOL(ready);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_SENSOR_DATA(BYTE *buffer, CHAR *userid, CHAR *type, CHAR *value0, CHAR *value1)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteBytes((BYTE*) userid, sizeof(CHAR) * 3);
	Stream->WriteBytes((BYTE*) type, sizeof(CHAR) * 3);
	Stream->WriteBytes((BYTE*) value0, sizeof(CHAR) * 3);
	Stream->WriteBytes((BYTE*) value1, sizeof(CHAR) * 3);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_END(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

