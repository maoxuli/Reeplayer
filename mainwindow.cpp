#include "mainwindow.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QWidget>
#include <QScrollArea>
#include <QStackedLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "camerasform.h"
#include "videosform.h"
#include "filesform.h"

#include "addcameraform.h"
#include "removecameraform.h"
#include "zoomvideoform.h"

#define NUM_FORMS 4

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle("Reeplayer");
    setVertical();

    // cameras manager
    cameras = new CamerasManager();
    assert(cameras);

    // Add forms to page layout
    // in the order of FormIndex
    page_layout = new QStackedLayout();

    // 0: cameras form (wrapped in scroll area)
    CamerasForm *cameras_form = new CamerasForm(cameras);
    connect(cameras_form, &CamerasForm::showForm, this, &MainWindow::showForm);
    QScrollArea *cameras_form_area = new QScrollArea();
    cameras_form_area->setFrameShape(QFrame::NoFrame);
    cameras_form_area->setWidgetResizable(true);
    cameras_form_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cameras_form_area->setWidget(cameras_form);
    page_layout->addWidget(cameras_form_area);

    // 1: videos form (wrapped in scroll area)
    VideosForm *videos_form = new VideosForm(cameras);
    connect(videos_form, &VideosForm::showForm, this, &MainWindow::showForm);
    connect(videos_form, &VideosForm::fullScreen, this, &MainWindow::fullScreen);
    QScrollArea *videos_form_area = new QScrollArea();
    videos_form_area->setFrameShape(QFrame::NoFrame);
    videos_form_area->setWidgetResizable(true);
    videos_form_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    videos_form_area->setWidget(videos_form);
    page_layout->addWidget(videos_form_area);

    // 2: files form (wrapped in scroll area)
    FilesForm *files_form = new FilesForm(cameras);
//    connect(files_form, &FilesForm::showForm, this, &MainWindow::showForm);
    QScrollArea *files_form_area = new QScrollArea();
    files_form_area->setFrameShape(QFrame::NoFrame);
    files_form_area->setWidgetResizable(true);
    files_form_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    files_form_area->setWidget(files_form);
    page_layout->addWidget(files_form_area);

    // 3: system form (wrapped in scroll area)
    QWidget *system_form = new QWidget();
//    connect(system_form, &SystemForm::showForm, this, &MainWindow::showForm);
    QScrollArea *system_form_area = new QScrollArea();
    system_form_area->setFrameShape(QFrame::NoFrame);
    system_form_area->setWidgetResizable(true);
    system_form_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    system_form_area->setWidget(system_form);
    page_layout->addWidget(system_form_area);

    // 4: add camera form (warped in scroll area)
    AddCameraForm *add_camera_form = new AddCameraForm(cameras);
    connect(add_camera_form, &AddCameraForm::showForm, this, &MainWindow::showForm);
    QScrollArea *add_camera_form_area = new QScrollArea();
    add_camera_form_area->setFrameShape(QFrame::NoFrame);
    add_camera_form_area->setWidgetResizable(true);
    add_camera_form_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    add_camera_form_area->setWidget(add_camera_form);
    page_layout->addWidget(add_camera_form_area);

    // 5: remove camera form (warped in scroll area)
    RemoveCameraForm *remove_camera_form = new RemoveCameraForm(cameras);
    connect(remove_camera_form, &RemoveCameraForm::showForm, this, &MainWindow::showForm);
    QScrollArea *remove_camera_form_area = new QScrollArea();
    remove_camera_form_area->setFrameShape(QFrame::NoFrame);
    remove_camera_form_area->setWidgetResizable(true);
    remove_camera_form_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    remove_camera_form_area->setWidget(remove_camera_form);
    page_layout->addWidget(remove_camera_form_area);

    // 5: zoom video form
    ZoomVideoForm *zoom_video_form = new ZoomVideoForm(cameras);
    connect(zoom_video_form, &ZoomVideoForm::showForm, this, &MainWindow::showForm);
    connect(zoom_video_form, &ZoomVideoForm::fullScreen, this, &MainWindow::fullScreen);
    page_layout->addWidget(zoom_video_form);

    // buttons in a frame
    QPushButton *cameras_button = new QPushButton("Cameras");
    QPushButton *videos_button = new QPushButton("Videos");
    QPushButton *files_button = new QPushButton("Files");
    QPushButton *system_button = new QPushButton("System");

    connect(cameras_button, &QPushButton::clicked, this, &MainWindow::showCameras);
    connect(videos_button, &QPushButton::clicked, this, &MainWindow::showVideos);
    connect(files_button, &QPushButton::clicked, this, &MainWindow::showFiles);
    connect(system_button, &QPushButton::clicked, this, &MainWindow::showSystem);

    buttons_frame = new QFrame();
    QHBoxLayout *buttons_layout= new QHBoxLayout(buttons_frame);
    buttons_layout->addWidget(cameras_button);
    buttons_layout->addWidget(videos_button);
    buttons_layout->addWidget(files_button);
    buttons_layout->addWidget(system_button);

    buttons_frame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    buttons_frame->setFixedHeight(50);

    cameras_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    cameras_button->setFixedHeight(40);

    videos_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    videos_button->setFixedHeight(40);

    files_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    files_button->setFixedHeight(40);

    system_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    system_button->setFixedHeight(40);

    // main layout
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addLayout(page_layout);
    main_layout->addWidget(buttons_frame);

    // switch to the default form
    showForm(CAMERAS_FORM);
}

MainWindow::~MainWindow()
{

}

// vertical window, size: 9:18
void MainWindow::setVertical()
{
    QRect rc = QDesktopWidget().availableGeometry();
    qDebug() << "screen: " << rc;
    int h = rc.height() - 100;
    int w = h * 9 / 18;
    //qDebug() << "w: " << w;
    QRect window_rc((rc.width() - w)/2, rc.top(), w, h);
    qDebug() << "window: " << window_rc;
    setGeometry(window_rc);
}

// horizontal windos, size: 18:9
void MainWindow::setHorizontal()
{
    QRect rc = QDesktopWidget().availableGeometry();
    qDebug() << "screen: " << rc;
    int w = rc.height() - 100;
    int h = w * 9 / 18;
    //qDebug() << "h: " << h;
    QRect window_rc((rc.width() - w)/2, rc.top(), w, h);
    qDebug() << "window: " << window_rc;
    setGeometry(window_rc);
}

// the normal display is vertical screen
// may rotate to horizontal in full screen
void MainWindow::fullScreen(bool full, bool rotate)
{
    if (full) {
        buttons_frame->hide();
        if (rotate)
            setHorizontal();
    }
    else {
        setVertical();
        buttons_frame->show();
    }
}

void MainWindow::showForm(int idx)
{
    if (idx < 0 && idx >= LAST_FROM)
        qDebug() << "Invalid form index: " << idx;

    //qDebug() << "show form: " << idx;
    page_layout->setCurrentIndex(idx);
}

void MainWindow::showCameras()
{
    showForm(CAMERAS_FORM);
}

void MainWindow::showVideos()
{
    showForm(VIDEOS_FORM);
}

void MainWindow::showFiles()
{
    showForm(FILES_FORM);
}

void MainWindow::showSystem()
{
    showForm(SYSTEM_FORM);
}
