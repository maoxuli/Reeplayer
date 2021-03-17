#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include <QObject>
#include <QTimer>

#include <memory>
#include "GstAppSinkPipeline.h"
#include "GstreamerPipelines.h"

class Camera;
class VideoStream : public QObject
{
    Q_OBJECT

public:
    explicit VideoStream(Camera *camera);
    ~VideoStream();

    // Get video resolution
    bool GetResolution(int *width, int *height);

    // check availability with NULL param
    bool GetVideoFrame(void **buffer = NULL);

public slots:
    void play();
    void pause();
    void stop();

signals:
    void played();
    void paused();
    void stopped();
    void updated();

private slots:
    void checkUpdate();

private:
    // Camera API
    Camera *camera;

    // GStreamer pipeline
    std::shared_ptr<GstAppSinkPipeline> pipeline;
    QTimer *update_timer;
};

#endif // VIDEOSTREAM_H
