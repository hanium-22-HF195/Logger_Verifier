#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <map>
#include <sys/stat.h>
#include <sys/types.h>
#include <../Server/util.h>

#define Hash_size_D 64
#define Signed_Hash_size_D 350
#define CID_size_D 23

#define SV

using namespace std;
void mkdir_func(string str);

HEADERPACKET for_res_packet;

void make_res_Packet(uint8_t destID, uint8_t cmd, uint8_t dataType, uint32_t dataSize)
{
	for_res_packet.startID = ThisID;
	for_res_packet.destID = destID;
	for_res_packet.command = cmd;
	for_res_packet.dataType = dataType;
	for_res_packet.dataSize = dataSize;
}

//string s_dir("/home/cloud/images");
// Modified by yhkang
string s_dir = sStorage_dir;


/*
 dataType : 0xa0 = char
			0xa1 = unsigned char
			0xb0 = int
			0xb1 = unsigned int
*/
void reshape_buffer(void* recv_buf, int type, int datasize){
	switch(type){
		case 0xa0 : case 0xc0 : 
					recv_buf = (char *)malloc(datasize);
					break;
		case 0xa1 : recv_buf = (unsigned char*)recv_buf;
					recv_buf = new unsigned char[datasize];
					break;
		case 0xb0 : recv_buf = (int *)recv_buf;
					recv_buf = new int[datasize];
					break;
		case 0xb1 : recv_buf = (unsigned int*)recv_buf;
					recv_buf = new unsigned int[datasize];
					break;
	}
}

/*------------------hi i am & nice to meet you----------------------------*/
int hi_i_am(HEADERPACKET* msg, IO_PORT *port){
	//cout << "HEADERPACKET received" << end;
	insert_port(msg->destID, port->s);
	void* p_packet = &for_res_packet;
	make_res_Packet(msg->startID, NICE_2_MEET_U, 0x00, 0x00);

	send_binary(port, sizeof(HEADERPACKET), p_packet);
	return 1;
}
int nice_to_meet_you(HEADERPACKET* msg, IO_PORT *port){
	cout << "Connect success to Server" << endl;
	
	return 1;
}
/*------------------------------------------------------------------------*/

/*------------------public key request & response----------------------------*/
int public_key_request(HEADERPACKET* msg, IO_PORT *port){
	unsigned char* recv_buf = (unsigned char*)malloc(msg->dataSize);
	
	if(recv_binary(port, msg->dataSize, recv_buf) == 0){
		cout << "recv_binary fail" << endl;
		return -1;
	}  // retry
	#ifdef SV
	string pk((char*)recv_buf);

	string sLID = insert_public_key(pk);
	cout << "LID : " << sLID << endl;

	char* LID = const_cast<char*>(sLID.c_str());

	make_res_Packet(msg->startID, PUBKEY_RES, 0xa0, strlen(LID));
	void* p_packet = &for_res_packet;
	insert_port(msg->destID, port->s);

	if(!send_binary(port, sizeof(HEADERPACKET), p_packet)){
		cout << "LID send Error!!" << endl;
	}
	if(!send_binary(port, strlen(LID), LID)){
		cout << "LID send Error!!" << endl;
	}
	#endif
	free(recv_buf);
	
	return 1;
}
int public_key_response(HEADERPACKET* msg, IO_PORT *port){
	cout << "public key response" << endl;
	unsigned char* recv_buf = (unsigned char*)malloc(msg->dataSize);
	
	if(recv_binary(port, msg->dataSize, recv_buf) == 0){
		cout << "recv_binary fail" << endl;
		return -1;
	}

	cout << "received LID : " << recv_buf << endl;
	
	return 1;
}
/*------------------------------------------------------------------------*/

