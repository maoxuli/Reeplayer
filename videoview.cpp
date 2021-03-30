#include "videoview.h"
#include <json/json.h>

#include <cassert>
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

    // streaming state
    streaming_state_label = new ClickableLabel();
    streaming_state_label->setStyleSheet("background-color:rgba(0,0,0,0%)");
    streaming_state_label->setPixmap(QIcon(":images/gray-light.png").pixmap(25, 25));

    // recording state
    recording_state_label = new ClickableLabel();
    recording_state_label->setStyleSheet("background-color:rgba(0,0,0,0%)");
    recording_state_label->setPixmap(QIcon(":images/gray-light.png").pixmap(25, 25));

    // header frame
    // so can set the size of the header region
    header_frame = new QFrame(this);
    header_frame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    header_frame->setFixedHeight(35);

    camera_name_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    camera_name_label->setFixedHeight(30);

    streaming_state_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    streaming_state_label->setFixedSize(30, 30);

    recording_state_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    recording_state_label->setFixedSize(30, 30);

    QHBoxLayout *header_layout = new QHBoxLayout(header_frame);
    header_layout->setSpacing(0);
    header_layout->setMargin(0);
    header_layout->addWidget(streaming_state_label);
    header_layout->addWidget(camera_name_label);
    header_layout->addWidget(recording_state_label);

    // buttons
    play_button = new QPushButton(">", this);
    pause_button = new QPushButton("||", this);
    //stop_button = new QPushButton("X", this);
    zoom_in_button = new QPushButton("+", this);
    zoom_out_button = new QPushButton("-", this);
    actual_size_button = new QPushButton("=", this);
    fit_window_button = new QPushButton("[]", this);

    reset_calib_button = new QPushButton("Reset");
    save_calib_button = new QPushButton("Save");

    // buttons frame
    // so can set the size of the buttons region
    buttons_frame = new QFrame(this);
    buttons_frame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    buttons_frame->setFixedHeight(40);

    play_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    play_button->setFixedSize(40, 40);

    pause_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pause_button->setFixedSize(40, 40);

    //stop_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //stop_button->setFixedSize(40, 40);

    zoom_in_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    zoom_in_button->setFixedSize(40, 40);

    zoom_out_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    zoom_out_button->setFixedSize(40, 40);

    actual_size_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    actual_size_button->setFixedSize(40, 40);

    fit_window_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    fit_window_button->setFixedSize(40, 40);

    reset_calib_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    reset_calib_button->setFixedSize(60, 40);

    save_calib_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    save_calib_button->setFixedSize(60, 40);

    QHBoxLayout *buttons_layout = new QHBoxLayout(buttons_frame);
    buttons_layout->setAlignment(Qt::AlignCenter);
    buttons_layout->setSpacing(5);
    buttons_layout->setMargin(0);
    buttons_layout->addWidget(play_button);
    buttons_layout->addWidget(pause_button);
    //buttons_layout->addWidget(stop_button);
    buttons_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Preferred));
    buttons_layout->addWidget(zoom_in_button);
    buttons_layout->addWidget(zoom_out_button);
    buttons_layout->addWidget(actual_size_button);
    buttons_layout->addWidget(fit_window_button);
    buttons_layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Preferred));
    buttons_layout->addWidget(reset_calib_button);
    buttons_layout->addWidget(save_calib_button);

    // main layout
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    //main_layout->setSpacing(0);
    //main_layout->setMargin(0);
    main_layout->addWidget(header_frame);
    main_layout->addSpacerItem(new QSpacerItem(100, 100, QSizePolicy::Preferred, QSizePolicy::Expanding));
    main_layout->addWidget(buttons_frame, 0, Qt::AlignCenter);

    // signal-slot for buttons
    connect(streaming_state_label, &ClickableLabel::clicked, this, &VideoView::streaming);
    connect(recording_state_label, &ClickableLabel::clicked, this, &VideoView::recording);
    connect(play_button, &QPushButton::clicked, this, &VideoView::play);
    connect(pause_button, &QPushButton::clicked, this, &VideoView::pause);
    //connect(stop_button, &QPushButton::clicked, this, &VideoView::stop);
    connect(zoom_in_button, &QPushButton::clicked, this, &ImageView::zoomIn);
    connect(zoom_out_button, &QPushButton::clicked, this, &ImageView::zoomOut);
    connect(actual_size_button, &QPushButton::clicked, this, &ImageView::actualSize);
    connect(fit_window_button, &QPushButton::clicked, this, &ImageView::fitWindow);
    connect(reset_calib_button, &QPushButton::clicked, this, &VideoView::resetCalib);
    connect(save_calib_button, &QPushButton::clicked, this, &VideoView::saveCalib);

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
    updateState();
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
    //updateState();
    state_timer->start(1000);
    ImageView::showEvent(event);
}

