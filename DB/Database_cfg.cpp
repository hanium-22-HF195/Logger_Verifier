#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <jsoncpp/json/json.h>

#pragma comment(lib, "jsoncpp.lib")
using namespace std;

#ifndef __DATABASE_CFG__
#define __DATABASE_CFG__

char *DB_IP;
char *DB_user;
char *DB_password;
char *DB_database;
string image_dir;

int CMD_HDR_SIZE_D;
int Hash_size_D;
int Signed_Hash_size_D;
int CID_size_D;

void Read_DB_cfg(){
        ifstream json_dir("../Sys_cfg.json");
        Json::CharReaderBuilder builder;
        builder["collectComments"] = false;
        Json::Value value;

        JSONCPP_STRING errs;
        bool ok = parseFromStream(builder, json_dir, &value, &errs);

        if(ok == true){
                string raw_string;
                raw_string = value["Database"]["DB IP"].asString();
                DB_IP = new char[raw_string.length() + 1];
                strcpy(DB_IP, raw_string.c_str());

                raw_string = value["Database"]["DB user"].asString();
                DB_user = new char[raw_string.length() + 1];
                strcpy(DB_user, raw_string.c_str());

                raw_string = value["Database"]["DB password"].asString();
                DB_password = new char[raw_string.length() + 1];
                strcpy(DB_password, raw_string.c_str());

                raw_string = value["Database"]["DB database"].asString();
                DB_database = new char[raw_string.length() + 1];
                strcpy(DB_database, raw_string.c_str());

                image_dir = value["Database"]["storage dir"].asString();

                CMD_HDR_SIZE_D = value["HeaderPacket"]["Command Header size"].asInt();
                Hash_size_D = value["Default data size"]["Hash size"].asInt();
                Signed_Hash_size_D = value["Default data size"]["Signed hash size"].asInt();
                CID_size_D = value["Default data size"]["CID size"].asInt();
        }
}

#endif
