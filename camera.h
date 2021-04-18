#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <memory>

#include "cameraclient.h"
#include <jsonrpccpp/client.h>
#include <jsonrpccpp/client/connectors/httpclient.h>

class VideoStream;
class Camera : public QObject
{
    Q_OBJECT

public:
    explicit Camera(int id, const std::string &ip,
                    const std::string &name = "", bool is_auto = false);
    ~Camera();

    int id() const { return camera_id; }
    std::string ip() const { return camera_ip; }
    std::string name() const { return camera_name; }
    bool is_auto() const { return auto_connect; }

    // rpc connection
    bool connect();
    bool disconnect();
    bool connected() const;

    std::string live_stream_url() const;

    // system
    bool restart();
    bool shutdown();

    struct SystemState
    {
        float battery;
        float temperature;
    };

    bool checkSystemState(SystemState &state);

    // file system
    struct FolderInfo
    {
        std::string path;
        FolderInfo(const std::string& folder_path) : path(folder_path) {}
    };

    bool checkFolders(std::vector<FolderInfo> &folders);

    bool createFolder(const std::string &path);
    bool changeFolder(const std::string &path);
    bool checkCurrentFolder(const std::string &path);

    struct FileInfo
    {
        std::string path;
        uint64_t size;
        FileInfo(const std::string& file_path, uint64_t file_size)
            : path(file_path), size(file_size) {}
    };

    bool checkFiles(std::vector<FileInfo> &files, const std::string &path = "");

    // live stitching
    bool changeLiveMode(int mode);
    bool startCalib();
    bool pauseCalib();
    bool resetCalib();
    bool saveCalib();
    bool startStreaming();
    bool stopStreaming();
    bool startRecording();
    bool stopRecording();

    struct LiveState
    {
        int mode;
        bool streaming;
        bool recording;
    };

    bool checkLiveState(LiveState &state);

    // file uploading
    bool startUploading();
    bool stopUploading();

    struct UploadState
    {
       bool uploading;
    };

    bool checkUploadState(UploadState& state);

    // field corners
    struct Corner
    {
       int x;
       int y;
       Corner(int corner_x, int corner_y) : x(corner_x), y(corner_y) {}
    };

    bool setFieldCorners(const std::vector<Corner> &corners);
    bool checkFieldCorners(std::vector<Corner> &corners);

public slots:

signals:

private:
    // config
    int camera_id; // ID used internally
    std::string camera_ip;
    std::string camera_name;
    bool auto_connect;

    // json rpc
    jsonrpc::HttpClient http_client;
    std::shared_ptr<CameraClient> camera_client;
};

#endif // CAMERA_H
