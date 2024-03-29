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
#include <time.h>
#include <sys/timeb.h>
#include <fstream>
#include <map>
#include <vector>

#include "logger_cfg.h"
#include "Logger_function_list.h"
//#include "../openssl/sign.cpp"
#include "../Merkle_Tree/merkle_tree.h"
#include "../Client/client.cpp"
#include "../command_parser/command_define_list.h"
#include "../msg_queue/msg_queue.cpp"

using namespace std;
using namespace cv;
#define ThisID Logger

cv::VideoCapture cap;
cv::Mat frame(cv::Size(width, height), CV_8UC3);
string getCID();

pthread_mutex_t frameLocker;
pthread_t UpdThread;

int LID;

queue<cv::Mat> bgr_queue;            // for original frame(BGR)Mat queue
queue<cv::Mat> yuv420_queue;         // for original frame(yuv)Mat queue
queue<cv::Mat> y_queue;              // for y_frame Mat queue
queue<cv::Mat> G_queue;              // for gray scale Mat queue
queue<cv::Mat> feature_vector_queue; // for edge detection Canny
queue<string> hash_queue;            // for hash made by feature vector
queue<string> hash_signed_queue;
queue<string> cid_queue; // for CID for images

int key_generation()
{
    cout << "----Key Geneartion----" << endl;
    RSA *privateRSA = genPrivateRSA();
    publicKey = genPublicRSA(privateRSA);

    cout << "PRIKEY and PUBKEY are made" << endl;
    cout << "public Key = " << endl
         << publicKey << endl;
    cout << "private key = " << endl
         << privateKey;

    cout << "SAVE PUBLIC KEY FOR VERIFIER" << endl;

    fstream pubkey_file(pubkeyfile_path , ios::trunc | ios::out);
        if(pubkey_file.is_open()){
            pubkey_file << publicKey;
        }   

        pubkey_file.close();
    return 0;
}

int send_pubKey_to_server()
{

    cout << "----SEND PUBKEY to SERVER----" << endl;
    int pubKey_bufsize = publicKey.size();
    std::cout << "pubKey_bufsize: " << pubKey_bufsize << std::endl;

    char *pubKey_buffer = new char[pubKey_bufsize];
    strcpy(pubKey_buffer, publicKey.c_str());

    makePacket(Server, PUBKEY_REQ, 0xa0, strlen(pubKey_buffer));
    void *p_packet = &sendDataPacket;

    if (!send_binary(&g_pNetwork->port, CMD_HDR_SIZE, p_packet))
    {
        cout << "PubKey send Error!!" << endl;
    }

    if (!send_binary(&g_pNetwork->port, strlen(pubKey_buffer), (void *)pubKey_buffer))
    {
        cout << "PubKey send Error!!" << endl;
    }
    cout << "----SENDING PUBKEY to SERVER END----" << endl;
    delete [] pubKey_buffer;
}

void open_camera() {
    
     // open the default camera using default API
    int deviceID = 0;         // 0 = open default camera
    int apiID = cv::CAP_V4L2; // use V4L2
    // open selected camera using selected API
    cap.open(deviceID, apiID);   

    if(!cap.isOpened()){
        cout << "camera doesn't open" << endl;
        exit(0);
    }
}

