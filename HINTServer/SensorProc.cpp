#include "stdafx.h"

#pragma warning(disable:4996)		// strncpy :: C4996 Error


#include "ConnectSensor.h"
#include "ConnectSensorManager.h"
#include "HINTSensorIocp.h"
#include "HINTSensorControler.h"

#include "ProtocolHeader.h"

VOID CHINTSensorIocp::onPT_SENSOR_READY( CConnectSensor *ConnectSession, BYTE* packet )
{
	BYTE	WriteBuffer[MAXBUFFERLENGTH]	= {0,};


	MULTI_LOG3() << "# Read packet : PT_SENSOR_READY ";
	memcpy( packet, NULL, sizeof( BYTE )*MAXBUFFERLENGTH);

}
VOID CHINTSensorIocp::onPT_SENSOR_VALUE( CConnectSensor *ConnectSession, BYTE* packet )
{
	BYTE	WriteBuffer[MAXBUFFERLENGTH]	= {0,};

	MULTI_LOG3() << "# Read packet : PT_SENSOR_DATA			: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;
}

VOID CHINTSensorIocp::onPT_LED_SENSOR_SETUP( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : PT_LED_SENSOR_SETUP 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;

	if( mConnectedSensorManager.GetStatus() == SENSOR_STATUS::SENSOR_ARDUINO_SETUP_SUCC )
	{
		DWORD dataLength = ArduinoPassingMessage[ALL_SENSOR_START].size();
		ConnectSession->WritePacket( (byte*)ArduinoPassingMessage[ALL_SENSOR_START].c_str(), dataLength);
	}
}
VOID CHINTSensorIocp::onPT_LED_SENSOR_DATA( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : LED_SENSOR_DATA 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;

	if( ConnectSession->GetSensorValue0().find( "255" ) != std::string::npos)
	{
		// ħ�뿡 �ɾҴ� !
		mBunddleOfStageData.SetStage( STORY_THIRD, THRID_SCENE::SCENE_ON_BED, true );
		
		// USB �� �־��!
		sendPT_GIVE_USB( ConnectSession );
	}
}
VOID CHINTSensorIocp::onPT_LED_SENSOR_RESPONSE( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : PT_LED_SENSOR_RESPONSE 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;
}
	
VOID CHINTSensorIocp::onPT_LOCK_SENSOR_SETUP( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : PT_LOCK_SENSOR_SETUP 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;

	if( mConnectedSensorManager.GetStatus() == SENSOR_STATUS::SENSOR_ARDUINO_SETUP_SUCC )
	{
		DWORD dataLength = ArduinoPassingMessage[ALL_SENSOR_START].size();
		ConnectSession->WritePacket( (byte*)ArduinoPassingMessage[ALL_SENSOR_START].c_str(), dataLength);
	}
}
VOID CHINTSensorIocp::onPT_LOCK_SENSOR_DATA( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : PT_LOCK_SENSOR_DATA 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;
}
VOID CHINTSensorIocp::onPT_LOCK_SENSOR_RESPONSE( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : PT_LOCK_SENSOR_RESPONSE 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;
}
	
VOID CHINTSensorIocp::onPT_RFID_SENSOR_SETUP( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : PT_RFID_SENSOR_SETUP 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;

	if( mConnectedSensorManager.GetStatus() == SENSOR_STATUS::SENSOR_ARDUINO_SETUP_SUCC )
	{
		DWORD dataLength = ArduinoPassingMessage[ALL_SENSOR_START].size();
		ConnectSession->WritePacket( (byte*)ArduinoPassingMessage[ALL_SENSOR_START].c_str(), dataLength);
	}
}
VOID CHINTSensorIocp::onPT_RFID_SENSOR_DATA( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : PT_RFID_SENSOR_DATA 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;

	if( ConnectSession->GetSensorValue0().find( "xx1" ) != std::string::npos)
	{
		// RFID ON
		mBunddleOfStageData.SetStage( STORY_SECOND, SECOND_SCENE::SCENE_SETTING_FIGURE, true );

		// �ڹ���� ������! ( ���� -> �Ƶ��̳� )
		CConnectSensor *ConnectSession2 = mConnectedSensorManager.FindConnectSensor( "xx2" );	 // �ڹ���
		sendPT_GIVE_BINARYCODE( ConnectSession2 );

		// �����ڵ�� ������! ( ���� -> ����� )
		sendPT_CREATE_BINARYCODE();
	
	}
}
VOID CHINTSensorIocp::onPT_RFID_SENSOR_RESPONSE( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : PT_RFID_SENSOR_RESPONSE 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;
}
	
