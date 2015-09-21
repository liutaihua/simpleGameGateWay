#include <iostream>
#include <stdio.h>
#include <ctime>
#include <unistd.h>

static const int kFrameCount = 60;
static const float kSecondsPerFrame = 1.0f/kFrameCount;

#include "newlist/ServerClientSocket.h"
//#include "asioclient.h"


int main(){
//    Logger::ResetInstance("test.log", LOGLEVEL_DEBUG);
    clock_t frameStart;
    frameStart = std::clock();
    uint64 frameCount = 0;
    double diff;
    double interval = 0.05;
    g_SocketsManager =new ClientSocketManager("0.0.0.0", 8888);

    DBG("SOCKET PREPARE OK ...");
    //DBG("%d",sizeof(float));

    while(true){
        usleep(CLOCKS_PER_SEC);
        g_SocketsManager->Update();
//        diff = (std::clock() - frameStart) / static_cast<double>(CLOCKS_PER_SEC);
//        double timeLeftForNextUpdate = interval - diff;
//        // timeLeftForNextUpdate 是当前时钟里开始时刻相差之后， 大于每帧的时间，即需要睡眠来跳过的， 直到下一帧来临
//        if (timeLeftForNextUpdate > 0){
//            ////std::cout<< "now usleep:"<< std::to_string(timeLeftForNextUpdate*1000)<<"\n";
//            usleep(timeLeftForNextUpdate*1000);
//        }
//        frameCount++;
////        if(frameCount % kFrameCount == 0)
////            DBG("now: %d", frameCount);
    }

}
