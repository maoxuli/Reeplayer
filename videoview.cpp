#include "videoview.h"
#include <json/json.h>

#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>

VideoView::VideoView(Camera *camera, QWidget *parent) :
    ImageView(parent),
    video_width(0),
    video_height(0)
{
    assert(camera);
    this->camera = camera;
    this->video_stream = camera->videoStream();

    // camera name
    camera_name_label = new QLabel(this->camera->name().c_str());
    camera_name_label->setStyleSheet("color:white; background-color:rgba(0,0,0,0%)");
    camera_name_label->setFont(QFont( "Arial", 13, QFont::Bold));

    // recording state
    camera_state_label = new QLabel();
    camera_state_label->setStyleSheet("background-color:rgba(0,0,0,0%)");
    camera_state_label->setPixmap(QIcon(":images/red-light.png").pixmap(25, 25));

    // buttons
    QPushButton *play_button = new QPushButton(">", this);
    QPushButton *pause_button = new QPushButton("||", this);
    QPushButton *stop_button = new QPushButton("X", this);
    QPushButton *zoom_in_button = new QPushButton("+", this);
    QPushButton *zoom_out_button = new QPushButton("-", this);
    QPushButton *actual_size_button = new QPushButton("=", this);
    QPushButton *fit_window_button = new QPushButton("[]", this);

    // header frame
    // so can set the size of the header region
    QFrame *header_frame = new QFrame(this);
    header_frame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    header_frame->setFixedHeight(35);

    camera_name_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    camera_name_label->setFixedHeight(30);

    camera_state_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    camera_state_label->setFixedSize(30, 30);

    QHBoxLayout *header_layout = new QHBoxLayout();
    header_layout->setSpacing(0);
    header_layout->setMargin(0);
    header_layout->addWidget(camera_name_label);
    header_layout->addWidget(camera_state_label);
    header_frame->setLayout(header_layout);

    // buttons frame
    // so can set the size of the buttons region
    QFrame *buttons_frame = new QFrame(this);
    buttons_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttons_frame->setFixedSize(320, 40);

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
    buttons_layout->setSpacing(0);
    buttons_layout->setMargin(0);
    buttons_layout->addWidget(play_button);
    buttons_layout->addWidget(pause_button);
    buttons_layout->addWidget(stop_button);
    buttons_layout->addWidget(zoom_in_button);
    buttons_layout->addWidget(zoom_out_button);
    buttons_layout->addWidget(actual_size_button);
    buttons_layout->addWidget(fit_window_button);
    buttons_frame->setLayout((buttons_layout));

    QSpacerItem *spacer = new QSpacerItem(100, 100, QSizePolicy::Preferred, QSizePolicy::Expanding);

    // main layout
    // put buttons on the bottom center
    QVBoxLayout *main_layout = new QVBoxLayout(); 
    //main_layout->setSpacing(0);
    //main_layout->setMargin(0);
    main_layout->addWidget(header_frame);
    main_layout->addSpacerItem(spacer);
    main_layout->addWidget(buttons_frame, 0, Qt::AlignCenter);
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

    // update timer
    video_timer = new QTimer(this);
    connect(video_timer, &QTimer::timeout, this, &VideoView::updateFrame);

    state_timer = new QTimer(this);
    connect(state_timer, &QTimer::timeout, this, &VideoView::updateState);
}

VideoView::~VideoView()
{
    delete video_timer;
    delete state_timer;
}

void VideoView::showEvent(QShowEvent *event)
{
    assert(video_stream);
    video_stream->play();
    state_timer->start(1000);
    ImageView::showEvent(event);
}

void VideoView::hideEvent(QHideEvent *event)
{
    assert(video_stream);
    video_stream->stop();
    state_timer->stop();
    ImageView::hideEvent(event);
}

void VideoView::playVideo()
{
    qDebug() << "play video";
    video_timer->start(1000/30.0);
}

void VideoView::pauseVideo()
{
    qDebug() << "pause video";
}

void VideoView::stopVideo()
{
    qDebug() << "stop video";
    video_timer->stop();
    video_width = 0;
    video_height = 0;
    Reset();
}

// timing update
void VideoView::updateFrame()
{
    if (video_width == 0 || video_height == 0) {
        if (!video_stream->GetResolution(&video_width, &video_height)) {
            return;
        }
        qDebug() << "reset play: " << video_width << "x" << video_height;
        Reset(video_width, video_height);
        fitWindow();
    }
    void* buffer;
    assert(video_stream);
    if (video_stream->GetVideoFrame(&buffer)) {
        QImage image((unsigned char*)buffer, video_width, video_height, QImage::Format_RGB888);
        UpdateImage(image);
    }
}

// timing update
void VideoView::updateState()
{
    assert(camera);
    camera_name_label->setText(camera->name().c_str());
    std::string state;
    if (camera->updateState(state)) {
        // parse json string
        Json::Reader reader;
        Json::Value root;
        std::string image;
        if (reader.parse(state, root)) {
            // link and recording state
            bool link_state = root["link_state"].asBool();
            bool recording_state = root["recording_state"].asBool();
            if (!link_state)
                image = ":images/yellow-light.png";
            else if (recording_state)
                image = ":images/red-light.png";
            else
                image = ":images/gray-light.png";
            camera_state_label->setPixmap(QIcon(image.c_str()).pixmap(25, 25));
        }
    }
}