VOID CHINTSensorIocp::onPT_PRESSURE_SENSOR_SETUP( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : PT_PRESSURE_SENSOR_SETUP 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;

	if( mConnectedSensorManager.GetStatus() == SENSOR_STATUS::SENSOR_ARDUINO_SETUP_SUCC )
	{
		// ������� SENSOR_ALL_SETUP_SUCC�϶� �ѹ��� ���������� �Ƶ��̳밡 Ǯ�� ����̹Ƿ� �Ұ�����.
		DWORD dataLength = ArduinoPassingMessage[ALL_SENSOR_START].size();
		ConnectSession->WritePacket( (byte*)ArduinoPassingMessage[ALL_SENSOR_START].c_str(), dataLength);
	}
}
VOID CHINTSensorIocp::onPT_PRESSURE_SENSOR_DATA( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : PT_PRESSURE_SENSOR_DATA 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;

	if( ConnectSession->GetSensorValue0().find("255") != std::string::npos
		&& ConnectSession->GetSensorValue1().find("xx1") != std::string::npos )
	{
		// ���� �з� �Ϸ� && RFID ��ġ �Ϸ� 1
		mBunddleOfStageData.SetStage( STORY_FIRST, FIRST_SCENE::SCENE_SETTING_CHAIR, true );
		
		sendPT_OPEN_INTERVIEW();
	}
}
VOID CHINTSensorIocp::onPT_PRESSURE_SENSOR_RESPONSE( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : PT_PRESSURE_SENSOR_RESPONSE 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;
}

//------------------------- Protocol Packet -----------------------------//
VOID CHINTSensorControler::onPT_SENSOR_VALUE( CConnectSensor *ConnectSession, BYTE* packet )
{
}

VOID CHINTSensorControler::PT_SENSOR_VALUE_SEND( CHAR* packet )
{
	std::string inputdata = packet;	

	CConnectSensor* receiver = mConnectedSensorManager.FindConnectSensor( inputdata.substr(0,3) );
	receiver->WritePacket( (byte*)inputdata.substr(3).c_str(), strlen(inputdata.substr(3).c_str()) );
	
	MULTI_LOG3() << "Receiver : " << receiver->GetSocket() << inputdata.substr(3).c_str() << std::endl;
}
//------------------------- Protocol Packet -----------------------------//

