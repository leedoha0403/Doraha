#pragma once
#include <sstream> 
#include <time.h>
#include <Windows.h>
#include <ostream>
#include <direct.h>			//mkdir
#include <errno.h>			//error
#include <list>
//#pragma comment(lib,"libmysql.lib")		// mytsql 

extern enum ENUM_LOGLEVEL
{
	ENM_LOG_ERROR,
	ENM_LOG_WARNING,
	ENM_LOG_INFO,
	//ENM_LOG_FILELOG,
	ENM_LOG_DEBUG,
	ENM_LOG_DEBUG1,
	ENM_LOG_DEBUG2,
	ENM_LOG_DEBUG3,
	ENM_LOG_DEBUG4,
};

extern std::string	NowTime();

//class CLogFilePathManager
//{
//public:
//	static std::list<CLogFile*>	mStrPathManager;
//public:
//	static bool					CreateFolder( __in std::string strPath );
//};

extern class CLogFile
{
public:
	static std::string mStrPath;
public:
	static FILE*&		Stream();
	static void		Output( __in const std::string& message );
	static void		CreateFolder( __in std::string strPath );
};

extern class CLogConsole
{
public:
	static void		Output( __in const std::string& message );
};

extern class CLog
{
public:
	CLog(void);
	virtual ~CLog(void);
protected:
	std::ostringstream		mOS;
	std::ostringstream		mCS;
	std::ostringstream		mMS;

public:
	std::ostringstream&		GetStream( std::string fileName, __in int nline, __in ENUM_LOGLEVEL	level = ENM_LOG_INFO );
	std::ostringstream&		GetCStream( std::string fileName,__in int nline, __in ENUM_LOGLEVEL	level = ENM_LOG_INFO );
	std::ostringstream&		GetStream( __in ENUM_LOGLEVEL	level = ENM_LOG_INFO );
	std::ostringstream&		GetCStream( __in ENUM_LOGLEVEL	level = ENM_LOG_INFO );
	
	std::ostringstream&		GetMStream(  std::string fileName, __in int nline, __in ENUM_LOGLEVEL	level = ENM_LOG_INFO );
	std::ostringstream&		GetMStream( __in ENUM_LOGLEVEL	level = ENM_LOG_INFO );
	std::ostringstream&		GetBaseMStream();

public:
	static void				Initialize( __in std::string strMode = "DEBUG1" );
	static void				SetPath( __in std::string strPath );
	static ENUM_LOGLEVEL&	ReportingLevel();
	static std::string			ToString( __in ENUM_LOGLEVEL level );
	static ENUM_LOGLEVEL	FromString( __in const std::string& level );
};

#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL	ENM_LOG_DEBUG4
#endif

// Debug
#define __FILENAME__ (strrchr(__FILE__,'\\')+1) 
//#define CONSOLE_LOG_CHECK_CPP( _x, ...) printf("[%s:%d] "_x"\n", __FILENAME__, __LINE__, __VA_ARGS__) 

#define FILE_LOG(level) \
	if (level > FILELOG_MAX_LEVEL) ;\
	else if (level > CLog::ReportingLevel() || !CLogFile::Stream()) ;\
	else CLog().GetStream(level)

#define CONSOLE_LOG(level) \
	if (level > FILELOG_MAX_LEVEL) ;\
	else if (level > CLog::ReportingLevel() || !CLogFile::Stream()) ;\
	else CLog().GetCStream(__FILENAME__, __LINE__, level)

#define COLSOLE_LOG2(level) \
	if (level > FILELOG_MAX_LEVEL) ;\
	else if (level > CLog::ReportingLevel() || !CLogFile::Stream()) ;\
	else CLog().GetCStream(level) 

#define MULTI_LOG(level)\
	if (level > FILELOG_MAX_LEVEL) ;\
	else if (level > CLog::ReportingLevel() || !CLogFile::Stream()) ;\
	else  CLog().GetMStream(__FILENAME__, __LINE__, level)

#define MULTI_LOG2(level)\
	if (level > FILELOG_MAX_LEVEL) ;\
	else if (level > CLog::ReportingLevel() || !CLogFile::Stream()) ;\
	else CLog().GetMStream(level)

#define MULTI_LOG3()\
	CLog().GetBaseMStream()

#define CREATE_FOLDER_PATH( string ) \
	CLogFile::CreateFolder( string )

//#define MULTI_FILELOG( logfileselect )\
//	if (level > FILELOG_MAX_LEVEL) ;\
//	else if (level > CLog::ReportingLevel() || !CLogFile::Stream()) ;\
//	else  CLog().GetMStream(__FILENAME__, __LINE__, level)
