#include <stdio.h>
#include <stdlib.h>

#ifndef __CMDPARSERHEADER__
#define __CMDPARSERHEADER__

#include "../Server/server.h"
#include "command_define_list.h"
#include "command_function_list.cpp"

typedef struct _cmdpdesp
{
	u_int8_t code;
	int(* callback)(HEADERPACKET* msg, IO_PORT *port);
}cmdp_desp;

int cmd_parser(IO_PORT port, HEADERPACKET *pmsg);

/* command fucntions*/
int public_key_send(HEADERPACKET* msg, IO_PORT *port);			//PUBKET_SND
int public_key_response(HEADERPACKET* msg, IO_PORT *port);		//PUBKEY_RES
int video_data_send(HEADERPACKET* msg, IO_PORT *port);			//VIDEO_DATA_SND
int video_data_response(HEADERPACKET* msg, IO_PORT *port);		//VIDEO_DATA_RES
int verify_request(HEADERPACKET* msg, IO_PORT *port);			//VER_REQ
int verify_response(HEADERPACKET* msg, IO_PORT *port);
int verify_to_prover(HEADERPACKET* msg, IO_PORT *port);
int prover_response(HEADERPACKET* msg, IO_PORT *port);

int term_socket(HEADERPACKET* msg, IO_PORT *port);				//STILL_ALIVE
int hi_i_am(HEADERPACKET* msg, IO_PORT *port);					//HI_I_M
int nice_to_meet_you(HEADERPACKET* msg, IO_PORT *port);			//NICE_2_MEET_U

int generate_aes(HEADERPACKET* msg, IO_PORT *port);
int generate_shares(HEADERPACKET* msg, IO_PORT *port);
int share_request(HEADERPACKET* msg, IO_PORT *port);
int share_response(HEADERPACKET* msg, IO_PORT *port);
int another_share_request(HEADERPACKET* msg, IO_PORT *port);
int another_share_response(HEADERPACKET* msg, IO_PORT *port);
int generate_key_at_logger(HEADERPACKET* msg, IO_PORT *port);
int encrypt_data_at_logger(HEADERPACKET* msg, IO_PORT *port);
int encrypt_data_request(HEADERPACKET* msg, IO_PORT *port);
int encrypt_data_response(HEADERPACKET* msg, IO_PORT *port);
int decrypt_data_at_server(HEADERPACKET* msg, IO_PORT *port);

int test(HEADERPACKET* msg, IO_PORT *port);						//TEST_CMD
#endif
