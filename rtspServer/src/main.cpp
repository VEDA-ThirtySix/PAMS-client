#include <rtsp_cam.hpp>
#include <rtsp.hpp>

#include <iostream>
#include <cstdlib>
#include <thread>

int main(int argc, char *argv[])
{
/*     RTSPCam rtspServer;
    std::thread capture_thread([&rtspServer]() {
         rtspServer.capture_frames();
     });

     rtspServer.Start(20001102, "rpi5_picamera", 600, 30);
    
     capture_thread.join(); */

    RTSP rtspServer(argv[1]);
    rtspServer.Start(20001102, "h264_streaming", 600, 30);

    return 0;
}
