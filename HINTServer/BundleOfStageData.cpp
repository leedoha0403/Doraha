#include "stdafx.h"
#include "StageData.h"
#include "BundleOfStageData.h"

CBundleOfStageData::CBundleOfStageData(void)
{
	for( int i = 0 ; i < MAX_STAGE ; ++i )
	{
		CStageData* temp = new CStageData;
		mStageData.push_back( temp );
	}
}


CBundleOfStageData::~CBundleOfStageData(void)
{
	if( false == mStageData.empty() )
	{
		mStageData.clear();
	}
}

BOOL CBundleOfStageData::SetStage( int stageNumber, int sceneNumber, BOOL data )
{
	if( MAX_STAGE < stageNumber )
	{
		MULTI_LOG3() << "	+ Stage Range Exceed " << std::endl;
		return FALSE;
	}
	return mStageData[stageNumber]->SetScene( sceneNumber, data );
}
BOOL CBundleOfStageData::GetStage( int stageNumber, int sceneNumber )
{
	if( MAX_STAGE < stageNumber )
	{
		MULTI_LOG3() << "	+ Stage Range Exceed " << std::endl;
		return FALSE;
	}

	return mStageData[stageNumber]->GetScene( sceneNumber );
}