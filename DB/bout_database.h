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
	char x;
	std::string table_name;
	std::string unverified_table;
	std::string verified_table;
	bool Prover_Switch = false;

	bout_database();
	~bout_database();
	void select_database(char* order, string &CID, string &HASH, string &SIGNED_HASH);
	void insert_database(char* CID, char* Hash, char* Signed_Hash);
	void insert_pk_database(string key_ID, char* key_value);
	string get_latest_key_ID(char* order);
	void get_list(vector<string> &list, string table, string first_cid, string last_cid, int Switch);
	MYSQL* mysql_connection_setup(struct db_user sql_user);
	MYSQL_RES* mysql_perform_query(MYSQL *connection, char *sql_query);
	void get_table_name();
	void initDatabase(struct db_user *db_info);
	void update_database(char* order);
	//string verified_table();
	void print_query();
};

bout_database::bout_database(){
	Read_DB_cfg();
	get_table_name();
	unverified_table = "unverified_frame";
	verified_table = "verified_frame";
	string image_dir_name(image_dir);
	image_dir_name = image_dir_name + this->table_name;
	x = this->table_name[8];
	if(ThisID == Server)mkdir_func(image_dir_name);

    initDatabase(&mysqlID);
	conn = mysql_connection_setup(mysqlID);
	//print_query();
}

bout_database::~bout_database(){
	mysql_free_result(res);
	mysql_close(conn);
}

void bout_database::get_table_name(){
	struct timeb tb;   // <sys/timeb.h>                       
    struct tm tstruct;                      
    std::ostringstream oss;   
                           
    char buf[128];                                            
                                                              
    ftime(&tb);
    // For Thread safe, use localtime_r
    if (nullptr != localtime_r(&tb.time, &tstruct)) {         
        strftime(buf, sizeof(buf), "%Y_%m%d", &tstruct);
        oss << buf; // YEAR_MMDD
    }              

    this->table_name = oss.str();
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
}

void bout_database::insert_database(char* CID, char* Hash, char* Signed_Hash){
	string sorder = "INSERT INTO " + unverified_table + " values('" + CID + "', '" + Hash + "', '" + Signed_Hash + "');";
	cout << sorder << endl;
	char *order = new char[sorder.length() + 1];
	strcpy(order, sorder.c_str());
	res = mysql_perform_query(conn, order);
}

void bout_database::insert_pk_database(string key_ID, char* key_value){
	string sorder = "INSERT INTO public_key values('" + key_ID + "', '" + key_value + "', 1);";
	char *order = new char[sorder.length() + 1];
	strcpy(order, sorder.c_str());
	res = mysql_perform_query(conn, order);
}

void bout_database::update_database(char* order){
	res = mysql_perform_query(conn, order);
	cout << endl << "---------------------------------------------------" << endl;
	while((row = mysql_fetch_row(res)) != NULL){
		string x = row[0];
		cout << x << endl;
	}
}

string bout_database::get_latest_key_ID(char* order){
	res = mysql_perform_query(conn, order);
	string key_ID;
	while((row = mysql_fetch_row(res)) != NULL){
		key_ID = row[0];
		cout << "key_ID : " << key_ID << endl;
	}
	
	return key_ID;
}

void bout_database::get_list(vector<string> &list, string table, string first_cid, string last_cid, int Switch){
	string sorder;
	if(Switch == 1)
		sorder = "select key_ID from " + table + " where '" + first_cid + "' < key_ID and key_ID< '" + last_cid + "' order by key_ID;";
	else if(Switch == 0)
		sorder = "select CID from " + table + " where '" + first_cid + "' <= CID and CID <= '" + last_cid + "' order by CID;";
	else if(Switch == -1)
		sorder = "select key_ID from " + table + " where '" + first_cid + "' < key_ID and key_ID < '" + last_cid + "' order by key_ID desc limit 1;";
	else{
		cout << "get_list() has something wrong" << endl;
		exit(1);
	}
	char *order;
	order = new char[sorder.length() + 1];
	strcpy(order, sorder.c_str());
	res = mysql_perform_query(conn, order);

	while((row = mysql_fetch_row(res)) != NULL){
		list.push_back(row[0]);
	}
}

void bout_database::print_query(){
	string sorder = "select * from " + table_name + ";";
	char *order = new char[sorder.length() + 1];
	strcpy(order, sorder.c_str());
	res = mysql_perform_query(conn, order);

	while((row = mysql_fetch_row(res)) != NULL){
		cout <<  "CID : " << row[0] << endl;
		cout <<  "Hash : " << row[1] << endl;
		cout <<  "Signed Hash : " << row[2] << endl;

	}
}