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
    QMainWindow(parent)
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
    ImageView *image_view_0 = new ImageView(center_widget);
    ImageView *image_view_1 = new ImageView(center_widget);

    // buttons
    QFrame *menu_bar = new QFrame(center_widget);
    menu_bar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    menu_bar->setFixedHeight(50);

    QPushButton *connect_button = new QPushButton("Connect", menu_bar);
    connect_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    connect_button->setFixedHeight(40);

    QPushButton *play_button = new QPushButton("Play", menu_bar);
    play_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    play_button->setFixedHeight(40);

    QPushButton *stop_button = new QPushButton("Stop", menu_bar);
    stop_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    stop_button->setFixedHeight(40);

    QHBoxLayout *h_layout= new QHBoxLayout(menu_bar);
    h_layout->addWidget(connect_button);
    h_layout->addWidget(play_button);
    h_layout->addWidget(stop_button);
    menu_bar->setLayout((h_layout));

    // main layout
    QVBoxLayout *v_layout = new QVBoxLayout(center_widget);
    v_layout->addWidget(image_view_0);
    v_layout->addWidget(image_view_1);
    v_layout->addWidget(menu_bar);
    this->setLayout(v_layout);

    // signal-slot
    connect(connect_button, SIGNAL(clicked()), this, SLOT(connect()));
    connect(&sink_pipeline, SIGNAL(received()), this, SLOT(update_frame()));
}

MainWindow::~MainWindow()
{

}

// connect to RTSP
void MainWindow::connect()
{
    std::string pipeline_str = CreateRtspSinkPipeline("rtsp://192.168.1.188:8554/ds-stream");
    qDebug() << pipeline_str;
    sink_pipeline.reset(new GstAppSinkPipeline());
    sink_pipeline->Initialize(pipeline_str);
    sink_pipeline->SetPipelineState(GST_STATE_PLAYING);
}

// update frame from sink pipeline to image view
void MainWindow::update_frame()
{
    if (sink_pipeline.CheckNewFrames() &&
        sink_pipeline.GetFrames(buffer))
    {
        //QImage
        sink_pipeline.ReleaseFrameBuffer();
    }
}
