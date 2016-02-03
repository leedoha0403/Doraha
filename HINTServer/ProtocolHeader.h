#pragma once

typedef enum TCPSensorProtocol
{
	PT_SETUP			= 100,
	PT_SETUP_LED,				// LED, �з�, �ڹ���	// �Ƶ��̳뿡�� ������ ������ �Ϲ� ���� / �ܼ��� �����ֱ�
	PT_SETUP_LOCK,				// �ڹ���				// RFID(4)�� ON/OFF������ �������� �Ǻ����� �����Ͽ� �ڹ��谡 ����((1xxxx1xxx(����) 1xxxx0xxx (���) ))
	PT_SETUP_RFID,				// RFID * 4				// �Ƶ��̳뿡�� ������ ������ �Ϲ� ���� / 1xxxx1xxx���� ���� �ڹ���(2)�� ������ ��ħ
	PT_SETUP_PRESSURE,			// �з�, RFID			// �Ƶ��̳뿡�� ������ ������ �Ϲ� ���� / 1xx255xx1���� ���� �������� �ȵ���̵��
	

	PT_DATA				= 300,	
	PT_DATA_LED,				// LED, �з�, �ڹ���
	PT_DATA_LOCK,				// �ڹ���
	PT_DATA_RFID,				// RFID * 4
	PT_DATA_RPESSURE,			// �з�, RFID

	PT_RESPONSE			= 500,
	PT_RESPONSE_LED,			// LED, �з�, �ڹ���
	PT_RESPONSE_LOCK,			// �ڹ���
	PT_RESPONSE_RFID,			// RFID * 5
	PT_RESPONSE_PRESSURE,		// �з�, RFID


	PT_UNITY_FOR_SERVER			= 1000,
	PT_MOBILE_CONNECT,
	PT_SCIENCE_OPEN,
	PT_SCIENCE_CLEAR,
	PT_DIARY_OPEN,
	
	//PT_SERVER_FOR_UNITY			= 2000,
	//PT_HEARING_OPEN,
	//PT_LOCKER_OPEN,

	PT_END
};

/////////////////////////////////////////////////////////////////////////////////////////

typedef enum ArduinoMSG
{
	ALL_SENSOR_START,	// ���� ����
	SENSOR_OFF,			// OFF
	SENSOR_ON,			// ON
}ArduinoMSG;
static std::string ArduinoPassingMessage[3] = 
{ 
	"xx1xxxxxx",		// ���� ����
	"1xxxx0xxx",		// OFF
	"1xxxx1xxx",		// ON
};

/////////////////////////////////////////////////////////////////////////////////////////

typedef enum MobileMSG
{
	ALL_MOBILE_START,				// ��� ����� ��� ����
	MOBILE_OPEN_INTERVIEW,			// �ɹ��� ����
	MOBILE_OPEN_LOCK,				// ��ݼ����� ���� / 2�� �ڵ� �����϶�
}MobileMSG;
static std::string MobilePassingMessage[3] =
{
	"111112111",					// ��� ����� ��� ����
	"111113111",					// �ɹ��� ����
	"111114111",					// ��ݼ����� ���� / 2�� �ڵ� �����϶�
};

/////////////////////////////////////////////////////////////////////////////////////////