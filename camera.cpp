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
    http_client("http://" + ip + ":8090")
{
    http_client.SetTimeout(3000);
    if (auto_connect) {
        connect();
    }
}

Camera::~Camera()
{
}


std::string Camera::live_stream_url() const
{
    std::string url = "rtsp://" + camera_ip + ":8554/ds-stream";
    //std::string url = "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov";
    return url;
}

// enable the rpc
bool Camera::connect()
{
    camera_client.reset(new CameraClient(http_client, JSONRPC_CLIENT_V2));  // json-rpc 2.0
    return true;
}

// disable the rpc
bool Camera::disconnect()
{
    camera_client.reset();
    return true;
}

bool Camera::connected() const
{
    return camera_client != nullptr;
}

bool Camera::restart()
{
    if (!connected()) return false;
    try {
        camera_client->restart("reeplayer-nx");
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::shutdown()
{
    if (!connected()) return false;
    try {
        camera_client->shutdown("reeplayer-nx");
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

// call camera API to get system state
bool Camera::checkSystemState(Camera::SystemState &state)
{
    if (!connected()) return false;
    try {
        Json::Value json_state = camera_client->checkSystemState();
        Json::FastWriter writer;
        qDebug() << writer.write(json_state).c_str();
        state.battery = json_state["battery"].asFloat();
        state.temperature = json_state["temperature"].asFloat();
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::checkFolders(std::vector<Camera::FolderInfo> &folders)
{
    if (!connected()) return false;
    try {
        Json::Value json_folders = camera_client->checkFolders();
        for (auto it = json_folders.begin(); it != json_folders.end(); ++it) {
            Json::Value json_folder = *it;
            std::string folder_path = json_folder["path"].asString();
            folders.push_back(Camera::FolderInfo(folder_path));
        }
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::createFolder(const std::string &path)
{
    if (!connected()) return false;
    try {
        return camera_client->createFolder(path);
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::changeFolder(const std::string &path)
{
    if (!connected()) return false;
    try {
        return camera_client->changeFolder(path);
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::checkFiles(std::vector<Camera::FileInfo> &files, const std::string &path)
{
    if (!connected()) return false;
    try {
        Json::Value json_files = camera_client->checkFiles(path);
        for (auto it = json_files.begin(); it != json_files.end(); ++it) {
            Json::Value json_file = *it;
            std::string file_path = json_file["path"].asString();
            uint64_t file_size = json_file["size"].asUInt64();
            files.push_back(Camera::FileInfo(file_path, file_size));
        }
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::changeLiveMode(int mode)
{
    if (!connected()) return false;
    try {
        return camera_client->changeLiveMode(mode);
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

// call camera API to get state
bool Camera::checkLiveState(Camera::LiveState &state)
{
    if (!connected()) return false;
    try {
        Json::Value json_state = camera_client->checkLiveState();
        Json::FastWriter writer;
        qDebug() << writer.write(json_state).c_str();
        state.mode = json_state["mode"].asInt();
        state.streaming = json_state["streaming"].asBool();
        state.recording = json_state["recording"].asBool();
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::startCalib()
{
    if (!connected()) return false;
    try {
        return camera_client->startCalib();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::pauseCalib()
{
    if (!connected()) return false;
    try {
        return camera_client->pauseCalib();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::resetCalib()
{
    if (!connected()) return false;
    try {
        return camera_client->resetCalib();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::saveCalib()
{
    if (!connected()) return false;
    try {
        return camera_client->saveCalib();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

// return stream URL
bool Camera::startStreaming()
{
    if (!connected()) return false;
    try {
        qDebug() << "camera start streaming";
        return camera_client->startStreaming();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::stopStreaming()
{
    if (!connected()) return false;
    try {
        return camera_client->stopStreaming();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::startRecording()
{
    if (!connected()) return false;
    try {
        return camera_client->startRecording();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::stopRecording()
{
    if (!connected()) return false;
    try {
        return camera_client->stopRecording();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}


bool Camera::startUploading()
{
    if (!connected()) return false;
    try {
        return camera_client->startUploading();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::stopUploading()
{
    if (!connected()) return false;
    try {
        return camera_client->stopUploading();
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::checkUploadState(Camera::UploadState& state)
{
    if (!connected()) return false;
    try {
        Json::Value json_state = camera_client->checkUploadState();
        Json::FastWriter writer;
        qDebug() << writer.write(json_state).c_str();
        state.uploading = json_state["uploading"].asBool();
        return true;
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
};

bool Camera::setFieldCorners(const std::vector<Camera::Corner> &corners)
{
    if (!connected()) return false;
    try {
        Json::Value json_corners(Json::arrayValue);
        for (int i = 0; i < corners.size(); i++) {
            Camera::Corner corner = corners[i];
            Json::Value json_corner;
            json_corner["x"] = corner.x;
            json_corner["y"] = corner.y;
            json_corners.append(json_corner);
        }
        return camera_client->setFieldCorners(json_corners);
    }
    catch (JsonRpcException &ex) {
      qDebug() << ex.what();
    }
    return false;
}

bool Camera::checkFieldCorners(std::vector<Camera::Corner> &corners)
{
    if (!connected()) return false;
    try {
        Json::Value json_corners = camera_client->checkFieldCorners();
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
