#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <resolv.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sstream>
#include <mutex>
#include <thread>
#define CMD_BACKGROUND 1
#define THIS_IS_SERVER
#include <ctime>
#include <cstdlib>

#include "server.h"
#include "tracex.h"
#include "../openssl/sign.cpp"
#include "../c-sss/src/shamir.c"
#include "../c-sss/src/strtok.c"
#include "../DB/bout_database.h"

#include "util.h"

using namespace std;

NETWORK_CONTEXT *g_pNetwork;
HEADERPACKET sendDataPacket;
bout_database DB(Server);
vector<string> test_shares(10);
unsigned char key[32], iv[32];

/*--------------------------------------------------------------------------*/
bool test_share_gen(int nshare, int threshold){
	if(test_shares.size() != 0){
		test_shares.clear();
	}
	cout << "nshare : " << nshare << ", threshold : " << threshold << endl;
	initAES(password.c_str(), NULL, key, iv);
	ofstream ofs("symmetric_key", ios::out | ios::app);
	if(ofs.fail()){
		cerr << "Error!" << endl;
		return false;
	}
	cout << "Success in generating AES" << endl;
	char *shares = generate_share_strings((char*)key, nshare, threshold);
	cout << "shares :" << shares << endl;
	istringstream c_shares;
	c_shares.str(shares);
	string substr;

	while(getline(c_shares, substr, '\n')){
		test_shares.push_back(substr);
	}

	vector<string>::iterator iter;
 
	for(iter=test_shares.begin();iter != test_shares.end();iter++){
		cout << "Generated test shares : " << *iter << endl;
	}
	return true;
}

string test_get_share(){
	int a = rand() % test_shares.size();
	return test_shares[a];
}
/*--------------------------------------------------------------------------*/

void closesocket(SOCKET sock_fd);

void cp(string str){
	cout << str << endl;
}

string getCID() {
    struct timeb tb;   // <sys/timeb.h>                       
    struct tm tstruct;                      
    std::ostringstream oss;   
    
    string s_CID;                             
    char buf[128];                                            
                                                              
    ftime(&tb);
    // For Thread safe, use localtime_r
    if (nullptr != localtime_r(&tb.time, &tstruct)) {         
        strftime(buf, sizeof(buf), "%Y-%m-%d_%T.", &tstruct);  
        oss << buf; // YEAR-MM-DD HH-mm_SS            
        oss << tb.millitm; // millisecond               
    }              

    s_CID = oss.str();
    
    s_CID = s_CID.substr(0,23);
    if(s_CID.length() == 22) {
        s_CID = s_CID.append("0");
    }
    if(s_CID.length() == 21) {
        s_CID = s_CID.append("00");
    }
    
    return s_CID;
}

void generate_shares(){
	char *shares = generate_share_strings(&*password.begin(), num_of_share, key_threshold);
	// -> make share with key and iv??
	istringstream c_shares;
	c_shares.str(shares);
	string substr;
	vector<string> share;

	while(getline(c_shares, substr, '\n')){
		share.push_back(substr);
	}

	vector<string>::iterator iter;
 
	for(iter=share.begin();iter != share.end();iter++){
		substr = "insert into shares value('" + *iter + "', -1);";
		DB.insert_data(substr);
	}
}

string get_share(int LID){
	string share = DB.get_share(LID);

	return share;
}

vector<string> get_ano_shares(int LID){
	return DB.get_ano_shares(LID, key_threshold);
}

char* extract_secret_from_share(vector<string> shares){
	string temp_share;

	vector<string>::iterator iter;
	for(iter = shares.begin(); iter != shares.end(); iter++){
		temp_share += *iter;
	}

	char* secret = extract_secret_from_share_strings(temp_share.c_str());

	return secret;
}

void generate_AES(string pass){
	initAES(pass.c_str(), NULL, key, iv);
	ofstream ofs("symmetric_key", ios::out | ios::app);
	if(ofs.fail()){
		cerr << "Error!" << endl;
	}
	cout << "Success in generating AES" << endl;
}

unsigned char* encrypt_data(char* secret, unsigned char* data){
	unsigned char ciphertext[514];
	int ciphertext_len = aes_encrypt(data, strlen((char*)data), key, iv, ciphertext);

	return ciphertext;
}

unsigned char *decrypt_data(char* secret, unsigned char* data){
	unsigned char plainText[514];
	int plainText_len = aes_decrypt(data, strlen((char*)data), key, iv, plainText);
	
	return plainText;
}

