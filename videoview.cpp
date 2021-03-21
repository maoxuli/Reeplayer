#include "videoview.h"
#include <json/json.h>

#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QMouseEvent>

VideoView::VideoView(Camera *_camera, QWidget *parent) :
    ImageView(parent),
    camera(0),
    video_width(0),
    video_height(0)
{
    // camera name
    camera_name_label = new QLabel("Camera");
    camera_name_label->setStyleSheet("color:white; background-color:rgba(0,0,0,0%)");
    camera_name_label->setFont(QFont( "Arial", 13, QFont::Bold));

    // recording state
    camera_state_label = new QLabel();
    camera_state_label->setStyleSheet("background-color:rgba(0,0,0,0%)");
    camera_state_label->setPixmap(QIcon(":images/gray-light.png").pixmap(25, 25));

    // header frame
    // so can set the size of the header region
    header_frame = new QFrame(this);
    header_frame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    header_frame->setFixedHeight(35);

    camera_name_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    camera_name_label->setFixedHeight(30);

    camera_state_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    camera_state_label->setFixedSize(30, 30);

    QHBoxLayout *header_layout = new QHBoxLayout(header_frame);
    header_layout->setSpacing(0);
    header_layout->setMargin(0);
    header_layout->addWidget(camera_name_label);
    header_layout->addWidget(camera_state_label);

    // buttons
    play_button = new QPushButton(">", this);
    pause_button = new QPushButton("||", this);
    stop_button = new QPushButton("X", this);
    zoom_in_button = new QPushButton("+", this);
    zoom_out_button = new QPushButton("-", this);
    actual_size_button = new QPushButton("=", this);
    fit_window_button = new QPushButton("[]", this);

    // buttons frame
    // so can set the size of the buttons region
    buttons_frame = new QFrame(this);
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

    QHBoxLayout *buttons_layout = new QHBoxLayout(buttons_frame);
    buttons_layout->setSpacing(0);
    buttons_layout->setMargin(0);
    buttons_layout->addWidget(play_button);
    buttons_layout->addWidget(pause_button);
    buttons_layout->addWidget(stop_button);
    buttons_layout->addWidget(zoom_in_button);
    buttons_layout->addWidget(zoom_out_button);
    buttons_layout->addWidget(actual_size_button);
    buttons_layout->addWidget(fit_window_button);

    // main layout
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    //main_layout->setSpacing(0);
    //main_layout->setMargin(0);
    main_layout->addWidget(header_frame);
    main_layout->addSpacerItem(new QSpacerItem(100, 100, QSizePolicy::Preferred, QSizePolicy::Expanding));
    main_layout->addWidget(buttons_frame, 0, Qt::AlignCenter);

    // signal-slot for buttons
    connect(play_button, &QPushButton::clicked, this, &VideoView::play);
    connect(pause_button, &QPushButton::clicked, this, &VideoView::pause);
    connect(stop_button, &QPushButton::clicked, this, &VideoView::stop);
    connect(zoom_in_button, &QPushButton::clicked, this, &ImageView::zoomIn);
    connect(zoom_out_button, &QPushButton::clicked, this, &ImageView::zoomOut);
    connect(actual_size_button, &QPushButton::clicked, this, &ImageView::actualSize);
    connect(fit_window_button, &QPushButton::clicked, this, &ImageView::fitWindow);

    // update timer
    video_timer = new QTimer(this);
    state_timer = new QTimer(this);

    connect(video_timer, &QTimer::timeout, this, &VideoView::updateFrame);
    connect(state_timer, &QTimer::timeout, this, &VideoView::updateState);

    // initial
    enableControl(true);
    attachCamera(_camera);
}

VideoView::~VideoView()
{
    stop();
    delete video_timer;
    delete state_timer;
}

void VideoView::attachCamera(Camera *_camera)
{
    stop();
    camera = _camera;
}

void VideoView::enableControl(bool enable)
{
    if (enable) {
        buttons_frame->show();
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
    else {
        buttons_frame->hide();
        setDragMode(QGraphicsView::NoDrag);
    }
}

void VideoView::showEvent(QShowEvent *event)
{
    state_timer->start(1000);
    ImageView::showEvent(event);
}

void VideoView::hideEvent(QHideEvent *event)
{
    stop();
    state_timer->stop();
    ImageView::hideEvent(event);
}

void VideoView::mousePressEvent(QMouseEvent *event)
{
    if(camera && event->button() == Qt::RightButton) {
        emit tapVideo(camera->id());
        return;
    }
    ImageView::mousePressEvent(event);
}

void VideoView::play()
{
    qDebug() << "video view play...";
    if (!pipeline) {
        assert(camera);
        std::string url;
        if (camera->startStreaming(url)) {
            std::string pipeline_str = CreateRtspSinkPipeline(url);
            pipeline.reset(new GstAppSinkPipeline());
            //pipeline->set_is_verbose(true);
            pipeline->Initialize(pipeline_str);
        }
    }

    if (pipeline) {
        pipeline->SetPipelineState(GST_STATE_PLAYING);
        video_timer->start(1000/30.0);
    }
}

void VideoView::pause()
{
    qDebug() << "video view pause...";
    if (pipeline) {
        pipeline->SetPipelineState(GST_STATE_PAUSED);
    }
}

void VideoView::stop()
{
    qDebug() << "video view stop...";
    video_timer->stop();
    if (pipeline) {
        pipeline->SetPipelineState(GST_STATE_NULL);
        pipeline.reset();
        camera->stopStreaming();
    }
    video_width = 0;
    video_height = 0;
    Reset();
}

// timing update video
void VideoView::updateFrame()
{
    assert(pipeline);
    if (video_width == 0 || video_height == 0) {
        if (!pipeline->GetResolution(&video_width, &video_height)) {
            //qDebug() << "Failed get resolution, waiting";
            return;
        }
        qDebug() << "reset play: " << video_width << "x" << video_height;
        Reset(video_width, video_height);
        fitWindow();
    }
    void* buffer;
    if (pipeline->GetLatestFrameBuffer(&buffer)) {
        QImage image((unsigned char*)buffer, video_width, video_height, QImage::Format_RGB888);
        UpdateImage(image);
    }
}

// timing update state
void VideoView::updateState()
{
    assert(camera);
    camera_name_label->setText(camera->name().c_str());
    std::string state;
    if (camera->checkState(state)) {
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
