#pragma once
#define MAX_STAGE			5

class CStageData;

class CBundleOfStageData
{
public:
	CBundleOfStageData(void);
	virtual ~CBundleOfStageData(void);

private:
	std::vector<CStageData*>			mStageData;

public:
	BOOL								SetStage( int stageNumber, int sceneNumber, BOOL data );
	BOOL								GetStage( int stageNumber, int sceneNumber );
};

typedef enum ENUM_STORY_TYPE
{
	STORY_FIRST			,
	STORY_SECOND		,
	STORY_THIRD			,
	STORY_FINAL_CHOICE	,
};
typedef enum ENUM_FIRST_SCENE_TYPE
{
	SCENE_SETTING_CHAIR		,		// 아두이노에서 서버에게 (의자세움/압력&RFID)
	SCENE_OPEN_INTERVIEW	,		// 서버에서 모바일에게 (인터뷰열림)
	SCENE_OPEN_SCIENCE		,		// 모바일에서 서버에게 (국과수열림)
	SCENE_FNISH_SCIENCE		,		// 모바일에서 서버에게 (국과수결과)
}FIRST_SCENE;
typedef enum ENUM_SECOND_SCENE_TYPE
{
	SCENE_OPEN_DIARY		,		// 모바일에서 서버에게 (다이어리 RFID)
	SCENE_SETTING_FIGURE	,		// 아두이노에서 서버에게 (피규어)
	SCENE_GIVE_BINARYCODE	,		// 서버에서 아두이노에게 (자물쇠)
	SCENE_CREATE_BINARYCODE	,		// 서버에서 모바일에게 (이진코드 생성)
}SECOND_SCENE;
typedef enum ENUM_THRID_SCENE_TYPE
{
	SCENE_ON_BED			,		// 침대위에 앉았을 시	( 압력센서		: 아두이노 -> 서버 ) 
	SCINE_GIVE_USB			,		// USB 생성				( LED / 자물쇠	: 서버 -> 아두이노 )
}THRID_SCENE;
/* ==================================== */
//		<<		story1.		>>
//
//	1.1		> 의자세움
//	1.2		- 인터뷰 열림
//	1.3		- 국과수 열림
//	1.4		- 국과수 조사결과 알림
/* ==================================== */

/* ==================================== */
//		<<		story2.		>>
//
//	2.1		- 다이어리 해금
//	2.2		> 피규어 조합(RFID*5)
//	2.3		> 이진코드 제공
//	2.4		- 수첩에 이진코드 생성 			
/* ==================================== */

/* ==================================== */
//		<<		story3.		>>
//
//	3.1		> 침대위에 앉음
//  3.2		> USB 생성
/* ==================================== */
