#include "camera.h"
#include "videostream.h"

#include <QDebug>
#include <QDateTime>

Camera::Camera(int id, const std::string& ip, const std::string& name) :
    camera_id(id),
    camera_ip(ip),
    camera_name(name)
{
    // dummy state
    update_time = 0;
    camera_state = "{\"link_state\":true,\"recording_state\":true,\"uploading_state\":false,\"battery_state\":false}";

    // one video stream from one camera by default
    video_stream = new VideoStream(this);
}

Camera::~Camera()
{
    delete video_stream;
}

// Establish connection to service
void Camera::connectService()
{

}

// Call camera API to get state
bool Camera::updateState(std::string &state)
{
    qDebug() << "camera update state...";
    qDebug() << camera_state.c_str();
    state = camera_state;
    update_time = QDateTime::currentSecsSinceEpoch();
    return true;
}

// Call camera API to start streaming, return stream URL
bool Camera::startVideo(std::string &url)
{
    qDebug() << "camera start video...";
    url = "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov";
    //url = "rtsp://192.168.1.190:8554/ds-stream";
    qDebug() << url.c_str();
    return true;
}

// 3. Call camera API to stop video streaming
bool Camera::stopVideo()
{
    qDebug() << "camera stop video...";
    return true;
}
