#include "mainwindow.h"
#include "imageview.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QWidget>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    is_streaming(false),
    image_width(0),
    image_height(0)
{
    setWindowTitle("Reeplayer");
    QRect rc = QDesktopWidget().availableGeometry();
    qDebug() << "screen: " << rc;
    int w = (rc.height() - 100) * 9 / 16;
    qDebug() << "w: " << w;
    QRect window_rc((rc.width() - w)/2, rc.top(), w, rc.height() - 100);
    qDebug() << "window_rc: " << window_rc;
    setGeometry(window_rc);

    // client area
    QWidget *center_widget = new QWidget();
    setCentralWidget(center_widget);

    // status bar
    QStatusBar *status_bar = new QStatusBar(center_widget);
    setStatusBar(status_bar);

    // image view
    image_view_0 = new ImageView(center_widget);
    image_view_1 = new ImageView(center_widget);

    //image_view_0->setStyleSheet("background-color: black");
    //image_view_1->setStyleSheet("background-color: black");

    // buttons
    qDebug() << "create buttons";
    QFrame *menu_bar = new QFrame(center_widget);
    menu_bar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    menu_bar->setFixedHeight(50);

    QPushButton *connect_button = new QPushButton("Connect", menu_bar);
    connect_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    connect_button->setFixedHeight(40);

    QPushButton *play_button = new QPushButton("Play", menu_bar);
    play_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    play_button->setFixedHeight(40);

    QPushButton *pause_button = new QPushButton("Pause", menu_bar);
    pause_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    pause_button->setFixedHeight(40);

    QPushButton *stop_button = new QPushButton("Stop", menu_bar);
    stop_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    stop_button->setFixedHeight(40);

    QPushButton *zoom_in_button = new QPushButton("+", menu_bar);
    zoom_in_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    zoom_in_button->setFixedHeight(40);

    QPushButton *actual_size_button = new QPushButton("=", menu_bar);
    actual_size_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    actual_size_button->setFixedHeight(40);

    QPushButton *zoom_out_button = new QPushButton("-", menu_bar);
    zoom_out_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    zoom_out_button->setFixedHeight(40);

    QPushButton *fit_window_button = new QPushButton("#", menu_bar);
    fit_window_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    fit_window_button->setFixedHeight(40);

    qDebug() << "layout buttons";
    QHBoxLayout *h_layout= new QHBoxLayout(menu_bar);
    h_layout->addWidget(connect_button);
    h_layout->addWidget(play_button);
    h_layout->addWidget(pause_button);
    h_layout->addWidget(stop_button);
    h_layout->addWidget(zoom_in_button);
    h_layout->addWidget(actual_size_button);
    h_layout->addWidget(zoom_out_button);
    h_layout->addWidget(fit_window_button);
    menu_bar->setLayout((h_layout));

    // main layout
    qDebug() << "layout main view";
    QVBoxLayout *v_layout = new QVBoxLayout(center_widget);
    v_layout->addWidget(image_view_0);
    v_layout->addWidget(image_view_1);
    v_layout->addWidget(menu_bar);
    center_widget->setLayout(v_layout);

    // signal-slot
    qDebug() << "connect signals";
    QObject::connect(&update_timer, SIGNAL(timeout()), this, SLOT(update_frame()));
    QObject::connect(connect_button, SIGNAL(clicked()), this, SLOT(connect()));
    QObject::connect(play_button, SIGNAL(clicked()), this, SLOT(play()));
    QObject::connect(pause_button, SIGNAL(clicked()), this, SLOT(pause()));
    QObject::connect(stop_button, SIGNAL(clicked()), this, SLOT(stop()));
    QObject::connect(zoom_in_button, SIGNAL(clicked()), this, SLOT(zoom_in()));
    QObject::connect(actual_size_button, SIGNAL(clicked()), this, SLOT(actual_size()));
    QObject::connect(zoom_out_button, SIGNAL(clicked()), this, SLOT(zoom_out()));
    QObject::connect(fit_window_button, SIGNAL(clicked()), this, SLOT(fit_window()));
}

MainWindow::~MainWindow()
{
    stop();
}

// connect to RTSP
void MainWindow::connect()
{
    qDebug() << "connecting";
    std::string pipeline_str = CreateRtspSinkPipeline("rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov");
    qDebug() << pipeline_str.c_str();
    sink_pipeline.reset(new GstAppSinkPipeline());
    sink_pipeline->Initialize(pipeline_str);
    sink_pipeline->SetPipelineState(GST_STATE_PLAYING);
    update_timer.start(1000.0/30.0);
}

void MainWindow::play()
{
    if (sink_pipeline)
        sink_pipeline->SetPipelineState(GST_STATE_PLAYING);
}

void MainWindow::pause()
{
    if (sink_pipeline)
        sink_pipeline->SetPipelineState(GST_STATE_PAUSED);
}

void MainWindow::stop()
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

void MainWindow::zoom_in()
{
    image_view_0->ZoomIn(0.1);
    image_view_1->ZoomIn(0.1);
}

void MainWindow::zoom_out()
{
    image_view_0->ZoomOut(0.1);
    image_view_1->ZoomOut(0.1);
}

void MainWindow::actual_size()
{
    image_view_0->SetScale(1.0, 1.0);
    image_view_1->SetScale(1.0, 1.0);
}

void MainWindow::fit_window()
{
    image_view_0->FitWindow();
    image_view_1->FitWindow();
}

// update frame from sink pipeline to image view
void MainWindow::update_frame()
{
    if (!is_streaming)
    {
        qDebug() << "waiting";
        if (sink_pipeline->GetIsNewFrameAvailable() &&
            sink_pipeline->GetResolution(&image_width, &image_height))
        {
            image_view_0->Reset(image_width, image_height);
            image_view_1->Reset(image_width, image_height);
            image_view_0->FitWindow();
            image_view_1->FitWindow();
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
            image_view_0->UpdateImage(image);
            image_view_1->UpdateImage(image);
            sink_pipeline->ReleaseFrameBuffer();
        }
    }
}
