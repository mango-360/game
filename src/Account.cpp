#include "Account.h"
#include "World.h"
#include <cstdio>

extern World world;

static const string SESSION_FILE = CONFIG_FOLDER + "session.txt";

// SQL that hashes the password, so the real password is never stored
// Names and passwords have only letters and digits, so they are safe to put straight in the SQL
static string passwordHash(string name, string password)
{
	return "HASHBYTES('SHA2_256', '" + name + ":" + password + "')";
}

// Loads the player that matches the condition into world, e.g. loadPlayer("Name = 'Ivan'")
static bool loadPlayer(string where)
{
	vector<vector<string>> rows = world.m_database.select("SELECT Id, Name, Skin FROM dbo.Players WHERE " + where);

	if (rows.empty()) return false;

	world.m_playerId = stoi(rows[0][0]);
	world.m_playerName = rows[0][1];
	world.m_playerSkin = stoi(rows[0][2]);

	if (world.m_playerSkin < 0 || world.m_playerSkin >= SKIN_COUNT) world.m_playerSkin = 0;

	cout << "Logged in as " << world.m_playerName << endl;
	return true;
}

// Gives the player a new random token and saves it in a file, so the next start logs in automatically
static void rememberLogIn()
{
	string id = to_string(world.m_playerId);

	world.m_database.execute("UPDATE dbo.Players SET SessionToken = NEWID() WHERE Id = " + id);
	vector<vector<string>> rows = world.m_database.select("SELECT SessionToken FROM dbo.Players WHERE Id = " + id);

	if (rows.empty()) return;

	ofstream file(SESSION_FILE);
	file << rows[0][0];
}

namespace Account
{
	bool signUp(string name, string password, string& error)
	{
		if (!world.m_database.isConnected()) { error = "No connection to the database"; return false; }
		if (name.empty() || password.empty()) { error = "Enter a name and a password"; return false; }
		if (password.size() < 4) { error = "The password must be at least 4 characters"; return false; }

		string sqlName = "'" + name + "'";

		if (!world.m_database.select("SELECT Id FROM dbo.Players WHERE Name = " + sqlName).empty())
		{
			error = "This name is already taken";
			return false;
		}

		if (!world.m_database.execute("INSERT INTO dbo.Players (Name, PasswordHash) VALUES ("
			+ sqlName + ", " + passwordHash(name, password) + ")"))
		{
			error = "Database error (see console)";
			return false;
		}

		loadPlayer("Name = " + sqlName);
		rememberLogIn();
		return true;
	}

	bool logIn(string name, string password, string& error)
	{
		if (!world.m_database.isConnected()) { error = "No connection to the database"; return false; }
		if (name.empty() || password.empty()) { error = "Enter a name and a password"; return false; }

		if (!loadPlayer("Name = '" + name + "' AND PasswordHash = " + passwordHash(name, password)))
		{
			error = "Wrong name or password";
			return false;
		}

		rememberLogIn();
		return true;
	}

	bool tryAutoLogIn()
	{
		ifstream file(SESSION_FILE);
		string token;
		file >> token;

		if (token.empty()) return false;

		// A token looks like 6F9619FF-8B86-D011-B42D-00C04FC964FF
		for (char c : token)
			if (!isxdigit(c) && c != '-') return false;

		return loadPlayer("SessionToken = '" + token + "'");
	}

	void logOut()
	{
		world.m_database.execute("UPDATE dbo.Players SET SessionToken = NULL WHERE Id = " + to_string(world.m_playerId));
		remove(SESSION_FILE.c_str());

		world.m_playerId = -1;
		world.m_playerName = "";
		world.m_playerSkin = 0;
	}

	void setSkin(int skin)
	{
		world.m_playerSkin = skin;
		world.m_database.execute("UPDATE dbo.Players SET Skin = " + to_string(skin)
			+ " WHERE Id = " + to_string(world.m_playerId));
	}
}
