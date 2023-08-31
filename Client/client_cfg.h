#include <iostream>
#include <string>
#include <fstream>
#include <jsoncpp/json/json.h>

using namespace std;

// CHANGE SERVER_IP_ADDR to RPI's IP that working as SERVER
char* SERVER_IP_ADDR;
int SERVER_PORT;
char* storage_dir;

// Deafault values for datasize
int Hash_size;
int Signed_Hash_size;
int CID_size;

// Communication protocol
int CMD_HDR_SIZE;
int ASYNC_BUFSIZE;
int MAX_USER_CNT;
//----------------------------------

int command;

#define ThisID Client

#pragma comment(lib, "jsoncpp.lib")

void Read_client_cfg(){
	ifstream json_dir("../Sys_cfg.json");
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	Json::Value value;

	JSONCPP_STRING errs;
	bool ok = parseFromStream(builder, json_dir, &value, &errs);

	if(ok == true){
		SERVER_PORT = value["Client"]["Server port"].asInt();
        
        string raw_addr = value["Client"]["Server IP addr"].asString();
        SERVER_IP_ADDR = new char[raw_addr.length() + 1];
        strcpy(SERVER_IP_ADDR, raw_addr.c_str());

        string raw_dir = value["Server"]["storage dir"].asString();
        storage_dir = new char[raw_dir.length() + 1];
        strcpy(storage_dir, raw_dir.c_str());

        Hash_size = value["Default data size"]["Hash size"].asInt();
        Signed_Hash_size = value["Default data size"]["Signed hash size"].asInt();
        CID_size = value["Default data size"]["CID size"].asInt();

        CMD_HDR_SIZE = value["HeaderPacket"]["Command Header size"].asInt();
        ASYNC_BUFSIZE = value["HeaderPacket"]["ASYNC BUFSIZE"].asInt();
        MAX_USER_CNT = value["HeaderPacket"]["Max User Count"].asInt();

        command = value["command for test"].asInt();
	}
}

