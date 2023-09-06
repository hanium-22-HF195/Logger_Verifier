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
#include "Database_cfg.h"

struct db_user {
	char *server;
	char *user;
	char *password;
	char *database;
};

void mkdir_func(string str){
	if(mkdir(str.c_str(), 0777) == -1 && errno != EEXIST){
		fprintf(stderr, strerror(errno));
		exit(0);
	}
	else if(errno == EEXIST){}
	else cout << "Create a directory for storing images" << endl;
}

class bout_database{
private:
	struct db_user mysqlID;
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

public:
	std::string unverified_table;
	std::string verified_table;
	bool Prover_Switch = false;

	bout_database();
	~bout_database();
	void select_database(char* order, string &CID, string &HASH, string &SIGNED_HASH);
	void insert_video_data(char* CID, char* Hash, char* Signed_Hash);
	void insert_pk_database(string key_ID, string LID, char* key_value);
	void insert_aes_key(string generated_date, string key);
	string get_latest_key_ID();
	MYSQL* mysql_connection_setup(struct db_user sql_user);
	MYSQL_RES* mysql_perform_query(MYSQL *connection, char *sql_query);
	void initDatabase(struct db_user *db_info);
	void update_database(string field, string set, string table, string where);
	void print_query();
};

bout_database::bout_database(){
	Read_DB_cfg();
	unverified_table = "data_table";
	verified_table = "verified_frame";
	if(ThisID == Server)mkdir_func(image_dir);

    initDatabase(&mysqlID);
	conn = mysql_connection_setup(mysqlID);
	//print_query();
}

bout_database::~bout_database(){
	mysql_free_result(res);
	mysql_close(conn);
}

void bout_database::initDatabase(struct db_user *db_info){
	db_info->server = DB_IP;
	db_info->user = DB_user;
	db_info->password = DB_password;
	db_info->database = DB_database;
}

MYSQL* bout_database::mysql_connection_setup(struct db_user sql_user){
  MYSQL *connection = mysql_init(NULL);

  if(!mysql_real_connect(connection, sql_user.server, sql_user.user, sql_user.password, sql_user.database, 0, NULL, 0)) {
    printf("Connection error : %s\n", mysql_error(connection)); 
    exit(1);
  }

  return connection;
}

MYSQL_RES* bout_database::mysql_perform_query(MYSQL *connection, char *sql_query) {
	int retry_cnt = 5;
	while(mysql_query(connection, sql_query) != 0){
		if(retry_cnt-- == 0)
			cout << "Failed mysql_perform_query()" << endl;
			break;
	}
  return mysql_use_result(connection);
}

void bout_database::select_database(char* order, string &CID, string &HASH, string &SIGNED_HASH) {
	res = mysql_perform_query(conn, order);
	cout << "---------------------------------------------------" << endl;
	while((row = mysql_fetch_row(res)) != NULL){
		CID = row[0];
		HASH = row[1];
		SIGNED_HASH = row[2];
	}
} //* CID, HASH, SIGNED_HASH를 반환함

void bout_database::insert_video_data(char* CID, char* Hash, char* Signed_Hash){
	string sorder = "INSERT INTO " + unverified_table + " values('" + CID + "', '" + Hash + "', '" + Signed_Hash + "');";
	cout << sorder << endl;
	char *order = const_cast<char*>(sorder.c_str());
	res = mysql_perform_query(conn, order);
} //** table name이 고정된 상태. command function 내에서 자체적으로 쿼리 작성 후 수행해도 상관없을듯 함

void bout_database::insert_pk_database(string key_ID, string LID, char* key_value){
	string sorder = "INSERT INTO public_keys values('" + key_ID + "', '" + key_value + ", 1, " + LID + ");";
	char *order = const_cast<char*>(sorder.c_str());
	res = mysql_perform_query(conn, order);
}

void bout_database::update_database(string field, string set, string table, string where = ""){
	string sorder = "update " + field + 
					" set " + set + 
					" where " + where + ";";
	char *order = const_cast<char*>(sorder.c_str());
	
	res = mysql_perform_query(conn, order);
	cout << endl << "---------------------------------------------------" << endl;
	while((row = mysql_fetch_row(res)) != NULL){
		string x = row[0];
		cout << x << endl;
	}
}

string bout_database::get_latest_key_ID(){

	char order[] = "select key_ID from public_key where key_status = 1;";
	res = mysql_perform_query(conn, order);
	string key_ID;
	while((row = mysql_fetch_row(res)) != NULL){
		key_ID = row[0];
	}
	
	return key_ID;
}

void bout_database::print_query(){
	string sorder = "select * from data_table;";
	char *order = const_cast<char*>(sorder.c_str());
	res = mysql_perform_query(conn, order);

	while((row = mysql_fetch_row(res)) != NULL){
		cout <<  "CID : " << row[0] << endl;
		cout <<  "Hash : " << row[1] << endl;
		cout <<  "Signed Hash : " << row[2] << endl;

	}
}