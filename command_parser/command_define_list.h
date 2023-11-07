
#define Logger          0x10
#define Verifier        0x11
#define Server          0x12
#define WebUI           0x13
#define Client          0x19

#define Char            0xa0
#define Uchar           0xa1
#define Int             0xb0
#define Uint            0xb1

#define PUBKEY_REQ      0x01
#define PUBKEY_RES      0xf1
#define VIDEO_DATA_SND  0x02
#define VIDEO_DATA_RES  0xf2
#define VER_REQ         0x03
#define VER_RES         0xf3
#define VER_2_PROV      0x04
#define PROV_RES        0xf4

/*-Communication function with API Server-*/
#define GEN_AES         0x80
#define SHARE_REQ       0x82
#define SHARE_RES       0x83
#define ANO_SHARE_REQ   0x84
#define ANO_SHARE_RES   0x85
#define GEN_KEY_LG      0x86
#define ENC_DATA_LG     0x87
#define ENC_DATA_REQ    0x88
#define ENC_DATA_RES    0x89
#define DEC_DATA_SER    0x90
/*---------This function is for Testing--------------*/

#define HI_I_M          0x09
#define NICE_2_MEET_U   0xf9

#define TERM_SOCKET     0xee
#define TEST_CMD        0xff


#define TEST_GEN_SHARES 0xe0
#define GEN_SHARE_RES   0xe1
#define TEST_SHARE_REQ  0xe2
#define TEST_SHARE_RES  0xe3