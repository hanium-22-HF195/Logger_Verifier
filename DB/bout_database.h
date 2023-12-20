#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

#ifndef _DATABASE_
#define _DATABASE_
struct db_user {
	char *server;
	char *user;
	char *password;
	char *database;
};

class bout_database{
private:
	struct db_user mysqlID;
	MYSQL *conn;
	MYSQL_ROW row;

public:
	std::string unverified_table;
	std::string verified_table;
	bool Prover_Switch = false;

	bout_database(int typeID);
	~bout_database();
	void select_database(char* order, string &CID, string &HASH, string &SIGNED_HASH);
	void insert_video_data(char* CID, char* Hash, char* Signed_Hash);
	string insert_pk_database(string key_ID, string key_value);
	void insert_aes_key(string generated_date, string key);
	string get_latest_key_ID();
	MYSQL* mysql_connection_setup(struct db_user sql_user);
	MYSQL_RES* mysql_perform_query(MYSQL *connection, char *sql_query);
	void initDatabase(struct db_user *db_info);
	void update_database(string set, string table, string where);
	void print_query();
	void insert_data(string sorder);
	string get_share(int LID);
	vector<string> get_ano_shares(int LID, int limit);
};

#endif