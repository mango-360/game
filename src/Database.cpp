#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>

#include "Database.h"

// Prints the error message that SQL Server returned
static void printError(SQLSMALLINT handleType, SQLHANDLE handle)
{
	SQLCHAR state[6], message[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER nativeError;
	SQLSMALLINT length;

	if (SQLGetDiagRecA(handleType, handle, 1, state, &nativeError, message, sizeof(message), &length) == SQL_SUCCESS)
		cout << "SQL error: " << message << endl;
}

Database::~Database()
{
	disconnect();
}

bool Database::connect(string configFile)
{
	// The config file holds the connection string on its first line
	string connectionString = "DRIVER={ODBC Driver 17 for SQL Server};SERVER=localhost\\SQLEXPRESS;DATABASE=PlatformerTestDB;Trusted_Connection=yes;";

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_env);
	SQLSetEnvAttr(m_env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	SQLAllocHandle(SQL_HANDLE_DBC, m_env, &m_dbc);

	SQLRETURN ret = SQLDriverConnectA(m_dbc, NULL, (SQLCHAR*)connectionString.c_str(), SQL_NTS,
		NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

	if (!SQL_SUCCEEDED(ret))
	{
		printError(SQL_HANDLE_DBC, m_dbc);
		disconnect();
		return false;
	}

	cout << "Connected to database" << endl;
	m_connected = true;
	return true;
}

void Database::disconnect()
{
	if (m_connected) SQLDisconnect(m_dbc);
	if (m_dbc) SQLFreeHandle(SQL_HANDLE_DBC, m_dbc);
	if (m_env) SQLFreeHandle(SQL_HANDLE_ENV, m_env);

	m_dbc = nullptr;
	m_env = nullptr;
	m_connected = false;
}

bool Database::execute(string sql)
{
	if (!m_connected) return false;

	SQLHSTMT stmt;
	SQLAllocHandle(SQL_HANDLE_STMT, m_dbc, &stmt);

	SQLRETURN ret = SQLExecDirectA(stmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
	bool success = SQL_SUCCEEDED(ret) || ret == SQL_NO_DATA; // SQL_NO_DATA = no rows affected

	if (!success) printError(SQL_HANDLE_STMT, stmt);

	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	return success;
}

vector<vector<string>> Database::select(string sql)
{
	vector<vector<string>> rows;
	if (!m_connected) return rows;

	SQLHSTMT stmt;
	SQLAllocHandle(SQL_HANDLE_STMT, m_dbc, &stmt);

	if (!SQL_SUCCEEDED(SQLExecDirectA(stmt, (SQLCHAR*)sql.c_str(), SQL_NTS)))
	{
		printError(SQL_HANDLE_STMT, stmt);
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return rows;
	}

	SQLSMALLINT columns = 0;
	SQLNumResultCols(stmt, &columns);

	// Go through the result row by row and read every column as text
	while (SQL_SUCCEEDED(SQLFetch(stmt)))
	{
		vector<string> row;

		for (SQLUSMALLINT c = 1; c <= columns; c++)
		{
			char value[1024];
			SQLLEN length;
			SQLGetData(stmt, c, SQL_C_CHAR, value, sizeof(value), &length);

			row.push_back(length == SQL_NULL_DATA ? "" : value); // NULL becomes ""
		}

		rows.push_back(row);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	return rows;
}

bool Database::executeScript(string scriptFile)
{
	fstream stream(SQL_FOLDER + scriptFile);

	if (!stream.is_open())
	{
		cout << "Failed to open sql script: " << scriptFile << endl;
		return false;
	}

	// "GO" works only in SSMS, so the script is sent piece by piece between the GO lines
	string line, batch;

	while (getline(stream, line))
	{
		if (line == "GO" || line == "GO\r")
		{
			if (!execute(batch)) return false;
			batch = "";
		}
		else
		{
			batch += line + "\n";
		}
	}

	return batch.empty() || execute(batch);
}
