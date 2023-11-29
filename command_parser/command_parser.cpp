#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

#include "command_parser.h"
#include "../Server/tracex.h"

using namespace std;

cmdp_desp parser_desp[] =
{
	{PUBKEY_REQ					, public_key_request	},
	{PUBKEY_RES					, public_key_response	},
	{VIDEO_DATA_SND				, video_data_send		},
	{VIDEO_DATA_RES				, video_data_response	},
	{VER_REQ					, verify_request		},
	{VER_RES					, verify_response		},
	{VER_2_PROV					, verify_to_prover		},
	{PROV_RES					, prover_response		},
	
	{HI_I_M						, hi_i_am				},
	{NICE_2_MEET_U				, nice_to_meet_you		},

	{GEN_AES					, generate_aes			},
	{SHARE_REQ					, share_request			},
	{SHARE_RES					, share_response		},
	{ANO_SHARE_REQ				, another_share_request	},
	{ANO_SHARE_RES				, another_share_response},
	{GEN_KEY_LG					, generate_key_at_logger},
	{ENC_DATA_LG				, encrypt_data_at_logger},
	{ENC_DATA_REQ				, encrypt_data_request	},
	{ENC_DATA_RES				, encrypt_data_response	},
	{DEC_DATA_SER				, decrypt_data_at_server},

	// Testing command
	{TEST_CMD					, test					},
	{TEST_GEN_SHARES			, test_gen_shares		},
	{GEN_SHARE_RES				, gen_share_res			},
	{TEST_SHARE_REQ				, test_share_req		},
	{TEST_SHARE_RES				, test_share_res		},
};

//-----------------------------------------------------------------------------
//	cmd_parser
//-----------------------------------------------------------------------------
#ifndef _CMD_PARSER_
#define _CMD_PARSER_
int cmd_parser(IO_PORT port, HEADERPACKET *pmsg)
{
	int ack, i=0, len;
	len = sizeof(parser_desp) / sizeof(parser_desp[0]);

	cout << "---------------------------------" << endl;
	cout << "port : " << port.s << endl;
	cout << "Start ID : "<< hex << (int)pmsg->startID << endl;
	cout << "Destination ID : " << (int)pmsg->destID << endl;
	cout << "Command : " << (int)pmsg->command << endl;
	cout << "Data Type : " << (int)pmsg->dataType << endl;
	cout << "Data Size : "<< dec << (int)pmsg->dataSize << endl;
	cout << "---------------------------------" << endl;

	// if(pmsg->destID != ThisID){
	// 	cout << "Wrong destination! >> " << port.s << " ThisID : " << ThisID;
	// 	return -1; //"HEADERPACKET' destID != ThisID";
	// }

	for (i = 0, ack = -3; i < len; i++){
		if(parser_desp[i].code == pmsg->command){
			ack = parser_desp[i].callback(pmsg, &port);
			break;
		}
	}
	cout << "ack : " << ack << endl;
	
	switch(ack){
		case -3 : cout << "Somethings Wrong... callback function doesn't work";
				  return -1;
		case -1 : cout << " doesn't work;" << endl;
				  return 0;
		case 0 : return 0;
	}

	return 0;
}
#endif
