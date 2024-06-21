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
#include "bout_database.h"
#include "Database_cfg.cpp"




void mkdir_func(string str){
	if(mkdir(str.c_str(), 0777) == -1 && errno != EEXIST){
		fprintf(stderr, strerror(errno));
		exit(0);
	}
	else if(errno == EEXIST){}
	else cout << "Create a directory for storing images" << endl;
}

// ThisID 값 전달
bout_database::bout_database(int typeID){
	Read_DB_cfg();
	unverified_table = "Video_data_table";
	verified_table = "verified_frame";
	if(typeID == 0x12)mkdir_func(image_dir);

    initDatabase(&mysqlID);
	conn = mysql_connection_setup(mysqlID);
}

bout_database::~bout_database(){
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

void bout_database::insert_data(string sorder){
	char *order = const_cast<char*>(sorder.c_str());
	mysql_perform_query(conn, order);
}

void bout_database::select_database(char* order, string &CID, string &HASH, string &SIGNED_HASH) {
	MYSQL_RES *res = mysql_perform_query(conn, order);
	cout << "---------------------------------------------------" << endl;
	while((row = mysql_fetch_row(res)) != NULL){
		CID = row[0];
		HASH = row[1];
		SIGNED_HASH = row[2];
	}
	mysql_free_result(res);
} //* CID, HASH, SIGNED_HASH를 반환함*//


void bout_database::insert_video_data(char* CID, char* Hash, char* Signed_Hash){
	string lid_value = "L-01";
	//string sorder = "INSERT INTO " + unverified_table + "(CID, DID, hash, signed_hash)  values('" + CID + "', '" + DID  + "', '" +  Hash + "', '" + Signed_Hash + "');"; //table 형식에 맞춰서 문자열 만들어서 넣어주기
	
	char query[2048];

	sprintf(query,"INSERT INTO %s (CID, LID, hash, signed_hash) values ('%s', '%s', '%s', '%s')", unverified_table.c_str(), CID, lid_value.c_str(), Hash, Signed_Hash);
	
	cout << query << endl;
	char *order = const_cast<char*>(query);
	MYSQL_RES *res = mysql_perform_query(conn, order);
	if(res ==NULL){
		cerr << mysql_error(conn) << endl;
	}
	mysql_free_result(res);
}
//** table name이 고정된 상태. command function 내에서 자체적으로 쿼리 작성 후 수행해도 상관없을듯 함*//

/*
void bout_database::insert_video_data(char* CID, char* Hash, char* Signed_Hash){
	string sorder = "INSERT INTO " + unverified_table + " values('" + CID + "', '" + Hash + "', '" + Signed_Hash + "');";
	cout << sorder << endl;
}
*/ //** table name이 고정된 상태. command function 내에서 자체적으로 쿼리 작성 후 수행해도 상관없을듯 함*//

string bout_database::insert_pk_database(string key_ID, string key_value){
	// LID reset query : 'ALTER TABLE Loggers AUTO_INCREMENT = 0;'
	//string sorder = "INSERT INTO Loggers (LID, pub_key) values(0, '" + key_value + "', '" + key_ID + "');";
	//string sorder = "INSERT INTO Loggers (DID, pub_key) values( '" +did_value  + "', '" + key_value + "');";

	string lid_value = "L-01";

	// 이 부분에  lid_value의 변수에 있는 값이 Loggers 테이블에 LID가 있는지 검색
	char select_query[2028];
	sprintf(select_query,"SELECT COUNT(*) FROM Loggers WHERE LID = '%s'",lid_value.c_str());
	cout << select_query << endl;
	char *select_order = const_cast<char*>(select_query);
	MYSQL_RES *select_res = mysql_perform_query(conn, select_order);
	if(select_res ==NULL){
		cerr << mysql_error(conn) << endl;
	}
	
	MYSQL_ROW checkRow;
	int lidExists = 0;

	if ((checkRow = mysql_fetch_row(select_res)) != NULL) {
		lidExists = atoi(checkRow[0]);  // 결과가 0보다 크면 LID가 존재함
	}
	mysql_free_result(select_res);
	cout << "lidExists : " <<lidExists << endl;


	if (lidExists == 0) { //Loggers 테이블에 LID가 새로운 값이 들어오는 경우

		char query[2048];

		sprintf(query,"INSERT INTO Loggers (LID, pub_key) values( '%s', '%s')", lid_value.c_str(), key_value.c_str());
		cout << query << endl;

		char *order = const_cast<char*>(query);
		MYSQL_RES *res = mysql_perform_query(conn, order);
		if(res ==NULL){
			cerr << mysql_error(conn) << endl;
		}
		mysql_free_result(res);

		insert_public_keys_table(lid_value, key_value);
	}
	else if (lidExists == 1) { //Loggers 테이블에 LID가 현재 같은 값이 있는 경우

		string table_Loggers = "Loggers";
		string set_key_value = "pub_key = '" + key_value +"'";
		string lid_where = "LID = '" + lid_value + "'";
		update_database(set_key_value, table_Loggers, lid_where);

		insert_public_keys_table(lid_value, key_value);
	}
	else {
		cout << "lidExists can only have zero or one" << endl;
	}
	
	






	string LID;
	//sorder = "select max(LID) from Loggers;";
	string sorder;
	sorder = "select max(LID) from Loggers; ";
	char *xorder = const_cast<char*>(sorder.c_str());
	MYSQL_RES *res2 = mysql_perform_query(conn, xorder);

	while((row = mysql_fetch_row(res2)) != NULL){
		LID = row[0];
	}
	mysql_free_result(res2);

	return LID;
}

void bout_database::insert_public_keys_table(string LID, string pub_key){

    char query[2048];
    sprintf(query, "INSERT INTO Public_keys (LID, pub_key) VALUES ('%s', '%s')", LID.c_str(), pub_key.c_str());
    cout << "public_keys query : " << query << endl; 

    char *order = const_cast<char*>(query);
    MYSQL_RES *res = mysql_perform_query(conn, order);
    if (res == NULL) {
        cerr << mysql_error(conn) << endl;
    }
    mysql_free_result(res); 

}

void bout_database::update_database(string set, string table, string where = ""){

	char query[2048];
	sprintf(query,"update %s set %s where %s;",table.c_str(),set.c_str(),where.c_str());

	//string sorder = "update " + table + 
	//				" set " + set + 
	//				" where " + where + ";";

	char *order = const_cast<char*>(query);
	cout << "update query : " << order << endl;
	cout << endl << "---------------------------------------------------" << endl;
	MYSQL_RES *res = mysql_perform_query(conn, order);
	if(res ==NULL){
		cerr << mysql_error(conn) << endl;
	}
	mysql_free_result(res);

}

string bout_database::get_latest_key_ID(){
	char order[] = "select key_ID from public_key where key_status = 1;";
	MYSQL_RES *res = mysql_perform_query(conn, order);
	string key_ID;
	while((row = mysql_fetch_row(res)) != NULL){
		key_ID = row[0];
	}
	mysql_free_result(res);
	return key_ID;
}

string bout_database::get_share(int LID){
	char *order = "select share from shares where LID=-1 limit 1;";
	MYSQL_RES *res = mysql_perform_query(conn, order);

	string share;
	while((row=mysql_fetch_row(res)) != NULL) share = row[0];
	cout << "DB share : " << share << endl;
	string set = "LID = " + LID;
	string table = "shares";
	string where = "share = '" + share + "'";

	update_database(set, table, where);
	mysql_free_result(res);
	return share;
}

vector<string> bout_database::get_ano_shares(int LID, int limit){
	vector<string> shares;
	string S_LID = to_string(LID) ;
	string sorder = "select share from shares where LID != " + S_LID + " limit " + to_string(limit) + ";";
	char *order = const_cast<char*>(sorder.c_str());

	MYSQL_RES *res = mysql_perform_query(conn, order);

	while((row = mysql_fetch_row(res)) != NULL){
		shares.push_back(row[0]);
	}

	mysql_free_result(res);
	return shares;
}

void bout_database::print_query(){
	string sorder = "select * from data_table;";
	char *order = const_cast<char*>(sorder.c_str());
	MYSQL_RES *res = mysql_perform_query(conn, order);

	while((row = mysql_fetch_row(res)) != NULL){
		cout <<  "CID : " << row[0] << endl;
		cout <<  "Hash : " << row[1] << endl;
		cout <<  "Signed Hash : " << row[2] << endl;

	}
	mysql_free_result(res);
}