string insert_public_key(string pubkey){
	string LID = DB.insert_pk_database(getCID(), pubkey);
	return LID;
}

void insert_video_data(char* CID, char* Hash, char* Signed_Hash){
	DB.insert_video_data(CID, Hash, Signed_Hash);
}

void insert_port(int ID, int port){
	static mutex m;
	while(true){
		if(m.try_lock()){
			m.unlock();
			break;
		}
		else{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	while(true){
		if(m.try_lock()){
			m.unlock();
			break;
		}
		else{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}

void pop_port(int port){
	static mutex m;
	while(true){
		if(m.try_lock()){
			m.unlock();
			break;
		}
		else{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}

void makePacket(uint8_t destID, uint8_t cmd, uint8_t dataType, uint32_t dataSize)
{
	sendDataPacket.startID = ThisID; //로거, �?증기, ?���? ...
	sendDataPacket.destID = destID;
	sendDataPacket.command = cmd;
	sendDataPacket.dataType = dataType;
	sendDataPacket.dataSize = dataSize;
}

static int __send( IO_PORT *p, HANDLE pdata, int len )
{
	int res = 0;
	int i = 0;
	struct timeval tv;
	fd_set fdset, temps;

	if( p == NULL ) {
		return 0;
	}

	FD_ZERO( &fdset );
	FD_SET( p->s, &fdset );
	do {
		temps = fdset;

		tv.tv_sec = 5;
		tv.tv_usec = 0;

		res = select( p->s+1, NULL, &temps, NULL, &tv );
		if( res > 0 ) {
			break;
		}
		TRACEF( "retry send(%d)\n", i );
	} while( ++i < 4 );

	if( res > 0 ) {
		res = send( p->s, pdata, len, MSG_NOSIGNAL );
		//(send(ns,(HEADERPACKET*)&sPacket,sizeof(sPacket),0)  == -1)
		if( res <= 0 ) {
			TRACE( "len(%d), timeout(%d) res(%d)", len, p->timeout, res );
		}
	}
	return res;
}

static int __recv( IO_PORT *p, HANDLE pdata, int len )
{
	int     res;
	struct  timeval tv;

	if( p->timeout > 0 ) {
		fd_set reads;

		FD_ZERO( &reads );
		FD_SET( p->s, &reads );

		tv.tv_sec = p->timeout;
		tv.tv_usec = 0;

		res = select( p->s+1, &reads, NULL, NULL, &tv );
		if( res == -1 ) {
			TRACE_ERR( "Select error.\n" );
			return -1;
		}
		else if( res == 0 ) {
			//TRACEF( ">>>> Select Time Out...\n" );
			return -9;
		}
	}
	return recv( p->s, pdata, len, 0 );
}

int send_binary( IO_PORT *p, long nSize, HANDLE pdata )
{
	int nSendBytes;

	if( p == NULL ) {
		TRACE_ERR("p == NULL\n");
		return FALSE;
	}

	//TRACEF("nSize = %d\n", nSize);
	do {
		nSendBytes = MIN( ASYNC_BUFSIZE, nSize );
		nSendBytes = __send( p, pdata, nSendBytes );
		if( nSendBytes <= 0 ) {
			TRACE_ERR( "return FALSE(%d)\n", nSendBytes );
			return FALSE;
		}

		pdata += nSendBytes;
		nSize -= nSendBytes;

	} while( nSize > 0 );

	return TRUE;
}

int recv_binary( IO_PORT *p, long size, HANDLE pdata )
{
	int remainbytes, recvbytes;

	remainbytes = size;
	while(remainbytes > 0) {
		recvbytes = __recv( p, pdata, remainbytes );
		if( recvbytes <= 0 ) {
			TRACE_ERR( "ERR recv_binary :%d\n", recvbytes );
			return FALSE;
		} // recvbytes <= 0 ??
		pdata += recvbytes;
		remainbytes -= recvbytes;
		TRACEC(D_RED, "remainbytes = %d, recvBytes = %d\n", remainbytes, recvbytes);
	}
	return TRUE;
}

SOCKET create_socket()
{
	SOCKET s;
	int opt;

	if( ( s = socket( AF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET ) {
		TRACE_ERR( "\tsocket error.\n" );
		return INVALID_SOCKET;
	}

	opt = 32*1024;
	setsockopt( s, SOL_SOCKET, SO_RCVBUF, &opt, sizeof( opt ) );
	opt = 32*1024;
	setsockopt( s, SOL_SOCKET, SO_SNDBUF, &opt, sizeof( opt ) );
	opt = 1;
	setsockopt( s, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof( opt ) );

	return s;
}

int bind_socket( SOCKET s, int port )
{
	int retval;
	int opt = 1;
	struct sockaddr_in addr;
	struct linger lingerStruct;

	lingerStruct.l_onoff = 1;
	lingerStruct.l_linger = 0; // blocking wait time

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl( INADDR_ANY );
	addr.sin_port = htons( SERVER_PORT );

	//set resusing socket address
	setsockopt( s, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof( opt ) );
	setsockopt(s, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct));

	if( bind( s, (struct sockaddr *)&addr, sizeof(struct sockaddr) ) == -1 ) {
		retval = errno;
		switch( retval ) {
			case EADDRINUSE     :   TRACE( "this address is already used\n" );                break;
			case EADDRNOTAVAIL  :   TRACE( "this address is not available\n" );               break;
			case EAFNOSUPPORT   :   TRACE( "socket address familly is wrong\n" );             break;
			case EBADF          :   TRACE( "socket parameter is wrong file descripter\n" );   break;
			case EINVAL         :   TRACE( "socket is binded another address\n" );            break;
			case ENOTSOCK       :   TRACE( "socket not socket\n" );                           break;
			case EOPNOTSUPP     :   TRACE( "socket not socket\n" );                           break;
			default             :   TRACE( "bind default \n" );                               break;
		}
		return FALSE;
	}
	return TRUE;
}

static void *ClientServiceThread(void *arg)
{
	cout << "ClientServiceThread start" << endl;
	cout << "-------------------------------------------" << endl << endl;
	IO_PORT *clientThd = (IO_PORT*) arg;
	uint8_t res;
	uint32_t fd_max;
	uint32_t fd_socket;
	uint32_t retry_cnt = 5;
	uint32_t send_retry_cnt = 5;
	struct  timeval tv;
	fd_set  reads;
	uint8_t buf[CMD_HDR_SIZE];
	uint8_t cmd[100]={0,};

	pthread_t mythread;
	mythread = pthread_self();
	pthread_detach( mythread );

	if(arg == NULL){
		TRACE_ERR( "arg == NULL\n" );
		return NULL;
	}

	TRACEF( "################## Start Connection Proecess ##################\n" );


	clientThd->timeout  = 30;
	fd_socket = clientThd->s;
	fd_max = fd_socket+1;

	while( clientThd->timeout > 0 ) {
		memset(buf, 0, sizeof(buf));
		FD_ZERO( &reads );
		FD_SET( fd_socket, &reads );
		tv.tv_sec = 10;
		tv.tv_usec = 0;

		//make HEADERPACKET
		res = select( fd_max, &reads, NULL, NULL, &tv );
		if( res == -1 ) {
			TRACE_ERR( "connect socket(%d) Select error.\n", fd_socket);
			usleep(10000);
			continue;
		}
		else if( res == 0 ) {
			TRACEF( "socket(%d) >>>> Select Time Out...\n", fd_socket);
			goto SERVICE_DONE;
		}
		while(retry_cnt >= 0) {
			res = recv( fd_socket, buf, CMD_HDR_SIZE, 0 );  //강제로 연결이 끊어졌을 경우 res가 255로 나오는 에러
			if(res <= 0) {
				if(retry_cnt <= 0){
					TRACE_ERR("connect socket(%d) Command receive error\n", fd_socket);
					goto SERVICE_DONE;
				}
				retry_cnt--;
			}
			else {
				break;
			}
		}
		retry_cnt = 5;
		if(cmd_parser(*clientThd, (HEADERPACKET *)buf) == -1) {
			cout << "cmd_parser return -1" << endl;
			TRACE_ERR("Data is sent with wrong destination : connected socket (%s)\n", fd_socket);
			continue;
		}
		else{
			send_retry_cnt--;
			if(send_retry_cnt == 0) {
				TRACE_ERR("connect socket(%d) Command send error\n", fd_socket);
				goto SERVICE_DONE;
			}
			// if ((HEADERPACKET *)buf->command == CMD_BACKGROUND) {
			// 	cout << "client still alive" << endl;
			// 	send_retry_cnt = 5;
			// 	usleep(10000);
			// 	continue;
			// }
		}
		
		send_retry_cnt = 5;

		usleep(10000);
	}
	cout << "ClientServiceThread end" << endl;
SERVICE_DONE:
	closesocket(fd_socket);
	return NULL;
}

static void *listenThd(void *arg)
{
	cout << "listen start" << endl;

	NETWORK_CONTEXT *thisThd = (NETWORK_CONTEXT *) arg;
	SOCKET      news;
	socklen_t   len;
	int         fd_max, opt;
	int         ret;
	fd_set      readset;
	struct timeval  tvout;
	struct sockaddr_in addr;
	struct linger lingerStruct;

	pthread_detach(pthread_self());

	listen( thisThd->m_socket, MAX_USER_CNT);

	fd_max = thisThd->m_socket;
	lingerStruct.l_onoff = 1;
	lingerStruct.l_linger = 0; // ???�? ?��?�� 바로 종료, blocking ???기시�?

	TRACEC(D_PURPLE," Server Service Listen Thread Start\n");
	while(thisThd->networkLoop){
		FD_ZERO( &readset );
		FD_SET( thisThd->m_socket, &readset );

		tvout.tv_sec = 0;
		tvout.tv_usec = 50*1000;
		ret = select( fd_max+1, &readset, 0, 0, &tvout );
		if(ret < 0){
			TRACE_ERR("select error : %s\n", strerror( errno ));
			continue;
		}
		else if(ret == 0){
			continue;
		}

		if(!FD_ISSET( thisThd->m_socket, &readset )){
			TRACE_ERR("FD_ISSET error : thisThd->m_socket=%d\n", thisThd->m_socket);
			continue;
		}

		len = sizeof( addr);
		news = accept( thisThd->m_socket, (struct sockaddr *)&addr, &len ); // news = unsigned int 
		if( news == -1 ) {
			TRACE_ERR( "accept() failed :%s\n", strerror( errno ) );
			continue;
		}

		TRACEC(D_GREEN, "accepted.client ip=(%s), new socket=(%d)\n", inet_ntoa(addr.sin_addr), news);

		opt = 1;
		setsockopt( news, SOL_SOCKET, SO_KEEPALIVE, (char*)&opt, sizeof(opt) );
		setsockopt( news, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct));

		thisThd->port.type = PORT_TYPE_TCP;
		thisThd->port.s = news;
		thisThd->port.addr = addr;
		thisThd->port.timeout = 30;
		
		ret = pthread_create( &thisThd->clientThread, NULL, ClientServiceThread, (void*)&thisThd->port);
		if( ret != 0 ){
			TRACE_ERR( "ERROR create ptt client service thread\n" );
			closesocket( news );
			news = INVALID_SOCKET;
			continue;
		}

		usleep(10);
	}

	return NULL;
}

int initServer()
{
	cout << "initServer start" << endl;
	Read_server_cfg();
	srand((unsigned int)time(NULL));
	//key_generation();
	int res;

	g_pNetwork = (NETWORK_CONTEXT *) malloc(sizeof(NETWORK_CONTEXT));
	
	if(!g_pNetwork){
		TRACE_ERR("network context initialize fail\n");
		return -3;
	}

	g_pNetwork->networkLoop = 1;
	g_pNetwork->m_socket = create_socket();

	pthread_mutex_init(&g_pNetwork->g_mc_mtx, NULL);

	if(bind_socket(g_pNetwork->m_socket, SERVER_PORT ) == FALSE ) {
		TRACEF("ERROR cannot bind listen port:%d \n", SERVER_PROTOCOL_PORT );
		closesocket( g_pNetwork->m_socket);
		g_pNetwork->m_socket = INVALID_SOCKET;
		return -2;
	}
	
	res = pthread_create(&g_pNetwork->listenThread, NULL, listenThd, (void*)g_pNetwork);
	if(res<0){
		TRACE_ERR("Server Listen thread init fail\n");
		g_pNetwork->networkLoop= 0;
		free(g_pNetwork);
		return -1;
	}
	cout << "initServer end" << endl;
	return TRUE;
}

void termServer()
{
	if(g_pNetwork == NULL){
		TRACE_ERR("g_pNetwork is NULL\n");
		return;
	}

	int result;
	while(true){
		if(!pthread_join(g_pNetwork->clientThread, (void**)&result)){
			if(!pthread_join(g_pNetwork->listenThread, (void**)&result)){
					g_pNetwork->recvLoop = 0;
					g_pNetwork->networkLoop = 0;
					break;
			}
		}
		sleep(3);
	}
	
	if(g_pNetwork){
		free(g_pNetwork);
	}
}

void closesocket(SOCKET sock_fd){
	close(sock_fd);
}
