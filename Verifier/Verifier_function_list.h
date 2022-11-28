using namespace std;

int get_data_from_DB(string &CID);
void read_video_data(string &CID);
void convert_frames(cv::Mat &YUV420);
void edge_detection(cv::Mat &Y); 
void compare_hash(string &HASH_VERIFIER, string &HASH_DB);
void update_result(string &CID, int VERIFIED);
void init_Verifier();