void VideoView::hideEvent(QHideEvent *event)
{
    stop();
    state_timer->stop();
    ImageView::hideEvent(event);
}

void VideoView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(camera && event->button() == Qt::LeftButton) {
        emit tapVideo(camera->id());
        return;
    }
    ImageView::mouseDoubleClickEvent(event);
}

void VideoView::play()
{
    //qDebug() << "video view play...";
    if (!pipeline) {
        assert(camera);
        if (camera->startStreaming()) {
            std::string stream_url = camera->stream_url();
            std::string pipeline_str = CreateRtspSinkPipeline(stream_url);
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
    //qDebug() << "video view pause...";
    if (pipeline) {
        pipeline->SetPipelineState(GST_STATE_PAUSED);
    }
}

void VideoView::stop()
{
    //qDebug() << "video view stop...";
    video_timer->stop();
    if (pipeline) {
        pipeline->SetPipelineState(GST_STATE_NULL);
        pipeline.reset();
    }
    video_width = 0;
    video_height = 0;
    Reset();
}

// control calib
void VideoView::resetCalib()
{
    assert(camera);
    camera->resetCalib();
}

void VideoView::saveCalib()
{
    assert(camera);
    camera->saveCalib();
}

// control streaming
void VideoView::streaming()
{
    assert(camera);
    Camera::State state;
    if (camera->checkState(state)) {
        bool streaming_state = state.streaming;
        if (streaming_state) {
            camera->stopStreaming();
            stop();
        }
        else {
            camera->startStreaming();
            play();
        }
    }
}

// control recording
void VideoView::recording()
{
    assert(camera);
    Camera::State state;
    if (camera->checkState(state)) {
        bool recording_state = state.recording;
        if (recording_state)
            camera->stopRecording();
        else
            camera->startRecording();
    }
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
    if (!camera) return;
    assert(camera);
    camera_name_label->setText(camera->name().c_str());
    std::string streaming_image, recording_image;
    Camera::State state;
    if (!camera->connected()) { // disconnected
        streaming_image = ":images/gray-light.png";
        recording_image = ":images/gray-light.png";
        reset_calib_button->hide();
        save_calib_button->hide();
    }
    else if (camera->checkState(state)) {
        bool streaming_state = state.streaming;
        streaming_image = streaming_state ? ":images/green-light.png" : ":images/gray-light.png";
        bool recording_state = state.recording;
        recording_image = recording_state ? ":images/red-light.png" : ":images/gray-light.png";
        if (state.mode > 0) {
            reset_calib_button->show();
            save_calib_button->show();
        }
        else {
            reset_calib_button->hide();
            save_calib_button->hide();
        }
    }
    else { // failure
        streaming_image = ":images/yellow-light.png";
        recording_image = ":images/yellow-light.png";
        reset_calib_button->hide();
        save_calib_button->hide();
    }
    streaming_state_label->setPixmap(QIcon(streaming_image.c_str()).pixmap(25, 25));
    recording_state_label->setPixmap(QIcon(recording_image.c_str()).pixmap(25, 25));
}
