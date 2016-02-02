#include "stdafx.h"
#include "StageData.h"


CStageData::CStageData(void)
{
	mSceneStatus.assign( MAX_SCENE, false );
}

CStageData::~CStageData(void)
{
	if( false == mSceneStatus.empty() )
		mSceneStatus.clear();
}

BOOL CStageData::SetScene( int sceneNumber, BOOL data )
{
	if( MAX_SCENE < sceneNumber )
	{
		MULTI_LOG3() << "	+ Scene Range Exceed " << std::endl;
		return FALSE;
	}

	mSceneStatus[sceneNumber] = data;

	return TRUE;
}

BOOL CStageData::GetScene( int sceneNumber )
{
	if( MAX_SCENE < sceneNumber )
	{
		MULTI_LOG3() << "	+ Scene Range Exceed " << std::endl;
		return FALSE;
	}

	return mSceneStatus[sceneNumber];
}
////----------------------------------------------//
//VOID CStageData::SetFirstStage( BOOL data )
//{}
//BOOL CStageData::GetFirstStage()
//{
//	return TRUE;
//}
//
//VOID CStageData::SetSecondStage( BOOL data )
//{}
//BOOL CStageData::GetSecondStage()
//{
//	return TRUE;
//}
//
//
//VOID CStageData::SetThridStage( BOOL data )
//{}
//BOOL CStageData::GetThridStage()
//{
//	return TRUE;
//}
//
////----------------------------------------------//