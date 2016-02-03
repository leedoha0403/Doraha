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
	SCENE_SETTING_CHAIR		,		// �Ƶ��̳뿡�� �������� (���ڼ���/�з�&RFID)
	SCENE_OPEN_INTERVIEW	,		// �������� ����Ͽ��� (���ͺ俭��)
	SCENE_OPEN_SCIENCE		,		// ����Ͽ��� �������� (����������)
	SCENE_FNISH_SCIENCE		,		// ����Ͽ��� �������� (���������)
}FIRST_SCENE;
typedef enum ENUM_SECOND_SCENE_TYPE
{
	SCENE_OPEN_DIARY		,		// ����Ͽ��� �������� (���̾ RFID)
	SCENE_SETTING_FIGURE	,		// �Ƶ��̳뿡�� �������� (�ǱԾ�)
	SCENE_GIVE_BINARYCODE	,		// �������� �Ƶ��̳뿡�� (�ڹ���)
	SCENE_CREATE_BINARYCODE	,		// �������� ����Ͽ��� (�����ڵ� ����)
}SECOND_SCENE;
typedef enum ENUM_THRID_SCENE_TYPE
{
	SCENE_ON_BED			,		// ħ������ �ɾ��� ��	( �з¼���		: �Ƶ��̳� -> ���� ) 
	SCINE_GIVE_USB			,		// USB ����				( LED / �ڹ���	: ���� -> �Ƶ��̳� )
}THRID_SCENE;
/* ==================================== */
//		<<		story1.		>>
//
//	1.1		> ���ڼ���
//	1.2		- ���ͺ� ����
//	1.3		- ������ ����
//	1.4		- ������ ������ �˸�
/* ==================================== */

/* ==================================== */
//		<<		story2.		>>
//
//	2.1		- ���̾ �ر�
//	2.2		> �ǱԾ� ����(RFID*5)
//	2.3		> �����ڵ� ����
//	2.4		- ��ø�� �����ڵ� ���� 			
/* ==================================== */

/* ==================================== */
//		<<		story3.		>>
//
//	3.1		> ħ������ ����
//  3.2		> USB ����
/* ==================================== */