int init()
{
    cout << "----Initalizing---------" << endl
         << endl;
   
    camera_cfg_recv(width, height, fps);

    cap.set(cv::CAP_PROP_FRAME_WIDTH, width );
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cap.set(cv::CAP_PROP_FPS, fps);

    cout << "    Frame Width: " << cvRound(cap.get(CAP_PROP_FRAME_WIDTH)) << endl;
    cout << "    Frame Height: " << cvRound(cap.get(CAP_PROP_FRAME_HEIGHT)) << endl;
    cout << "    FPS : " << cvRound(cap.get(CAP_PROP_FPS)) << endl;

    cv::Mat img(cv::Size(width, height), CV_8UC3, Scalar(0));
    frame = img.clone();
    img.release();

    //--- If Cap is opened
    if (!cap.isOpened())
    {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    else
    {
        cout << "----Initalized----------" << endl;
        return 0;
    }
}

void init_all_settings()
{

    init_queue();

    cout << endl
         << "----Initializing all settings." << endl
         << endl;
    cout << "    bgr queue size: " << bgr_queue.size() << endl;
    cout << "    yuv420 queue size: " << yuv420_queue.size() << endl;
    cout << "    y_frame queue size: " << y_queue.size() << endl;
    cout << "    feature vector queue size: " << feature_vector_queue.size() << endl;
    cout << "    hash queue size: " << hash_queue.size() << endl;
    cout << "    CID queue size: " << cid_queue.size() << endl;
    cout << endl
         << "--------------------------------" << endl
         << endl;
}

void init_queue()
{

    while (!yuv420_queue.empty())
        yuv420_queue.pop();
    while (!bgr_queue.empty())
        bgr_queue.pop();
    while (!y_queue.empty())
        y_queue.pop();
    while (!feature_vector_queue.empty())
        feature_vector_queue.pop();
    while (!hash_queue.empty())
        hash_queue.pop();
    while (!hash_signed_queue.empty())
        hash_signed_queue.pop();
    while (!cid_queue.empty())
        cid_queue.pop();
}

void lamping_time() {
    Mat temp;
    if (!cap.isOpened())
    {
        cerr << "ERROR! Unable to open camera -- lamping time\n";
    }
    for(int i = 0; i< 20; i++){
        cap >> temp;
    }
    temp.release();
}

void *UpdateFrame(void *arg)
{
    while (true)
    {

        cv::Mat tempFrame(cv::Size(width, height), CV_8UC3);
        cap >> tempFrame;

        pthread_mutex_lock(&frameLocker);
        frame = tempFrame.clone();
        pthread_mutex_unlock(&frameLocker);
    }
    pthread_exit((void *)0);
}

void capture()
{
    cout << endl
         << "----Starting Capturing" << endl
         << endl;

    pthread_mutex_init(&frameLocker, NULL);
    pthread_create(&UpdThread, NULL, UpdateFrame, NULL);

    while (true)
    {
        cv::Mat currentFrame(cv::Size(height, width), CV_8UC3);

        pthread_mutex_lock(&frameLocker);
        currentFrame = frame;
        pthread_mutex_unlock(&frameLocker);

        int sum1 = (int)sum(currentFrame)[0];
        int sum2 = (int)sum(currentFrame)[1];
        int sum3 = (int)sum(currentFrame)[2];
        int elementmean = (sum1 + sum2 + sum3) / 3;

        if (currentFrame.empty())
        {
            cout << "Frame is empty" << endl;
        }
        else if (elementmean != 0)
        {
            bgr_queue.push(currentFrame);
            // Make CID for FRAMES
            string s_cid = getCID();
            cid_queue.push(s_cid);
        }
        else
        {
            cout << "lamping time" << endl;
        }

        if (bgr_queue.size() == frame_count)
        {

            int ret = pthread_cancel(UpdThread);
            int status;

            if (ret == 0)
            {
                // auto END
                ret = pthread_join(UpdThread, (void **)&status);
                if (ret == 0)
                {
                    // AUTO END = STATUS= -1
                    cout << "Capture End Successfully." << endl;
                    pthread_mutex_destroy(&frameLocker);
                    currentFrame.release();
                }
                else
                {
                    cout << "Thread End Error!" << ret << endl;
                    pthread_mutex_destroy(&frameLocker);
                    currentFrame.release();
                }
                break;
            }
        }

        // if ESC is pressed, then force thread to end
        // if (cv::waitKey(20) == 27)
        // {

        //     int ret = pthread_cancel(UpdThread);
        //     int status;

        //     if (ret == 0)
        //     {
        //         // auto END
        //         ret = pthread_join(UpdThread, (void **)&status);
        //         if (ret == 0)
        //         {
        //             // AUTO END = STATUS= -1
        //             cout << "Capture End Successfully." << endl;
        //             pthread_mutex_destroy(&frameLocker);
        //             currentFrame.release();
        //         }
        //         else
        //         {
        //             cout << "Thread End Error!" << ret << endl;
        //             pthread_mutex_destroy(&frameLocker);
        //             currentFrame.release();
        //         }
        //         break;
        //     }
        // }
    }
}


void convert_frames2YUV(queue<cv::Mat> &BGR_QUEUE)
{

    cout << endl
         << "----Start to convert Frames into YUV420 and Y----" << endl
         << endl;
    queue<cv::Mat> BGR_queue(BGR_QUEUE);

    while (true)
    {

        if (BGR_queue.size() == 0)
        {
            break;
        }

        cv::Mat original = BGR_queue.front();
        cv::Mat yuv_frame(cv::Size((height * 3 / 2), width), CV_8UC1);
        cv::Mat y_frame(cv::Size(height, width), CV_8UC1);
        BGR_queue.pop();

        // CONVERT BGR To YUV420 and YUV420 to Y
        cv::cvtColor(original, yuv_frame, cv::COLOR_BGR2YUV_I420);
        cv::cvtColor(yuv_frame, y_frame, cv::COLOR_YUV2GRAY_I420);

        // save frames into queue
        yuv420_queue.push(yuv_frame);
        y_queue.push(y_frame);

        // release Mat
        original.release();
        yuv_frame.release();
        y_frame.release();
    }

    cout << "    YUV420 amd Y frame are saved" << endl;
    cout << "    YUV420 frame: " << yuv420_queue.size() << "    Y frame: " << y_queue.size() << endl
         << endl;
    cout << "----FRAMES CONVERTED---------" << endl
         << endl;
}

void convert_frames2gray(queue<cv::Mat> &BGR_QUEUE)
{

    cout << endl
         << "----Start to convert Frames into Grayscale----" << endl
         << endl;
    queue<cv::Mat> BGR_queue(BGR_QUEUE);

    while (true)
    {

        if (BGR_queue.size() == 0)
        {
            break;
        }

        cv::Mat original = BGR_queue.front();
        string img_name =orifile_path + getCID() + ".jpg";
        cv::imwrite(img_name, original);
        cv::Mat temp;
        BGR_queue.pop();

        // CONVERT BGR To YUV420 and YUV420 to Y
        cv::cvtColor(original, temp, cv::COLOR_BGR2GRAY);

        // save frames into queue
        G_queue.push(temp);

        // release Mat
        original.release();
        temp.release();
    }

    cout << "    Gray scale frame are saved" << endl;
    cout << "    Grayscale :  " << G_queue.size() << endl;
    cout << "----FRAMES CONVERTED---------" << endl
         << endl;
}

void edge_detection_YUV(queue<cv::Mat> &Y_QUEUE)
{
    queue<cv::Mat> Y_queue(Y_QUEUE);

    cout << "----Building feature vectors." << endl;
    int cnt = 0;

    while (true)
    {
        if (Y_queue.size() == 0)
        {
            break;
        }
        cv::Mat temp;

        // Canny(img, threshold1, threshold2)
        // threshold1 = Determining whether an edge is in the adjacency with another edge
        // threshold2 = Determine if it is an edge or not
        cv::Canny(Y_queue.front(), temp, 20, 40);

        feature_vector_queue.push(temp);
        Y_queue.pop();
        cnt++;
        temp.release();
    }
    cout << endl
         << "    Edge Detection made: " << feature_vector_queue.size() << endl;
}

void edge_detection_BGR(queue<cv::Mat> &G_QUEUE)
{
    queue<cv::Mat> G_queue(G_QUEUE);

    cout << "----Building feature vectors." << endl;
    int cnt = 0;

    while (true)
    {
        if (bgr_queue.size() == 0)
        {
            break;
        }
        cv::Mat temp;

        // Canny(img, threshold1, threshold2)
        // threshold1 = Determining whether an edge is in the adjacency with another edge
        // threshold2 = Determine if it is an edge or not
        cv::Canny(bgr_queue.front(), temp, 20, 40);

        feature_vector_queue.push(temp);
        bgr_queue.pop();
        cnt++;
        temp.release();
    }
    cout << endl
         << "    Edge Detection made: " << feature_vector_queue.size() << endl;
}

void make_hash(queue<cv::Mat> &FV_QUEUE)
{

    queue<cv::Mat> Feature_Vector_queue(FV_QUEUE);
    cout << endl
         << "----Make HASH from feature vectors." << endl
         << endl;

    int mat_width = Feature_Vector_queue.front().cols;
    int mat_height = Feature_Vector_queue.front().rows;
    int mat_channels = Feature_Vector_queue.front().channels();
    int umat_data_bufsize = mat_width * mat_height * mat_channels;

    while (true)
    {
        if (Feature_Vector_queue.size() == 0)
        {
            break;
        }
        cv::Mat temp = Feature_Vector_queue.front();
        Feature_Vector_queue.pop();

        string mat_data = "";
        string sha_result = "";

        for (int i = 0; i < temp.rows; i++)
        {
            for (int j = 0; j < temp.cols; j++)
            {
                mat_data += to_string(temp.at<uchar>(i, j));
            }
        }

        sha_result = hash_sha256(mat_data);
        hash_queue.push(sha_result);
        temp.release();
    }
    cout << "    hash made : " << hash_queue.size() << endl;
}

void sign_hash(queue<string> &HASH_QUEUE)
{
    queue<string> sign(HASH_QUEUE);

    cout << "----Signing Hash by private Key" << endl
         << endl;

    char *ch = new char[SIGNED_HASH_BUFSIZE];
 
    while (true)
    {
        if (sign.size() == 0)
        {
            break;
        }
        string signed_hash = signMessage(privateKey, sign.front());

        memset(ch, 0, sizeof(char)*SIGNED_HASH_BUFSIZE);
        strcpy(ch, signed_hash.c_str());

        hash_signed_queue.push(signed_hash);
        sign.pop();
        
    }
    delete [] ch;
    cout << "    Signed Hash made: " << hash_signed_queue.size() << endl;
}

void send_image_hash_to_UI(queue<cv::Mat> &ORI, queue<cv::Mat> &Y)
{
    cout << "----SEND BGR, Y frame and hash to WEB----" << endl;
    cv::Mat ori(Size(width, height), CV_8UC3);
    cv::Mat y(Size(width, height), CV_8UC3);

    ORI.front().copyTo(ori);
    Y.front().copyTo(y);
    
    if( Image_Hash_request() == 1) {
        cv::imwrite(orifile_path, ori);
        cv::imwrite(yfile_path, y);
        string hash = hash_queue.front();

        fstream hash_file(hashfile_path, ios::trunc | ios::out);
        if(hash_file.is_open()){
            hash_file << hash << endl;
        }   

        hash_file.close();
        Image_Hash_response();
    }
    else {
        cout << "    No request from Web UI." << endl;
    }
    
    ori.release();
    y.release();
}

void send_data_to_server(queue<string> &CID_QUEUE, queue<string> &HASH_QUEUE, queue<string> &SIGNED_HASH_QUEUE, queue<cv::Mat> &YUV420_QUEUE)
{
    cout << endl
         << "----SEND DATA to SERVER" << endl;

    queue<string> cid_send(CID_QUEUE);
    queue<cv::Mat> yuv_send(YUV420_QUEUE);
    queue<string> hash_send(HASH_QUEUE);
    queue<string> signed_hash_send(SIGNED_HASH_QUEUE);

    int total_data_size = 0;
    int cid_bufsize = cid_send.front().capacity();
    int hash_bufsize = hash_send.front().capacity();
    int signed_hash_bufsize = signed_hash_send.front().capacity();

    int video_rows = yuv_send.front().rows;
    int video_cols = yuv_send.front().cols;
    int video_channels = yuv_send.front().channels();
    int video_bufsize = video_rows * video_cols * video_channels;

    total_data_size += cid_bufsize;
    total_data_size += hash_bufsize;
    total_data_size += signed_hash_bufsize;
    total_data_size += video_bufsize;

    cout << "total data size : " << total_data_size << endl;
    cout << "size of CID data: " << cid_bufsize << endl;
    cout << "size of hash data: " << hash_bufsize << endl;
    cout << "size of signed_hash data: " << signed_hash_bufsize << endl;
    cout << "video size: " << yuv_send.front().size() << endl;
    cout << "size of video data: " << video_bufsize << endl;
    cout << endl
         << "---------------------- " << endl;

    int step = 0;

    char *cid_buffer = new char[cid_bufsize];
    char *hash_buffer = new char[hash_bufsize];
    char *signed_hash_buffer = new char[signed_hash_bufsize];
    unsigned char *video_buffer = new unsigned char[video_bufsize];

    while (true)
    {
        if (cid_send.size() == 0 && hash_send.size() == 0 && signed_hash_send.size() == 0 && yuv_send.size() == 0)
        {
            break;
        }
        cout << "step : " << ++step << endl;

        memset(cid_buffer, 0, sizeof(char)*cid_bufsize);
        memset(hash_buffer, 0, sizeof(char)*hash_bufsize);
        memset(signed_hash_buffer, 0, sizeof(char)*signed_hash_bufsize);
        memset(video_buffer, 0, sizeof(unsigned char)*video_bufsize);

        strcpy(cid_buffer, cid_send.front().c_str());
        strcpy(hash_buffer, hash_send.front().c_str());
        strcpy(signed_hash_buffer, signed_hash_send.front().c_str());
        memcpy(video_buffer, yuv_send.front().data, video_bufsize); // queue -> mapping //

        makePacket(Server, VIDEO_DATA_SND, 0xa1, total_data_size);

        // cout << hex << (int)sendDataPacket.startID << endl;
        // cout << (int)sendDataPacket.destID << endl;
        // cout << (int)sendDataPacket.command << endl;
        // cout << (int)sendDataPacket.dataType << endl;
        // cout << dec << (int)sendDataPacket.dataSize << endl;
        // cout << endl << "----------------------------------------------------------" << endl << endl;
        // cout << "video rows: " << video_rows << endl << "video cols: " << video_cols << endl;
        // cout << "size: " << (strlen((char*)video_buffer)) * sizeof(unsigned char) << endl;
        // cout << "hash: " << hash_buffer << "size: " << strlen(hash_buffer) * sizeof(char) << endl;
        // cout << "CID: " << cid_buffer << endl << "size: " << strlen(cid_buffer) * sizeof(char) << endl;
        // cout << endl << "----------------------------------------------------------" << endl << endl;

        void *p_packet = &sendDataPacket;

        if (!send_binary(&g_pNetwork->port, sizeof(HEADERPACKET), (void **)p_packet))
        {
            cout << "Packet send Error!!" << endl;
            break;
        }

        if (!send_binary(&g_pNetwork->port, cid_bufsize, (void *)cid_buffer))
        {
            cout << "CID send Error!!" << endl;
        }

        if (!send_binary(&g_pNetwork->port, hash_bufsize, (void *)hash_buffer))
        {
            cout << "hash send Error!!" << endl;
        }

        if (!send_binary(&g_pNetwork->port, signed_hash_bufsize, (void *)signed_hash_buffer))
        {
            cout << "signed_hash send Error!!" << endl;
        }

        if (!send_binary(&g_pNetwork->port, video_bufsize, (void *)video_buffer))
        {
            cout << "Image send Error!!" << endl;
        }

        //ClientServiceThread((void *)&g_pNetwork->port);
        yuv_send.pop();
        hash_send.pop();
        signed_hash_send.pop();
        cid_send.pop();

    }
    delete [] cid_buffer;
    delete [] hash_buffer;
    delete [] signed_hash_buffer;
    delete [] video_buffer;
    cout << "----SEND END----------------" << endl;
}

void gen_shares(int num_of_share, int threshold){
    string sShare = to_string(num_of_share);
    string sThreshold = to_string(threshold);

    string sNum = sShare + sThreshold;
    char* Num = const_cast<char*>(sNum.c_str());

    makePacket(Server, TEST_GEN_SHARES, 0xa0, strlen(Num));
    void *p_packet = &sendDataPacket;

    if (!send_binary(&g_pNetwork->port, CMD_HDR_SIZE, p_packet))
    {
        cout << "PubKey send Error!!" << endl;
    }

    if (!send_binary(&g_pNetwork->port, strlen(Num), (void *)Num))
    {
        cout << "PubKey send Error!!" << endl;
    }
}

string test_share_req(){
    makePacket(Server, TEST_SHARE_REQ, 0xa0, 0);
    void *p_packet = &sendDataPacket;
    if (!send_binary(&g_pNetwork->port, CMD_HDR_SIZE, p_packet))
    {
        cout << "test share req() Error" << endl;
    }

    uint8_t buf[CMD_HDR_SIZE];
    memset(buf, 0, sizeof(buf));
    void* port = (void*)&g_pNetwork->port;
    IO_PORT *clientThd = (IO_PORT*) port;

    uint32_t fd_socket = clientThd->s;
    recv(fd_socket, buf, CMD_HDR_SIZE, 0);
    HEADERPACKET* msg = (HEADERPACKET*)buf;

    int datasize = msg->dataSize;

    unsigned char* recv_buf = new unsigned char[datasize + 1] ;

    memset(recv_buf, 0, datasize + 1);

	if(recv_binary(&g_pNetwork->port, datasize, recv_buf) == 0){
		cout << "recv_binary fail" << endl;
	}
    recv_buf[datasize] = '\0';
    string str = static_cast<std::string>(reinterpret_cast<const char *>(recv_buf));
    
    free(recv_buf);
    return str;
}

void performance_evaluation(){
    int num_of_share = 3;
    int threshold;
    int request_time = 0;
    vector<string> shares;
    map<pair<int,int>,int> request_times;
    for(num_of_share; num_of_share < 11 ; num_of_share ++){
        threshold = 2;
        for(threshold; threshold < num_of_share * 0.7; threshold++){
            gen_shares(num_of_share, threshold);
            request_time = 0;
            if(shares.size() != 0)shares.clear();
            sleep(1);
            while(shares.size() != threshold){
                string str = test_share_req();
                try{
                    for(int i = 0; i < shares.size(); i++){
                        if(shares[i].compare(str) == 0)throw -1;
                    }
                    shares.push_back(str);
                    request_time++;
                }    
                catch(int a)
                {
                    request_time++;
                    if (a == -1)continue;
                }
            }
            request_times[{num_of_share, threshold}] = request_time;
        }
    }
    cout << "------------------------------------------" << endl;

    for(auto &[key, value] : request_times){
        cout << "Number of shares : " << key.first << " / Number of thresholds : " << key.second << " / " << "request times : " << value << endl << "------------------------------------------" << endl;
    }
}

int main(int, char **)
{
    Read_Logger_cfg();
    
    // key GEN
    //key_generation();

    // Init Client
    // if (!test_initClient())
    // {
    //     cout << "init client error!!" << endl;
    //     return -1;
    // }

    // if (!initClient())
    // {
    //     cout << "init client error!!" << endl;
    //     return -1;
    // }
    
    // //performance_evaluation();
    // send_pubKey_to_server();
    
    open_camera();
    lamping_time();
    bool YUV_switch = false;
    while (true)
    {
        if (init() == -1)
        {
            break;
        }

        else
        {
            // capture frames
            capture();

            // show_frames(bgr_queue);

            if(YUV_switch){
                // convert frames to YUV420 and Y
                convert_frames2YUV(bgr_queue);
                // USE Canny Edge Detection with Y_Frames
                edge_detection_YUV(y_queue);
            }
            else{
                //convert frames to Grayscale
                convert_frames2gray(bgr_queue);
                // USE Canny Edge Detection with Grayscale
                edge_detection_BGR(G_queue);
            }
            
            // make Hash by edge_detected datas
            make_hash(feature_vector_queue);
            sign_hash(hash_queue);

            // Send Data to WEB UI
            //send_image_hash_to_UI(bgr_queue, y_queue);

            // send Datas to Server
            //send_data_to_server(cid_queue, hash_queue, hash_signed_queue, yuv420_queue);
            // initialize all settings
            init_all_settings();

        }
    }
}
