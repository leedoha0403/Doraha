#pragma once
// -------------------------------------- Log ------------------------------------ //
#pragma comment(lib, "Log.lib")
//
//#include "../Log/stdafx.h"
//#include "../Log/Log.h"
// --------------------------------------- mysql ���� ���� �⺻ ���� -------------------------------------- //
#include <iostream>


#pragma warning(disable:4996)
#pragma comment(lib, "libmySQL.lib")

#define DB_HOST		"127.0.0.1"	// DB IP�ּ� �Ǵ� ������ �Ǵ� localhost
#define DB_USER		"local"		// DB���� ���� ��
#define DB_PASS		"root"			// DB���� ���� ��ȣ
#define DB_NAME		"local"		// DB �̸�
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
	
	BOOL			PrintResult( std::string tablename );					// console, log type ����
	BOOL			PrintResultType( int type, std::string tablename );		// console, log type ����
	std::string		ReturnResult( std::string tablename );
	
private:
	MYSQL			*connection;
	MYSQL			conn;
	MYSQL_RES		*sql_result;
	MYSQL_ROW		sql_row;
	int					query_stat;
private:
};

