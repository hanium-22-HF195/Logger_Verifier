#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <map>
#include <sys/stat.h>
#include <sys/types.h>

#include "../openssl/sign.h"
#include "../DB/bout_database.h"

using namespace std;
void mkdir_func(string str);

HEADERPACKET for_res_packet;
void* p_packet = &for_res_packet;

void make_res_Packet(uint8_t destID, uint8_t cmd, uint8_t dataType, uint32_t dataSize)
{
	for_res_packet.startID = ThisID;
	for_res_packet.destID = destID;
	for_res_packet.command = cmd;
	for_res_packet.dataType = dataType;
	for_res_packet.dataSize = dataSize;
}

void* recv_buf;
char* Hash = new char[Hash_size_D];
char* Signed_Hash = new char[Signed_Hash_size_D];
char* CID = new char[CID_size_D];
FILE* file;
bout_database bDB;

string s_dir(image_dir);

/*
 dataType : 0xa0 = char
			0xa1 = unsigned char
			0xb0 = int
			0xb1 = unsigned int
*/
void reshape_buffer(int type, int datasize){
	switch(type){
		case 0xa0 : case 0xc0 : 
					recv_buf = (char *)recv_buf;
					recv_buf = new char[datasize];
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

int term_socket(HEADERPACKET* msg, IO_PORT *port){
	cout << "socket " << port->s << " terminated" << endl;
	return 0;
}

/*------------------hi i am & nice to meet you----------------------------*/
int hi_i_am(HEADERPACKET* msg, IO_PORT *port){
	//cout << "HEADERPACKET received" << end;
	insert_port(msg->destID, port->s);
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
	reshape_buffer(msg->dataType, msg->dataSize);
	
	if(recv_binary(port, msg->dataSize, recv_buf) == 0){
		cout << "recv_binary fail" << endl;
		return -1;
	}

	string key_ID = bDB.get_latest_key_ID();
	
	string filed = "key_ID";
	string set = "key_status = 0";
	string table = "public_keys";
	string where = "key_ID = '" + key_ID +"'";
	bDB.update_database(key_ID, set, table, where);

	string pk((char*)recv_buf);
	char *key_value = new char[pk.length() + 1];
	strcpy(key_value, pk.c_str());
	bDB.insert_pk_database(getCID(), 0,key_value);

	return 1;
}
int public_key_response(HEADERPACKET* msg, IO_PORT *port){
	make_res_Packet(Logger, PUBKEY_RES, 0xa0, 0);
	
	return send_binary(port, CMD_HDR_SIZE_D, p_packet);
}
/*------------------------------------------------------------------------*/

/*-------------------video data send & response---------------------------*/
int video_data_send(HEADERPACKET* msg, IO_PORT *port){
	reshape_buffer(msg->dataType, msg->dataSize);

	memset(recv_buf, 0, msg->dataSize);
	memset(CID, 0, CID_size_D);
	memset(Hash, 0, Hash_size_D);
	memset(Signed_Hash, 0, Signed_Hash_size_D);

	int frame_size =  msg->dataSize - CID_size_D - Hash_size_D - Signed_Hash_size_D;
	FILE *file;

	recv_binary(port, CID_size_D, (void*)recv_buf);
	strcpy(CID, (char*)recv_buf);

	string frame_dir((const char*)recv_buf);
	frame_dir = s_dir + "/" + frame_dir; 
	const char* file_name = frame_dir.c_str();

	file = fopen(file_name, "wb");

	if (file == NULL)
		cout << "file creation failed " << endl;

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
	bDB.insert_video_data(CID, Hash, Signed_Hash);

	fflush(file);
	fclose(file);

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
	// if(msg->destID == Server){
	// 	reshape_buffer(msg->dataType, msg->dataSize);

	// 	//Receive Start CID from WebUI
	// 	recv_binary(port, msg->dataSize, (void*)recv_buf);
	// 	string first_cid((char*)recv_buf);

	// 	//Receive End CID from WebUI
	// 	recv_binary(port, msg->dataSize, (void*)recv_buf);
	// 	string last_cid((char*)recv_buf);

	// 	if(first_cid > last_cid){
	// 		first_cid.swap(last_cid);
	// 	}

	// 	vector<string> pk_list;
	// 	vector<string> CID_list;
	// 	map<string, vector<string>> key_CID_map;

	// 	CIDINFO start_cid = 
	// 	{
	// 		first_cid.substr(0, 4),
	// 		first_cid.substr(5, 2),
	// 		first_cid.substr(8, 2),
	// 		first_cid.substr(11, 2),
	// 		first_cid.substr(14, 2),
	// 		first_cid.substr(17, 2),
	// 		first_cid.substr(20,3),
	// 	};

	// 	CIDINFO end_cid = 
	// 	{
	// 		last_cid.substr(0, 4),
	// 		last_cid.substr(5, 2),
	// 		last_cid.substr(8, 2),
	// 		last_cid.substr(11, 2),
	// 		last_cid.substr(14, 2),
	// 		last_cid.substr(17, 2),
	// 		last_cid.substr(20,3),
	// 	};

	// 	if(start_cid.Day != end_cid.Day){
	// 		vector<string> table_list;
	// 		int j = stoi(end_cid.Day) - stoi(start_cid.Day);
	// 		for(int i = 0; i <= j; i++){
	// 			int d = i + stoi(start_cid.Day);
	// 			string x_table = start_cid.Year + "_" + start_cid.Month + to_string(d);
	// 			table_list.push_back(x_table);
	// 		}
	// 	}
	// 	else{
	// 		string vtable_name = start_cid.Year + '_' + start_cid.Month + start_cid.Day;
			
	// 		bDB.get_list(pk_list, "public_key", "-1", first_cid, -1);
	// 		bDB.get_list(pk_list, "public_key", first_cid, last_cid, 1);

	// 		bDB.get_list(CID_list, vtable_name, first_cid, pk_list[1], 0);
	// 		key_CID_map[pk_list[0]] = CID_list;
	// 		CID_list.clear();
	// 		for(int i = 1; i < pk_list.size() - 1; i++){
	// 			bDB.get_list(CID_list, vtable_name, pk_list[i], pk_list[i + 1], 0);
	// 			key_CID_map[pk_list[i]] = CID_list;
	// 			CID_list.clear();
	// 		}

	// 		bDB.get_list(CID_list, vtable_name, pk_list[pk_list.size()-1], last_cid, 0);
	// 		key_CID_map[pk_list[pk_list.size()-1]] = CID_list;
	// 		CID_list.clear();
	// 	}

	// 	map<string, vector<string>>::iterator iter;
	// 	for (iter = key_CID_map.begin(); iter != key_CID_map.end(); ++iter) { //�??��
	// 		vector<string> inVect = (*iter).second;
	// 		int str_size = CID_size_D * (inVect.size() + 1);

	// 		make_res_Packet(Verifier, VER_REQ, Uchar, str_size);
	// 		//send_binary(Verifier_port, sizeof(HEADERPACKET), p_packet);

	// 		unsigned char *PK = new unsigned char[CID_size];
	// 		strcpy((char*)PK, (*iter).first.c_str());
			
	// 		//send_binary(Verifier_port, CID_size, (void*)PK);

	// 		unsigned char *image_CID = new unsigned char[CID_size];
	// 		for (int j = 0; j < inVect.size(); j++) {
	// 			strcpy((char*)image_CID, inVect[j].c_str());
	// 			//send_binary(Verifier_port, CID_size, image_CID);
	// 		}
	// 	}
	// }
	// else if(msg->destID == Verifier){
	// 	cout <<"hello:";
	// }
	// else{
	// 	cout << "Something was wrong..." << endl;
	// 	exit(1);
	// }
	return 1;
}
int verify_response(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}
int verify_to_prover(HEADERPACKET *msg, IO_PORT *port){
	bDB.Prover_Switch = true;
	return 1;
}
int prover_response(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
/* 																		  */
int generate_aes(HEADERPACKET* msg, IO_PORT *port){
	// cout << "generate symmetric key" << endl;
	// string skey = "seed key";

    // EVP_CIPHER_CTX *en = EVP_CIPHER_CTX_new();
    // EVP_CIPHER_CTX *de = EVP_CIPHER_CTX_new();

	// char *key_data = const_cast<char*>(skey.c_str());
	// int key_data_len = skey.length() + 1;

	// unsigned char key[32], iv[32];
	// cout << key << endl;
	// cout << iv << endl;

	// aes_init((unsigned char*)key_data, key_data_len, NULL, en, de, key, iv);
	// cout << "symmetric key generation complete" << endl;

	// char *shares = generate_share_strings((char*)key, 3, 2);
	// fprintf(stdout, "%s", shares);

	// free(shares);
	generate_shares();
	const char *ptxt = "Hello world!";

	int len = strlen(ptxt) + 1;;

	unsigned char *ciphertext = aes_encrypt(en, (unsigned char *)ptxt, &len);
	cout << "cipher text : " << ciphertext << endl;

	unsigned char *plaintext = aes_decrypt(de, ciphertext, &len);
	cout << "plain text : " << plaintext << endl;

	return 1;
}

int generate_shares(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}
int share_request(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}
int share_response(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}
int another_share_request(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}
int another_share_response(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}
int generate_key_at_logger(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}
int encrypt_data_at_logger(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}
int encrypt_data_request(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}
int encrypt_data_response(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}
int decrypt_data_at_server(HEADERPACKET* msg, IO_PORT *port){
	return 1;
}



/*																		  */
/*------------------------------------------------------------------------*/









/*
 This function is for test. Receive data and write down .txt file. 
*/
int test(HEADERPACKET* msg, IO_PORT *port){
	FILE *file = fopen("test.txt", "wb");
	reshape_buffer(msg->dataType, msg->dataSize);
	
	if(recv_binary(port, msg->dataSize, recv_buf) == 0){
		cout << "recv_binary fail" << endl;
		return -1;
	}
	
	fwrite(recv_buf, sizeof(char), msg->dataSize, file);
	
	fflush(file);
	fclose(file);

	return 1;
}
