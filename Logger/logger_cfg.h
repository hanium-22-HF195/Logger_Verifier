#include <iostream>
#include <string>
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;
// values for Camera Settings
int width;
int height;
int fps;
int frame_count;
int SIGNED_HASH_BUFSIZE;
//----------------------------------
char* pubkeyfile_path;
char* orifile_path;
char* yfile_path;
char* hashfile_path;
//----------------------------------
#define ThisID Logger

#pragma comment(lib, "jsoncpp.lib")

void Read_Logger_cfg(){
	ifstream json_dir("../Sys_cfg.json");
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	Json::Value value;

	JSONCPP_STRING errs;
	bool ok = parseFromStream(builder, json_dir, &value, &errs);

	if(ok == true){
        string str;
        width = value["Logger"]["width"].asInt();
        height = value["Logger"]["height"].asInt();
        fps = value["Logger"]["fps"].asInt();
        frame_count = value["Logger"]["frame count"].asInt();
        SIGNED_HASH_BUFSIZE = value["Logger"]["signed hash bufsize"].asInt();

        str = value["Logger"]["public key path"].asString();
        pubkeyfile_path = new char[str.length() + 1];
        strcpy(pubkeyfile_path, str.c_str());

        str = value["Logger"]["original file path"].asString();
        orifile_path = new char[str.length() + 1];
        strcpy(orifile_path, str.c_str());

        str = value["Logger"]["Y frame file path"].asString();
        yfile_path = new char[str.length() + 1];
        strcpy(yfile_path, str.c_str());

        str = value["Logger"]["hash file path"].asString();
        hashfile_path = new char[str.length() + 1];
        strcpy(hashfile_path, str.c_str());
	}
}

