#include "camera.h"
#include "cameraclient.h"

#include <QDebug>
#include <QDateTime>

using namespace jsonrpc;

Camera::Camera(int id, const std::string &ip,
               const std::string &name, bool is_auto) :
    camera_id(id),
    camera_ip(ip),
    camera_name(name),
    auto_connect(is_auto),
    http_client("http://" + ip + ":8080"),
    camera_client(http_client, JSONRPC_CLIENT_V2) // json-rpc 2.0
{
    if (auto_connect)
        connectService();
}

Camera::~Camera()
{
    stopStreaming();
}

std::string Camera::stream_url() const
{
    std::string url = "rtsp://" + camera_ip + ":8554/ds-stream";
    //std::string url = "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov";
    return url;
}

// start pulling with timer
void Camera::connectService()
{

}

bool Camera::restart()
{
    try {
        camera_client.restart();
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::shutdown()
{
    try {
        camera_client.shutdown();
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::changeMode(int mode)
{
    try {
        return camera_client.changeMode(mode);
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

// call camera API to get state
bool Camera::checkState(Camera::State &state)
{
    try {
        Json::Value json_state = camera_client.checkState();
        Json::FastWriter writer;
        qDebug() << writer.write(json_state).c_str();
        state.mode = json_state["mode"].asInt();
        state.streaming = json_state["streaming"].asBool();
        state.recording = json_state["recording"].asBool();
        state.uploading = json_state["uploading"].asBool();
        state.battery = json_state["battery"].asFloat();
        state.temperature = json_state["temperature"].asFloat();
        state.folder = json_state["folder"].asString();
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::resetCalib()
{
    try {
        return camera_client.resetCalib();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::saveCalib()
{
    try {
        return camera_client.saveCalib();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

// return stream URL
bool Camera::startStreaming()
{
    try {
        return camera_client.startStreaming();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::stopStreaming()
{
    try {
        return camera_client.stopStreaming();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::startRecording()
{
    try {
        return camera_client.startRecording();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::stopRecording()
{
    try {
        return camera_client.stopRecording();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}


bool Camera::startUploading()
{
    try {
        return camera_client.startUploading();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::stopUploading()
{
    try {
        return camera_client.stopUploading();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::createFolder(const std::string &folder)
{
    try {
        return camera_client.createFolder(folder);
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::changeFolder(const std::string &folder)
{
    try {
        return camera_client.changeFolder(folder);
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::checkFolders(std::vector<Camera::Folder> &folders)
{
    try {
        Json::Value json_folders = camera_client.checkFolders();
        for (auto it = json_folders.begin(); it != json_folders.end(); ++it) {
            Json::Value json_folder = *it;
            std::string folder_name = json_folder["name"].asString();
            folders.push_back(Camera::Folder(folder_name));
        }
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::checkFiles(std::vector<Camera::File> &files, const std::string& folder)
{
    try {
        Json::Value json_files = camera_client.checkFiles(folder);
        for (auto it = json_files.begin(); it != json_files.end(); ++it) {
            Json::Value json_file = *it;
            std::string file_name = json_file["name"].asString();
            uint64_t file_size = json_file["size"].asUInt64();
            float file_uploading = json_file["uploading"].asFloat();
            files.push_back(Camera::File(file_name, file_size, file_uploading));
        }
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::setFieldCorners(const std::vector<Camera::Corner> &corners)
{
    try {
        Json::Value json_corners(Json::arrayValue);
        for (int i = 0; i < corners.size(); i++) {
            Camera::Corner corner = corners[i];
            Json::Value json_corner;
            json_corner["x"] = corner.x;
            json_corner["y"] = corner.y;
            json_corners.append(json_corner);
        }
        return camera_client.setFieldCorners(json_corners);
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::fieldCorners(std::vector<Camera::Corner> &corners)
{
    try {
        Json::Value json_corners = camera_client.fieldCorners();
        for (auto it = json_corners.begin(); it != json_corners.end(); ++it) {
            Json::Value json_corner = *it;
            int corner_x = json_corner["x"].asInt();
            int corner_y = json_corner["y"].asInt();
            corners.push_back(Camera::Corner(corner_x, corner_y));
        }
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}
