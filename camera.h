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

    bool connect();
    bool disconnect();
    bool connected() const;

    std::string stream_url() const;

    bool restart();
    bool shutdown();
    bool changeMode(int mode);

    struct State
    {
        int mode;
        bool streaming;
        bool recording;
        bool uploading;
        float battery;
        float temperature;
        std::string folder;
    };

    bool checkState(State &state);

    bool resetCalib();
    bool saveCalib();

    bool startStreaming();
    bool stopStreaming();

    bool startRecording();
    bool stopRecording();

    bool startUploading();
    bool stopUploading();

    bool createFolder(const std::string &folder);
    bool changeFolder(const std::string &folder);

    struct Folder
    {
        std::string name;
        Folder(const std::string& folder_name) : name(folder_name) {}
    };

    bool checkFolders(std::vector<Folder> &folders);

    struct File
    {
        std::string name;
        uint64_t size;
        float uploading;
        File(const std::string& file_name, uint64_t file_size, float file_uploading)
            : name(file_name), size(file_size), uploading(file_uploading) {}
    };

    bool checkFiles(std::vector<File> &files, const std::string &folder = "");

    struct Corner
    {
       int x;
       int y;
       Corner(int corner_x, int corner_y) : x(corner_x), y(corner_y) {}
    };

    bool setFieldCorners(const std::vector<Corner> &corners);
    bool fieldCorners(std::vector<Corner> &corners);

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
