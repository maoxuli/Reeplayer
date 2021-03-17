#include "videostream.h"
#include "camera.h"

#include <QDebug>

VideoStream::VideoStream(Camera *camera)
{
    assert(camera);
    this->camera = camera;

    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, &VideoStream::checkUpdate);
}

VideoStream::~VideoStream()
{
    delete update_timer;

    assert(camera);
    camera->stopVideo();
}

// Check video status, start video if necessary
// Set gstreamer pipeline to playing state
void VideoStream::play()
{
    if (!pipeline) {
        assert(camera);
        std::string url;
        if (camera->startVideo(url)) {
            qDebug() << "video stream URL: " << url.c_str();
            std::string pipeline_str = CreateRtspSinkPipeline(url);
            pipeline.reset(new GstAppSinkPipeline());
            pipeline->Initialize(pipeline_str);
        }
    }
    qDebug() << "play video stream...";
    if (pipeline) {
        pipeline->SetPipelineState(GST_STATE_PLAYING);
        update_timer->start(1000/30.0);
    }
    emit played();
}

// Set gstreamer pipeline to pause state
void VideoStream::pause()
{
    qDebug() << "pause video stream...";
    if (pipeline) {
        pipeline->SetPipelineState(GST_STATE_PAUSED);
    }
    emit paused();
}

// Set gstreamer pipeline to stop state
// Call camera API to stop video streaming
void VideoStream::stop()
{
    qDebug() << "stop video stream...";
    update_timer->stop();
    if (pipeline) {
        pipeline->SetPipelineState(GST_STATE_NULL);
        pipeline.reset();
    }
    emit stopped();
    assert(camera);
    camera->stopVideo();
}

void VideoStream::checkUpdate()
{
    if (pipeline && pipeline->IsNewFrameAvailable())
        emit updated();
}

bool VideoStream::GetResolution(int *width, int *height)
{
    return (pipeline && pipeline->GetResolution(width, height));
}

bool VideoStream::GetVideoFrame(void **buffer)
{
    return (pipeline && pipeline->GetLatestFrameBuffer(buffer));
}
