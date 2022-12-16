using namespace std;

int read_pubKey();
int get_data_from_DB();
void read_video_data(string &CID);
void convert_frames(cv::Mat &YUV420);
void edge_detection(cv::Mat &Y); 
void compare_hash(string &HASH_VERIFIER);
void update_result( string &CID, int VERIFIED);
void init_Verifier();