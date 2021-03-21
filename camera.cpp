#include "camera.h"
#include "cameraclient.h"

#include <QDebug>
#include <QDateTime>

using namespace jsonrpc;

Camera::Camera(int id, const std::string& ip,
               const std::string& name, bool is_auto) :
    camera_id(id),
    camera_ip(ip),
    camera_name(name),
    auto_connect(is_auto),
    http_client("http://" + ip + ":8080"),
    camera_client(http_client, JSONRPC_CLIENT_V2) // json-rpc 2.0
{
    qDebug("Camera contructed");
    if (auto_connect)
        connectService();
}

Camera::~Camera()
{
    stopStreaming();
}

bool Camera::restart()
{

}

bool Camera::shutdown()
{
    return true;
}

// start pulling with timer
void Camera::connectService()
{

}

// call camera API to get state
bool Camera::checkState(std::string &state)
{
    try {
        //Json::Value json_state = camera_client.checkState();
        state = "{\"link_state\":true,\"recording_state\":true,\"uploading_state\":false,\"battery_state\":false}";
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::checkFiles(std::string &files)
{
    try {
        //Json::Value json_files = camera_client.checkFiles("");
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

// call camera API to start streaming, return stream URL
bool Camera::startStreaming(std::string &url)
{
    try {
        url = camera_client.startStreaming();
        url = "rtsp://192.168.1.190:8554/ds-stream";
        //url = "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov";
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

// call camera API to stop video streaming
bool Camera::stopStreaming()
{
    try {
        camera_client.stopStreaming();
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}
