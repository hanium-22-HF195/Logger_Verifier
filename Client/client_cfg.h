#include <iostream>
#include <string>
#include <fstream>
#include <jsoncpp/json/json.h>

// CHANGE SERVER_IP_ADDR to RPI's IP that working as SERVER
# define SERVER_IP_ADDR     "127.0.0.1"
# define SERVER_PORT        8700

// Deafault values for datasize
# define Hash_size          64
# define Signed_Hash_size   350
# define CID_size           23

// Communication protocol
#define CMD_HDR_SIZE        8
#define ASYNC_BUFSIZE       4096
#define MAX_USER_CNT        5
//----------------------------------

#define ThisID Logger

#pragma comment(lib, "jsoncpp.lib")

void Read_client_cfg(){
	ifstream json_dir("Sys_cfg.json");
	Json::CharReaderBuilder builder;
	builder["collectComments"] = false;
	Json::Value value;

	JSONCPP_STRING errs;
	bool ok = parseFromStream(builder, json_dir, &value, &errs);

	if(ok == true){
		SERVER_PORT = value["Server"]["Server Port"].get("Int", -1).asInt();
        string raw_dir = value["Server"]["storage dir"].get("Double", "Empty").asString();
        storage_dir = new char[raw_dir.length() + 1];
        strcpy(storage_dir, raw_dir.c_str());

        Hash_size = value["Default data size"]["Hash size"].get("Int", -1).asInt();
        Signed_Hash_size = value["Default data size"]["Signed hash size"].get("Int", -1).asInt();
        CID_size = value["Default data size"]["CID size"].get("Int", -1).asInt();

        CMD_HDR_SIZE = value["HeaderPacket"]["Command Header size"].get("Int", -1).asInt();
        ASYNC_BUFSIZE = value["HeaderPacket"]["ASYNC BUFSIZE"].get("Int", -1).asInt();
        MAX_USER_CNT = value["HeaderPacket"]["Max User Count"].get("Int", -1).asInt();
	}
}

