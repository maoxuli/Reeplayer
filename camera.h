#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

#include "cameraclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

class VideoStream;
class Camera : public QObject
{
    Q_OBJECT

public:
    explicit Camera(int id, const std::string& ip,
                    const std::string& name = "", bool is_auto = false);
    ~Camera();

    int id() const { return camera_id; }
    std::string ip() const { return camera_ip; }
    std::string name() const { return camera_name; }
    bool is_auto() const { return auto_connect; }

    bool restart();
    bool shutdown();

    bool checkState(std::string &state);
    bool checkFiles(std::string &files);

    bool startStreaming(std::string &url);
    bool stopStreaming();

    bool startRecording();
    bool stopRecording();

    bool startUploading();
    bool stopUploading();

public slots:
    void connectService();

signals:

private:
    // config
    int camera_id; // ID used internally
    std::string camera_ip;
    std::string camera_name;
    bool auto_connect;

    // json rpc
    jsonrpc::HttpClient http_client;
    CameraClient camera_client;
};

#endif // CAMERA_H