/*-------------------video data send & response---------------------------*/
int video_data_send(HEADERPACKET* msg, IO_PORT *port){
	unsigned char* recv_buf = (unsigned char*)malloc(msg->dataSize);
	
	char* Hash = new char[Hash_size_D];
	char* Signed_Hash = new char[Signed_Hash_size_D];
	char* CID = new char[CID_size_D];

	memset(recv_buf, 0, msg->dataSize);
	memset(CID, 0, CID_size_D);
	memset(Hash, 0, Hash_size_D);
	memset(Signed_Hash, 0, Signed_Hash_size_D);

	int frame_size =  msg->dataSize - CID_size_D - Hash_size_D - Signed_Hash_size_D;
	FILE *file;

	recv_binary(port, CID_size_D, (void*)recv_buf);
	strcpy(CID, (char*)recv_buf);

	string frame_dir((const char*)recv_buf);

	cout << "F storage dir : " << sStorage_dir.c_str() << endl;

	frame_dir = sStorage_dir + "/" + frame_dir; 


	//frame_dir = s_dir(storage_dir) + "/" + frame_dir; 
	
	const char* file_name = frame_dir.c_str();

	file = fopen(file_name, "wb");

	if (file == NULL)
		cout << " [command_parser.cpp/video_data_send] file creation failed " << file_name << endl;

	memset(recv_buf, 0, msg->dataSize);
	recv_binary(port, Hash_size_D, (void*)recv_buf);
	strcpy(Hash, (char*)recv_buf);

	memset(recv_buf, 0, msg->dataSize);
	recv_binary(port, Signed_Hash_size_D, (void*)recv_buf);
	strcpy(Signed_Hash, (char*)recv_buf);

	memset(recv_buf, 0, msg->dataSize);
	recv_binary(port, frame_size, (void*)recv_buf);
	fwrite(recv_buf, sizeof(char), frame_size, file);

	make_res_Packet(Logger, VIDEO_DATA_RES, 0, 0);

	#ifdef SV
	cout << "Insert a metadata of video frame received" << endl;
	insert_video_data(CID, Hash, Signed_Hash);
	#endif

	fflush(file);
	fclose(file);

	free(recv_buf);
	free(CID);
	free(Hash);
	free(Signed_Hash);
	void* p_packet = &for_res_packet;

 	send_binary(port, sizeof(HEADERPACKET), p_packet);
	
	return 1;
}
int video_data_response(HEADERPACKET* msg, IO_PORT *port){
	cout << "video data response recv" << endl;
	return 1;
}
/*------------------------------------------------------------------------*/

