#include <iostream>
#include <string>

#include "client.cpp"

int main(){
    if(!initClient()){
        cout << "init client error!!" << endl;
        return -1;
    }

    makePacket(Server, command, 0x00, 0);
    void *p_packet = &sendDataPacket;

    if (!send_binary(&g_pNetwork->port, CMD_HDR_SIZE, p_packet))
    {
        cout << "packet send Error!!" << endl;
    }
}