VOID CHINTSensorIocp::onPT_MOBILE_CONNECT( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : onPT_ALL_MOBILE_CONNECT 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;	

	mConnectedSensorManager.CheckCurrentStatus();

	if( mConnectedSensorManager.GetStatus() == SENSOR_STATUS::SENSOR_MOBLIE_SETUP_SUCC || mConnectedSensorManager.GetStatus() == SENSOR_STATUS::SENSOR_ALL_SETUP_SUCC )
	{
		MULTI_LOG3() << "# Send Packet : ALL_MOBILE_READY    :" << std::endl;
		mConnectedSensorManager.WriteMobile( (byte*)MobilePassingMessage[ALL_MOBILE_START].c_str(), MobilePassingMessage[ALL_MOBILE_START].size() );
	}
}
VOID CHINTSensorIocp::onPT_SCIENCE_OPEN( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : onPT_SCIENCE_OPEN 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;

	// ���� ����� ����
	mBunddleOfStageData.SetStage( STORY_FIRST, FIRST_SCENE::SCENE_OPEN_SCIENCE, true );
}
VOID CHINTSensorIocp::onPT_SCIENCE_CLEAR( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : onPT_SCIENCE_CLEAR 		: "
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;

	// ���м���� �Ϸ�
	mBunddleOfStageData.SetStage( STORY_FIRST, FIRST_SCENE::SCENE_FNISH_SCIENCE, true );
}
VOID CHINTSensorIocp::onPT_DIARY_OPEN( CConnectSensor *ConnectSession, BYTE* packet )
{
	MULTI_LOG3() << "# Read packet : onPT_DIARY_OPEN 		: " 
	<< ConnectSession->GetSensorType()
	<< ConnectSession->GetSensorValue0()
	<< ConnectSession->GetSensorValue1() << std::endl;

	// ���̾�� ����
	mBunddleOfStageData.SetStage( STORY_SECOND, SECOND_SCENE::SCENE_OPEN_DIARY, true );
}


//------------------------- Protocol Packet -----------------------------//
VOID CHINTSensorIocp::sendPT_GIVE_USB( CConnectSensor *ConnectSession )
{
	MULTI_LOG3() << "# Read packet : sendPT_GIVE_USB 		: " << std::endl;

	// SCINE_GIVE_USB : USB�� ��
	mBunddleOfStageData.SetStage( STORY_THIRD, THRID_SCENE::SCINE_GIVE_USB, true );

	DWORD dataLength = ArduinoPassingMessage[SENSOR_ON].size();
	ConnectSession->WritePacket( (byte*)ArduinoPassingMessage[SENSOR_ON].c_str(), dataLength);
}

VOID CHINTSensorIocp::sendPT_OPEN_INTERVIEW()
{
	MULTI_LOG3() << "# Read packet : sendPT_OPEN_INTERVIEW 		: " 
		<< MobilePassingMessage[MOBILE_OPEN_INTERVIEW] << std::endl;
	
	// SCENE_OPEN_INTERVIEW : ���ͺ� ����
	mBunddleOfStageData.SetStage( STORY_FIRST, FIRST_SCENE::SCENE_OPEN_INTERVIEW, true );

	DWORD dataLength = MobilePassingMessage[MOBILE_OPEN_INTERVIEW].size();
	mConnectedSensorManager.WriteMobile((byte*)MobilePassingMessage[MOBILE_OPEN_INTERVIEW].c_str(), dataLength);
}

VOID CHINTSensorIocp::sendPT_GIVE_BINARYCODE( CConnectSensor *ConnectSession )
{
	MULTI_LOG3() << "# Read packet : sendPT_GIVE_BINARYCODE 		: " << std::endl;

	// SCINE_GIVE_USB : USB�� ��
	mBunddleOfStageData.SetStage( STORY_SECOND, SECOND_SCENE::SCENE_GIVE_BINARYCODE, true );

	DWORD dataLength = ArduinoPassingMessage[SENSOR_ON].size();
	ConnectSession->WritePacket( (byte*)ArduinoPassingMessage[SENSOR_ON].c_str(), dataLength);
}

VOID CHINTSensorIocp::sendPT_CREATE_BINARYCODE()
{
	MULTI_LOG3() << "# Read packet : sendPT_CREATE_BINARYCODE 		: "
		<< MobilePassingMessage[MOBILE_OPEN_LOCK] << std::endl;
	
	// SCENE_OPEN_INTERVIEW : ���ͺ� ����
	mBunddleOfStageData.SetStage( STORY_SECOND, SECOND_SCENE::SCENE_CREATE_BINARYCODE, true );

	DWORD dataLength = MobilePassingMessage[MOBILE_OPEN_LOCK].size();
	mConnectedSensorManager.WriteMobile((byte*)MobilePassingMessage[MOBILE_OPEN_LOCK].c_str(), dataLength);
}