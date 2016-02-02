#pragma once
#define MAX_SCENE	10

class CStageData
{
public:
	CStageData(void);
	virtual ~CStageData(void);

private:
	std::vector<BOOL>		mSceneStatus;

public:
	BOOL					SetScene( int sceneNumber, BOOL data );
	BOOL					GetScene( int sceneNumber );

	////----------------------------------------------//
	//VOID		SetFirstStage( BOOL data );
	//BOOL		GetFirstStage();
	//
	//VOID		SetSecondStage( BOOL data );
	//BOOL		GetSecondStage();
	//
	//VOID		SetThridStage( BOOL data );
	//BOOL		GetThridStage();
	////----------------------------------------------//
};