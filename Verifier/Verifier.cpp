#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include <iostream>
#include <queue>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <fstream>
#include <error.h>

#include "command_define_list.h"
#include "verifier_cfg.h"
#include "Verifier_function_list.h"
#include "verify.cpp"
#include "../Merkle_Tree/merkle_tree.cpp"
#include "../Merkle_Tree/node.cpp"
#include "../DB/bout_database.cpp"

using namespace std;

cv::Mat yuv420;
cv::Mat y;
cv::Mat feature_vector;
string hash_DB;
string hash_VF;
string cid;
int verified = 0;
bout_database bDB;


int get_data_from_DB(string &CID)
{
    init_DB(mysqlID);

    // Table name change needed
    string table_name = CID.substr(0, 4) + "_" + CID.substr(5, 2) + CID.substr(8, 2);
    string sorder = "select Hash, Signed_Hash from " + table_name + " where CID = " + CID + ";";

    char *order = new char[sorder.length() + 1];
    strcpy(order, sorder.c_str());
    res = mysql_perform_query(conn, order);

    while ((row = mysql_fetch_row(res)) != NULL)
    {
        hash_DB = row[0];
        string s_signed_hash_DB = row[1];

        char *c_signed_hash_DB = new char[Signed_Hash_size];
        strcpy(c_signed_hash_DB, s_signed_hash_DB.c_str());

        bool authentic = verifySignature(publicKey, hash_DB, c_signed_hash_DB);
        if (authentic)
        {
            cout << CID << "'s signed hash is verified." << endl;

        }
        else 
        {
            cout << "Not Authentic." << endl;
        }
    }
    return 0;
}

void read_video_data(string &CID)
{
    cout << "Read frames from storage." << endl;

    string file_dir = storage_dir + CID;
    ifstream frame_file(file_dir, ifstream::binary);
    frame_file.seekg(0, frame_file.end);
    int size = (int)frame_file.tellg();
    frame_file.seekg(0, frame_file.beg);

    unsigned char *frame_data = (unsigned char *)malloc(size);

    frame_file.read((char *)frame_data, size);
    frame_file.close();

    if (size == VGA_SIZE) {
        cv::Mat frame = cv::Mat(cv::Size(YUV420_VGA_WIDTH, YUV420_VGA_HEIGHT), CV_8UC1, frame_data);
        yuv420 = frame.clone();
    }
    else if (size == CIF_SIZE) {
        cv::Mat frame(cv::Size(YUV420_CIF_WIDTH, YUV420_CIF_HEIGHT), CV_8UC1, frame_data);
        yuv420 = frame.clone();
    }
    else if (size == HD_SIZE) {
        cv::Mat frame(cv::Size(YUV420_HD_WIDTH, YUV420_HD_HEIGHT), CV_8UC1, frame_data);
        yuv420 = frame.clone();
    }
    cout << "Frame read " << endl;
}

void convert_frames(cv::Mat &YUV420)
{
    cout << "----Start to convert Frames into Y----" << endl;

    // CONVERT BGR To YUV420 and YUV420 to Y
    cv::cvtColor(yuv420, y, cv::COLOR_YUV2GRAY_I420);
    YUV420.release();

}

void edge_detection(cv::Mat &Y)
{
    cout << "----Start to convert Frames into Y----" << endl;

    // Canny(img, threshold1, threshold2)
    // threshold1 = Determining whether an edge is in the adjacency with another edge
    // threshold2 = Determine if it is an edge or not
    cv::Canny(y, feature_vector, 20, 40);
    cout << "----edge Detected----" << endl;
    Y.release();
}


void make_hash(cv::Mat &FV)
{
    string mat_data= "";
    for (int i = 0; i < FV.rows; i++) {
        for (int j = 0; j < FV.cols; j++) {
            mat_data += to_string(FV.at<uchar>(i, j));
        }
    }

    string sha_result = hash_sha256(mat_data);
    hash_VF = sha_result;
    cout << "hash made by VF" << endl;
    FV.release();
}

void compare_hash(string &HASH_VERIFIER, string &HASH_DB) {
    cout << HASH_VERIFIER << endl;
    cout << HASH_DB << endl;
    if(HASH_DB.compare(HASH_VERIFIER) == 0 ){
        cout << "VERIFIED." << endl;
        verified = 1;
        cout << "---------------------------" << endl;
    }
    else {
        cout << "NOT VERIFIED" << endl;
        verified = -1;
        cout << "---------------------------" << endl;
    }
}

void update_result(string &CID, int VERIFIED){

    // Table name change needed
    string table_name = "";
    string sorder = "update " + table_name + " SET verified = " + to_string(VERIFIED) + " where CID = " + CID + ";";

    char *order = new char[sorder.length() + 1];
    
    //UPDATE NEDDED

}

void init_Verifier() {
    string hash_DB = "";
    string hash_VF = "";
    string cid = "";
    int verified = 0;
}

int main()
{

    // Server to Verifier ( CID, PUBKEY)
    
    //
    get_data_from_DB(cid);
    read_video_data(cid);
    convert_frames(yuv420);
    edge_detection(y); 
    compare_hash(hash_VF, hash_DB);
    update_result(cid, verified);
    init_Verifier();

    return 0;
}
