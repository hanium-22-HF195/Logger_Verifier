
#include <pthread.h>
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <mysql.h>
#include <openssl/evp.h>

using namespace std;
#define ThisID Server

#include "server_cfg.h"
#include "../command_parser/command_define_list.h"

#define HANDLE void*

#define TRUE 1
#define FALSE 0
#define INVALID_SOCKET -1

#define SOCKET u_int32_t

#ifndef __SERVERSOCKET_HEADER__
#define __SERVERSOCKET_HEADER__

typedef struct
{
	u_int8_t startID;
	u_int8_t destID;
	u_int8_t command;
	u_int8_t dataType;
	u_int32_t dataSize;
}HEADERPACKET;

enum {
	PORT_TYPE_TCP = 0,
	PORT_TYPE_UDP,
};

typedef struct {
    u_int32_t type;
    u_int32_t s;
    u_int32_t timeout;
    struct sockaddr_in  addr;
	
}IO_PORT;

typedef struct
{
	pthread_t listenThread;
	pthread_t clientThread;

	pthread_mutex_t g_mc_mtx;

	u_int32_t networkLoop;
	u_int32_t recvLoop;
	u_int32_t m_socket;
	IO_PORT port;

}NETWORK_CONTEXT;

int initServer();
void termServer();

int send_binary( IO_PORT *p, long nSize, HANDLE pdata );
int recv_binary( IO_PORT *p, long size, HANDLE pdata );

string getCID();
void mkdir_func(string str);
void insert_port(int ID, int port);
void pop_port(int ID);
void makePacket(uint8_t destID, uint8_t cmd, uint8_t dataType, uint32_t dataSize);
int cmd_parser(IO_PORT port, HEADERPACKET *pmsg);

void generate_shares();
string insert_public_key(string pubkey);
void insert_video_data(char* CID, char* Hash, char* Signed_Hash);
string get_share(int LID);
vector<string> get_ano_shares(int LID);
char* extract_secret_from_share(vector<string> shares);
void generate_AES(string pass);
unsigned char* encrypt_data(char* secret, unsigned char* data);
unsigned char* decrypt_data(char* secret, unsigned char* data);

void encrypt_data();

bool test_share_gen(int nshare, int threshold);
string test_get_share();

NETWORK_CONTEXT getpnetwork();

extern NETWORK_CONTEXT *g_pNetwork;
extern HEADERPACKET sendDataPacket;
extern EVP_CIPHER_CTX *en;
extern EVP_CIPHER_CTX *de;

#endif

