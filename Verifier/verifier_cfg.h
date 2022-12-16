#ifndef __VERIFIER_CFG__
#define __VERIFIER_CFG__
//----------------------------------
#define Hash_size 64
#define Signed_Hash_size 350
//----------------------------------
// Default values for Camera Settings
#define VGA_SIZE 460800
#define CIF_SIZE 152064
#define HD_SIZE  1382400

#define YUV420_VGA_WIDTH 640
#define YUV420_VGA_HEIGHT 480 * 3/2
#define YUV420_CIF_WIDTH 352
#define YUV420_CIF_HEIGHT 288 * 3/2
#define YUV420_HD_WIDTH  1280
#define YUV420_HD_HEIGHT 720 * 3/2

#define VGA_WIDTH 640
#define VGA_HEIGHT 480
#define CIF_WIDTH 352
#define CIF_HEIGHT 288
#define HD_WIDTH 1280
#define HD_HEIGHT 720
//----------------------------------
// Deafault values PATH for Verifier
#define FORDER_NAME   "data_table/"
#define pubKey_path     "/home/pi/hanium_2022/Verifier/pubkey.txt"
#define video_data_path "/home/pi/images/"
//----------------------------------
#define ThisID Verifier

#endif
