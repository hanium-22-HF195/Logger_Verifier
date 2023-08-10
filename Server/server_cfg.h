#include <iostream>
#include <string>
#include <fstream>
#include <jsoncpp/json/json.h>

#pragma comment(lib, "jsoncpp.lib")

#ifdef THIS_IS_SERVER
//RPI's IP that working as SERVER
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


void Read_server_cfg(){
	ifstream json_dir("../Sys_cfg.json");
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	Json::Value value;

	JSONCPP_STRING errs;
	bool ok = parseFromStream(builder, json_dir, &value, &errs);

	if(ok == true){
		SERVER_PORT = value["Server"]["Server Port"].asInt();
        string raw_dir = value["Server"]["storage dir"].asString();
        storage_dir = new char[raw_dir.length() + 1];
        strcpy(storage_dir, raw_dir.c_str());
        raw_dir = value["Server"]["c-sss dir"].asString();

        Hash_size = value["Server"]["Hash size"].asInt();
        Signed_Hash_size = value["Server"]["Signed hash size"].asInt();
        CID_size = value["Server"]["CID size"].asInt();

        CMD_HDR_SIZE = value["HeaderPacket"]["Command Header size"].asInt();
        ASYNC_BUFSIZE = value["HeaderPacket"]["ASYNC BUFSIZE"].asInt();
        MAX_USER_CNT = value["HeaderPacket"]["Max User Count"].asInt();
	}
}


#endif

