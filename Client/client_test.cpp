#include <iostream>
#include <string>

#include "client.cpp"
#define THIS_IS_CLIENT

int main(){
    if(!initClient()){
        cout << "init client error!!" << endl;
        return -1;
    }
    char* LID = "Testing LID";

    makePacket(Server, command, 0xa0, strlen(LID));
    void *p_packet = &sendDataPacket;

    if (!send_binary(&g_pNetwork->port, CMD_HDR_SIZE, p_packet))
    {
        cout << "packet send Error!!" << endl;
    }

    if (!send_binary(&g_pNetwork->port, strlen(LID), (void *)LID))
    {
        cout << "packet send Error!!" << endl;
    }
    
    while(1){
        sleep(10);
    }
}
