// HINTServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "ConnectSensor.h"
#include "ConnectSensorManager.h"
#include "HINTSensorIocp.h"

#include "HINTSensorIocp.h"
#include <objbase.h>

int _tmain(int argc, _TCHAR* argv[])
{
	CREATE_FOLDER_PATH("TEST+FILE");
	
	MULTI_LOG3() << "+ Prepare Startup" << std::endl ;
	
	MULTI_LOG3() << "	- Winsock2 DLL loading ";
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);
	MULTI_LOG3() << "	: OK " << std::endl ;
	
	MULTI_LOG3() << "	- Database DLL loading ";
	DBINST->InitializeDatabase( "127.0.0.1", "local", "root", "local" );	// host, pass, dbname
	MULTI_LOG3() << "	: OK " << std::endl ;
	
	MULTI_LOG3() << "+ HINT Server Startup " << std::endl;
	CoInitialize(NULL);

	CHINTSensorIocp				SensorServer;

	MULTI_LOG3() << "	- HINT Sensor Server Begin : ";
	if ( SensorServer.Begin( 9600 ) )										// port 번호
		MULTI_LOG3() << "	: OK " << std::endl ;
	else
	{
		WSACleanup();
		
		MULTI_LOG3() << "	: FAIL " << std::endl ;
		MULTI_LOG3() << "# HINT Sensor Server Stoped... #" << std::endl;
		MULTI_LOG3() << "Press any key... " << std::endl;
		getchar();

		return 0;
	}

	MULTI_LOG3() << std::endl;
	MULTI_LOG3() <<"# HINT Server Running... #" << std::endl;

	MULTI_LOG3() <<"	- HINT Sensor Server Iocp End	: [ ok ]\n"<< std::endl;
	SensorServer.End();

	WSACleanup();
	MULTI_LOG3() <<"	- Winsock2 DLL unloading	: [ ok ]\n"<< std::endl;

	CoUninitialize();

	MULTI_LOG3() <<"# HINT Server Stoped... #\n"<< std::endl;
	MULTI_LOG3() <<"Press any key... \n"<< std::endl;
	getchar();

	return 0;
}