/*-----------------------Verify request------------------------*/
int verify_request(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}
int verify_response(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}
int verify_to_prover(HEADERPACKET *msg, IO_PORT *port){
	return 1;
}
int prover_response(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
/* 																		  */
int generate_aes(HEADERPACKET* msg, IO_PORT *port){
	#ifdef SV
	generate_shares();
	#endif
	
	return 1;
}
int share_request(HEADERPACKET* msg, IO_PORT *port){
	unsigned char* recv_buf = (unsigned char*)malloc(msg->dataSize);

	if(recv_binary(port, msg->dataSize, recv_buf) == 0){
		cout << "recv_binary fail" << endl;
		return -1;
	}

	int LID = atoi((char*)recv_buf);
	
	cout << "LID : " <<  LID << endl;

	string share;
	#ifdef SV
	share = get_share(LID);
	#endif
	
	cout << "**share : " << share << endl;
	free(recv_buf);
	char* c_share = const_cast<char*>(share.c_str());

	make_res_Packet(Server, SHARE_RES, 0xa0, strlen(c_share));
	void* p_packet = &for_res_packet;

	if(!send_binary(port, sizeof(HEADERPACKET), p_packet)){
		cout << "share request : send packet error!" << endl;
	}
	if(!send_binary(port, strlen(c_share), (void*)c_share)){
		cout << "share request : send share error!" << endl;
	}

	return 1;
}
int share_response(HEADERPACKET* msg, IO_PORT *port){
	unsigned char* recv_buf = (unsigned char*)malloc(msg->dataSize);

	if(recv_binary(port, msg->dataSize, recv_buf) == 0){
		cout << "recv_binary fail" << endl;
		return -1;
	}

	string share((char*)recv_buf);

	cout << "received share : " << share << endl;

	return 1;
}
int another_share_request(HEADERPACKET* msg, IO_PORT *port){
	unsigned char* recv_buf = (unsigned char*)malloc(msg->dataSize);

	if(recv_binary(port, msg->dataSize, recv_buf) == 0){
		cout << "recv_binary fail" << endl;
		return -1;
	}
	int LID = atoi((char*)recv_buf);
	vector<string> share;
	#ifdef SV
	share = get_ano_shares(LID);
	#endif

	for(vector<string>::iterator iter=share.begin();iter != share.end();iter++){
		cout << *iter << endl;
	}
	free(recv_buf);

	void* p_packet = &for_res_packet;
	make_res_Packet(Server, ANO_SHARE_RES, 0xa0, strlen(share[0].c_str()));

	if(!send_binary(port, sizeof(HEADERPACKET), p_packet)){
		cout << "share request : send packet error!" << endl;
	}

	for(int i = 0; i < share.size(); i++){
		const char *c_share = const_cast<char*>(share[i].c_str());
		cout << "c_share : " << c_share<< endl;

		if(!send_binary(port, strlen(c_share), (void*)c_share)){
			cout << "share request : send share error!" << endl;
		}
	}
	return 1;
}
int another_share_response(HEADERPACKET* msg, IO_PORT *port){
	vector<string> ano_shares;

	for(int i = 0; i < 5 - 1; i ++){
		unsigned char* recv_buf = (unsigned char*)malloc(msg->dataSize);
		if(recv_binary(port, msg->dataSize, recv_buf) == 0){
			cout << "recv_binary fail" << endl;
			return -1;
		}

		string share((char*)recv_buf);
		cout << "received share : " << share << endl;
		ano_shares.push_back(share);
	}
	// generate_symmetric_key(share);

	return 1;
}


/*																		  */
/*------------------------------------------------------------------------*/


/*
 This function is for test. Receive data and write down .txt file. 
*/
int test(HEADERPACKET* msg, IO_PORT *port){
	FILE *file = fopen("test.txt", "wb");
	unsigned char* recv_buf = (unsigned char*)malloc(msg->dataSize);
	
	if(recv_binary(port, msg->dataSize, recv_buf) == 0){
		cout << "recv_binary fail" << endl;
		return -1;
	}
	
	fwrite(recv_buf, sizeof(char), msg->dataSize, file);
	
	fflush(file);
	fclose(file);
	free(recv_buf);

	return 1;
}

int test_gen_shares(HEADERPACKET* msg, IO_PORT *port){
	#ifdef SV
	unsigned char* recv_buf = (unsigned char*)malloc(msg->dataSize);
	
	if(recv_binary(port, msg->dataSize, recv_buf) == 0){
		cout << "recv_binary fail" << endl;
		return -1;
	}
	int res = atoi((char*)recv_buf);
	int nshare = res / 10;
	int threshold = res % 10;

	if(!test_share_gen(nshare, threshold)) return -1;
	#endif
	
	return 1;
}

int gen_share_res(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}

int test_share_req(HEADERPACKET* msg, IO_PORT *port){
	#ifdef SV
	string s_share = test_get_share() + '\0';
	char* c_share = const_cast<char*>(s_share.c_str());
	cout << c_share << endl;

	void* p_packet = &for_res_packet;
	make_res_Packet(Logger, TEST_SHARE_REQ, 0xa0, strlen(c_share));

	if(!send_binary(port, sizeof(HEADERPACKET), p_packet)){
		cout << "share request : send packet error!" << endl;
	}

	if(!send_binary(port, strlen(c_share), (void*)c_share)){
			cout << "share request : send share error!" << endl;
	}
	#endif
}

int test_share_res(HEADERPACKET* msg, IO_PORT *port){
	
}
