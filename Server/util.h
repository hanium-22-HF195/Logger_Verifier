
// Symmetric key
extern std::string password;
extern int num_of_share;
extern int key_threshold;
extern char *salt;

extern int SERVER_PORT;

extern char* storage_dir;

// Deafault values for datasize
extern int Hash_size;
extern int Signed_Hash_size;
extern int CID_size;

// Communication protocol
extern int CMD_HDR_SIZE;
extern int ASYNC_BUFSIZE;
extern int MAX_USER_CNT;

extern char* pubkeyfile_path;
extern char* prikeyfile_path;


#include <jsoncpp/json/json.h>
extern  std::string sStorage_dir;


void Read_server_cfg();

