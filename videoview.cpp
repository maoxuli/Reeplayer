#include "videoview.h"

#include <QDebug>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

VideoView::VideoView(VideoStream *stream, QWidget *parent) :
    ImageView(parent),
    video_stream(stream),
    video_width(0),
    video_height(0)
{
    // buttons
    QPushButton *play_button = new QPushButton(">", this);
    QPushButton *pause_button = new QPushButton("||", this);
    QPushButton *stop_button = new QPushButton("X", this);
    QPushButton *zoom_in_button = new QPushButton("+", this);
    QPushButton *zoom_out_button = new QPushButton("-", this);
    QPushButton *actual_size_button = new QPushButton("=", this);
    QPushButton *fit_window_button = new QPushButton("[]", this);

    // buttons frame
    // so can set the size of the buttons region
    QFrame *buttons_frame = new QFrame(this);
    buttons_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttons_frame->setFixedSize(320, 50);

    play_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    play_button->setFixedSize(40, 40);

    pause_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pause_button->setFixedSize(40, 40);

    stop_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    stop_button->setFixedSize(40, 40);

    zoom_in_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    zoom_in_button->setFixedSize(40, 40);

    zoom_out_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    zoom_out_button->setFixedSize(40, 40);

    actual_size_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    actual_size_button->setFixedSize(40, 40);

    fit_window_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    fit_window_button->setFixedSize(40, 40);

    QHBoxLayout *buttons_layout = new QHBoxLayout();
    buttons_layout->addWidget(play_button);
    buttons_layout->addWidget(pause_button);
    buttons_layout->addWidget(stop_button);
    buttons_layout->addWidget(zoom_in_button);
    buttons_layout->addWidget(zoom_out_button);
    buttons_layout->addWidget(actual_size_button);
    buttons_layout->addWidget(fit_window_button);
    buttons_frame->setLayout((buttons_layout));

    // main layout
    // put buttons on the bottom center
    QVBoxLayout *main_layout = new QVBoxLayout(); 
    main_layout->addWidget(buttons_frame); 
    main_layout->setAlignment(Qt::AlignBottom | Qt::AlignCenter);
    setLayout(main_layout); 

    // signal-slot for zooming
    connect(zoom_in_button, &QPushButton::clicked, this, &ImageView::zoomIn);
    connect(zoom_out_button, &QPushButton::clicked, this, &ImageView::zoomOut);
    connect(actual_size_button, &QPushButton::clicked, this, &ImageView::actualSize);
    connect(fit_window_button, &QPushButton::clicked, this, &ImageView::fitWindow);

    // signal-slot for video streaming
    connect(play_button, &QPushButton::clicked, video_stream, &VideoStream::play);
    connect(pause_button, &QPushButton::clicked, video_stream, &VideoStream::pause);
    connect(stop_button, &QPushButton::clicked, video_stream, &VideoStream::stop);

    connect(video_stream, &VideoStream::played, this, &VideoView::playVideo);
    connect(video_stream, &VideoStream::paused, this, &VideoView::pauseVideo);
    connect(video_stream, &VideoStream::stopped, this, &VideoView::stopVideo);
    connect(video_stream, &VideoStream::updated, this, &VideoView::updateFrame);
}

VideoView::~VideoView()
{

}

void VideoView::showEvent(QShowEvent *event)
{
    assert(video_stream);
    video_stream->play();
    ImageView::showEvent(event);
}

void VideoView::hideEvent(QHideEvent *event)
{
    assert(video_stream);
    video_stream->stop();
    ImageView::hideEvent(event);
}

void VideoView::playVideo()
{
    assert(video_stream);
    int width, height;
    if (video_stream->GetResolution(&width, &height)) {
        qDebug() << "play video: " << video_width << "x" << video_height;
        if (width != video_width || height != video_height) {
            Reset(video_width, video_height);
            fitWindow();
        }
    }
}

void VideoView::pauseVideo()
{
    qDebug() << "pause video";
}

void VideoView::stopVideo()
{
    qDebug() << "stop video";
    video_width = 0;
    video_height = 0;
    Reset();
}

// Notification of update frame is ready
void VideoView::updateFrame()
{
    if (video_width == 0 || video_height == 0) {
        if (video_stream->GetResolution(&video_width, &video_height)) {
            qDebug() << "play video: " << video_width << "x" << video_height;
            Reset(video_width, video_height);
            fitWindow();
        }
        return;
    }
    void* buffer;
    assert(video_stream);
    if (video_stream->GetVideoFrame(&buffer)) {
        QImage image((unsigned char*)buffer, video_width, video_height, QImage::Format_RGB888);
        UpdateImage(image);
    }
}
