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

    int GetID() const { return camera_id; }

    bool startVideo(std::string& url);
    bool stopVideo();

    VideoStream* GetVideoStream() const { return video_stream; }

public slots:
    void connectService();

signals:


private:
    // Config
    int camera_id; // ID used internally
    std::string camera_ip;
    std::string camera_name;
    std::string serial_num;

    // JSON-RPC interface for state and command


    // One video stream from one camera
    VideoStream *video_stream;
};

#endif // CAMERA_H
