#pragma once
// -------------------------------------- Log ------------------------------------ //
#pragma comment(lib, "Log.lib")
//
//#include "../Log/stdafx.h"
//#include "../Log/Log.h"
// --------------------------------------- mysql 연동 관련 기본 설정 -------------------------------------- //
#include <iostream>


#pragma warning(disable:4996)
#pragma comment(lib, "libmySQL.lib")

#define DB_HOST		"127.0.0.1"	// DB IP주소 또는 도메인 또는 localhost
#define DB_USER		"local"		// DB접속 계정 명
#define DB_PASS		"root"			// DB접속 계정 암호
#define DB_NAME		"local"		// DB 이름
// ----------------------------------------------------------------------------------------------------------------- //

#pragma once

enum ENM_ERRORCODE
{
	NONE_ERROR		= 0,

	NONE_DATABASE	= 10,
	NONE_TABLE,
	NONE_VALUE,
};

enum ENM_LOGTYPE
{
	DB_NONE			= 0,
	
	DB_CONSOLE,
	DB_FILE,
	DB_MULTI,
};
// ----------------------------------------------------------------------------------------------------------------- //

#pragma once

#define DBINST		CDataBases::Instance()

#include <my_global.h>
#include <string>
#include <mysql.h>

extern class CDataBases
{
private:
	CDataBases(void);	
public:
	virtual			~CDataBases(void);
	static			CDataBases* Instance();
	
	BOOL			InitializeDatabase();
	BOOL			InitializeDatabase( std::string host, std::string user, std::string pass, std::string dbname );

	BOOL			SelectQuery( std::string query );

	BOOL			CreateTable( std::string query );
	BOOL			CreateTable( std::string tablename, std::string valuetype );
	BOOL			DropTable( std::string tablename );

	BOOL			InsertQuery( std::string query );
	BOOL			InsertData( std::string tablename, std::string value );
	BOOL			RemoveTable(std::string tablename);
	
	BOOL			PrintResult( std::string tablename );					// console, log type 설정
	BOOL			PrintResultType( int type, std::string tablename );		// console, log type 설정
	std::string		ReturnResult( std::string tablename );
	
private:
	MYSQL			*connection;
	MYSQL			conn;
	MYSQL_RES		*sql_result;
	MYSQL_ROW		sql_row;
	int					query_stat;
private:
};

