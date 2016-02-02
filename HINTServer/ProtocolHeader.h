#pragma once

typedef enum TCPSensorProtocol
{
	PT_SETUP			= 100,
	PT_SETUP_LED,				// LED, 압력, 자물쇠	// 아두이노에서 서버로 데이터 일방 전송 / 단순히 보여주기
	PT_SETUP_LOCK,				// 자물쇠				// RFID(4)의 ON/OFF값으로 서버에서 판별값을 전송하여 자물쇠가 열림((1xxxx1xxx(열림) 1xxxx0xxx (잠김) ))
	PT_SETUP_RFID,				// RFID * 4				// 아두이노에서 서버로 데이터 일방 전송 / 1xxxx1xxx값이 오면 자물쇠(2)에 영향을 미침
	PT_SETUP_PRESSURE,			// 압력, RFID			// 아두이노에서 서버로 데이터 일방 전송 / 1xx255xx1값이 오면 서버에서 안드로이드로
	

	PT_DATA				= 300,	
	PT_DATA_LED,				// LED, 압력, 자물쇠
	PT_DATA_LOCK,				// 자물쇠
	PT_DATA_RFID,				// RFID * 4
	PT_DATA_RPESSURE,			// 압력, RFID

	PT_RESPONSE			= 500,
	PT_RESPONSE_LED,			// LED, 압력, 자물쇠
	PT_RESPONSE_LOCK,			// 자물쇠
	PT_RESPONSE_RFID,			// RFID * 5
	PT_RESPONSE_PRESSURE,		// 압력, RFID


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
	ALL_SENSOR_START,	// 공통 시작
	SENSOR_OFF,			// OFF
	SENSOR_ON,			// ON
}ArduinoMSG;
static std::string ArduinoPassingMessage[3] = 
{ 
	"xx1xxxxxx",		// 공통 시작
	"1xxxx0xxx",		// OFF
	"1xxxx1xxx",		// ON
};

/////////////////////////////////////////////////////////////////////////////////////////

typedef enum MobileMSG
{
	ALL_MOBILE_START,				// 모든 모바일 기기 접속
	MOBILE_OPEN_INTERVIEW,			// 심문이 열림
	MOBILE_OPEN_LOCK,				// 잠금센서가 열림 / 2진 코드 생성하라
}MobileMSG;
static std::string MobilePassingMessage[3] =
{
	"111112111",					// 모든 모바일 기기 접속
	"111113111",					// 심문이 열림
	"111114111",					// 잠금센서가 열림 / 2진 코드 생성하라
};

/////////////////////////////////////////////////////////////////////////////////////////