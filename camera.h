#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

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

    bool updateState(std::string &state);

    bool startVideo(std::string &url);
    bool stopVideo();

public slots:
    void connectService();

signals:


private:
    // Config
    int camera_id; // ID used internally
    std::string camera_ip;
    std::string camera_name;
    bool auto_connect;

    // JSON-RPC interface for state and command
    std::string camera_state;
    int64_t update_time;
};

#endif // CAMERA_H
