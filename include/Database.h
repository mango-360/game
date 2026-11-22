#pragma once

#include "defines.h"

static const string SQL_FOLDER = "sql\\";

class Database
{
public:
	~Database();

	bool connect(string configFile);
	void disconnect();
	bool isConnected() { return m_connected; };

	// Executes one SQL command, e.g. execute("DELETE FROM dbo.TestData")
	bool execute(string sql);

	// Runs a SELECT and returns the rows, every value as text
	// e.g. select("SELECT Id, PlayerName FROM dbo.TestData")[0][1] -> name of the first row
	vector<vector<string>> select(string sql);

	// Executes a .sql file from the sql folder
	bool executeScript(string scriptFile);

private:
	void* m_env = nullptr;
	void* m_dbc = nullptr;
	bool m_connected = false;
};
