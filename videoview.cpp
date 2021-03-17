#include "videoview.h"

#include <QDebug>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

VideoView::VideoView(QWidget *parent) :
    ImageView(parent),
    image_width(0),
    image_height(0),
    is_streaming(false)
{
    // buttons
    qDebug() << "create buttons";
    connect_button = new QPushButton("#");
    play_button = new QPushButton(">");
    pause_button = new QPushButton("||");
    stop_button = new QPushButton("X");
    zoom_in_button = new QPushButton("+");
    zoom_out_button = new QPushButton("-");
    actual_size_button = new QPushButton("=");
    fit_window_button = new QPushButton("[]");

    // layout
    // buttons frame
    // used to set the size of the buttons region
    QFrame *buttons_frame = new QFrame(this);
    buttons_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buttons_frame->setFixedSize(400, 50);

    connect_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect_button->setFixedSize(40, 40);

    play_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    play_button->setFixedSize(40, 40);

    pause_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    pause_button->setFixedSize(40, 40);

    stop_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    stop_button->setFixedSize(40, 40);

    zoom_in_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    zoom_in_button->setFixedSize(40, 40);

    actual_size_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    actual_size_button->setFixedSize(40, 40);

    zoom_out_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    zoom_out_button->setFixedSize(40, 40);

    fit_window_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    fit_window_button->setFixedSize(40, 40);

    QHBoxLayout *buttons_layout = new QHBoxLayout();
    buttons_layout->addWidget(connect_button);
    buttons_layout->addWidget(play_button);
    buttons_layout->addWidget(pause_button);
    buttons_layout->addWidget(stop_button);
    buttons_layout->addWidget(zoom_in_button);
    buttons_layout->addWidget(actual_size_button);
    buttons_layout->addWidget(zoom_out_button);
    buttons_layout->addWidget(fit_window_button);
    buttons_frame->setLayout((buttons_layout));

    QVBoxLayout *main_layout = new QVBoxLayout(); 
    main_layout->addWidget(buttons_frame); 
    main_layout->setAlignment(Qt::AlignBottom | Qt::AlignCenter);
    setLayout(main_layout); 

    // signal-slot
    qDebug() << "connect signals";
    QObject::connect(&update_timer, &QTimer::timeout, this, &VideoView::update_frame);
    QObject::connect(connect_button, &QPushButton::clicked, this, &VideoView::connect);
    QObject::connect(play_button, &QPushButton::clicked, this, &VideoView::play);
    QObject::connect(pause_button, &QPushButton::clicked, this, &VideoView::pause);
    QObject::connect(stop_button, &QPushButton::clicked, this, &VideoView::stop);
    QObject::connect(zoom_in_button, &QPushButton::clicked, this, &VideoView::zoom_in);
    QObject::connect(actual_size_button, &QPushButton::clicked, this, &VideoView::actual_size);
    QObject::connect(zoom_out_button, &QPushButton::clicked, this, &VideoView::zoom_out);
    QObject::connect(fit_window_button, &QPushButton::clicked, this, &VideoView::fit_window);
}

VideoView::~VideoView()
{
    stop();
}

// connect to RTSP
void VideoView::connect()
{
    std::string url = "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov";
//    std::string url = "rtsp://192.168.1.190:8554/ds-stream";
    qDebug() << "connecting " << url.c_str();
    std::string pipeline_str = CreateRtspSinkPipeline(url);
    sink_pipeline.reset(new GstAppSinkPipeline());
    sink_pipeline->Initialize(pipeline_str);
    sink_pipeline->SetPipelineState(GST_STATE_PLAYING);
    update_timer.start(1000.0/30.0);
}

void VideoView::play()
{
    if (sink_pipeline)
        sink_pipeline->SetPipelineState(GST_STATE_PLAYING);
}

void VideoView::pause()
{
    if (sink_pipeline)
        sink_pipeline->SetPipelineState(GST_STATE_PAUSED);
}

void VideoView::stop()
{
    qDebug() << "disconnecting";
    update_timer.stop();
    if (sink_pipeline)
    {
        sink_pipeline->SetPipelineState(GST_STATE_NULL);
        sink_pipeline.reset();
    }
    is_streaming = false;
    image_width = 0;
    image_height = 0;
}

void VideoView::zoom_in()
{
    ZoomIn(0.1);
}

void VideoView::zoom_out()
{
    ZoomOut(0.1);
}

void VideoView::actual_size()
{
    SetScale(1.0, 1.0);
}

void VideoView::fit_window()
{
    FitWindow();
}

// update frame from sink pipeline
void VideoView::update_frame()
{
    if (!is_streaming)
    {
        qDebug() << "waiting";
        if (sink_pipeline->GetIsNewFrameAvailable() &&
            sink_pipeline->GetResolution(&image_width, &image_height))
        {
            Reset(image_width, image_height);
            FitWindow();
            is_streaming = true;
        }
    }
    else
    {
        //qDebug() << "streaming";
        void* buffer;
        if (sink_pipeline->GetIsNewFrameAvailable() &&
            sink_pipeline->GetLatestFrameBuffer(&buffer))
        {
            QImage image((unsigned char*)buffer, image_width, image_height, QImage::Format_RGB888);
            UpdateImage(image);
            sink_pipeline->ReleaseFrameBuffer();
        }
    }
}
