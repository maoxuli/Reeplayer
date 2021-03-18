#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include "videostream.h"

class VideoStream;
class Camera : public QObject
{
    Q_OBJECT

public:
    explicit Camera(int id, const std::string& ip, const std::string& name = "");
    ~Camera();

    int id() const { return camera_id; }
    std::string ip() const { return camera_ip; }
    std::string name() const { return camera_name; }

    bool updateState(std::string &state);

    bool startVideo(std::string &url);
    bool stopVideo();

    VideoStream* videoStream() const { return video_stream; }

public slots:
    void connectService();

signals:


private:
    // Config
    int camera_id; // ID used internally
    std::string camera_ip;
    std::string camera_name;

    // JSON-RPC interface for state and command
    std::string camera_state;
    int64_t update_time;

    // One video stream from one camera
    VideoStream *video_stream;
};

#endif // CAMERA_H
