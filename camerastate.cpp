#include "camerastate.h"
#include <json/json.h>

#include <cassert>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QIcon>
#include <QTimer>

CameraState::CameraState(Camera *camera, QWidget *parent) :
    QWidget(parent)
{
    assert(camera);
    this->camera = camera;

    // fixed the height
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setFixedHeight(200);

    // camera_name
    camera_name_label = new QLabel(camera->name().c_str());
    camera_name_label->setStyleSheet("padding-left:3px; background-color:#A9CCE3");
    camera_name_label->setFont(QFont("Arial", 13, QFont::Bold));
    camera_name_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    camera_name_label->setFixedHeight(40);

    // link_state
    link_state_label = new ClickableLabel();
    link_state_label->setStyleSheet("background-color: #A9CCE3");
    link_state_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    link_state_label->setFixedSize(40, 40);
    link_state_label->setPixmap(QIcon(":images/gray-light.png").pixmap(25, 25));

    // Streaming state
    QLabel *streaming_label = new QLabel("Streaming");
    streaming_label->setStyleSheet("padding-left:3px; background-color: #E8F6F3");
    streaming_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    streaming_label->setFixedHeight(40);

    streaming_state_label = new ClickableLabel();
    streaming_state_label->setStyleSheet("background-color: #E8F6F3");
    streaming_state_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    streaming_state_label->setFixedSize(40, 40);
    streaming_state_label->setPixmap(QIcon(":images/gray-light.png").pixmap(25, 25));

    // Recording state
    QLabel *recording_label = new QLabel("Recording");
    recording_label->setStyleSheet("padding-left:3px; background-color: #E8F6F3");
    recording_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    recording_label->setFixedHeight(40);

    recording_state_label = new ClickableLabel();
    recording_state_label->setStyleSheet("background-color: #E8F6F3");
    recording_state_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    recording_state_label->setFixedSize(40, 40);
    recording_state_label->setPixmap(QIcon(":images/gray-light.png").pixmap(25, 25));

    // Uploading state
    QLabel *uploading_label = new QLabel("Uploading");
    uploading_label->setStyleSheet("padding-left:3px; background-color: #FEF5E7");
    uploading_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    uploading_label->setFixedHeight(40);

    uploading_state_label = new ClickableLabel();
    uploading_state_label->setStyleSheet("background-color: #FEF5E7");
    uploading_state_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    uploading_state_label->setFixedSize(40, 40);
    uploading_state_label->setPixmap(QIcon(":images/gray-light.png").pixmap(25, 25));

    // Battery state
    QLabel *battery_label = new QLabel("Battery");
    battery_label->setStyleSheet("padding-left:3px; background-color: #F5EEF8");
    battery_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    battery_label->setFixedHeight(40);

    battery_state_label = new QLabel();
    battery_state_label->setStyleSheet("background-color: #F5EEF8");
    battery_state_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    battery_state_label->setFixedSize(40, 40);
    battery_state_label->setText("0.0%");
    //battery_state_label->setPixmap(QIcon(":images/gray-light.png").pixmap(25, 25));

    QSpacerItem *spacer = new QSpacerItem(40, 40, QSizePolicy::Preferred, QSizePolicy::Expanding);

    QGridLayout *state_grid = new QGridLayout();
    state_grid->setSpacing(0);
    state_grid->setMargin(0);
    state_grid->addWidget(camera_name_label, 0, 0, 1, 1);
    state_grid->addWidget(link_state_label, 0, 1, 1, 1);
    state_grid->addWidget(streaming_label, 1, 0, 1, 1);
    state_grid->addWidget(streaming_state_label, 1, 1, 1, 1);
    state_grid->addWidget(recording_label, 2, 0, 1, 1);
    state_grid->addWidget(recording_state_label, 2, 1, 1, 1);
    state_grid->addWidget(uploading_label, 3, 0, 1, 1);
    state_grid->addWidget(uploading_state_label, 3, 1, 1, 1);
    state_grid->addWidget(battery_label, 4, 0, 1, 1);
    state_grid->addWidget(battery_state_label, 4, 1, 1, 1);
    state_grid->addItem(spacer, 5, 0, 1, 2);
    setLayout(state_grid);

    connect(link_state_label, &ClickableLabel::clicked, this, &CameraState::connection);
    connect(streaming_state_label, &ClickableLabel::clicked, this, &CameraState::streaming);
    connect(recording_state_label, &ClickableLabel::clicked, this, &CameraState::recording);
    connect(uploading_state_label, &ClickableLabel::clicked, this, &CameraState::uploading);

    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, &CameraState::updateState);
}

CameraState::~CameraState()
{
    delete update_timer;
}

void CameraState::showEvent(QShowEvent *event)
{
    //updateState();
    update_timer->start(1000);
    QWidget::showEvent(event);
}

void CameraState::hideEvent(QHideEvent *event)
{
    update_timer->stop();
    QWidget::hideEvent(event);
}

void CameraState::connection()
{
    assert(camera);
    if (camera->connected())
        camera->disconnect();
    else
        camera->connect();
}

void CameraState::streaming()
{
    assert(camera);
    Camera::LiveState state;
    if (camera->checkLiveState(state)) {
        bool streaming_state = state.streaming;
        if (streaming_state)
            camera->stopStreaming();
        else
            camera->startStreaming();
    }
}

void CameraState::recording()
{
    assert(camera);
    Camera::LiveState state;
    if (camera->checkLiveState(state)) {
        bool recording_state = state.recording;
        if (recording_state)
            camera->stopRecording();
        else
            camera->startRecording();
    }
}

void CameraState::uploading()
{
    assert(camera);
    Camera::UploadState state;
    if (camera->checkUploadState(state)) {
        bool uploading_state = state.uploading;
        if (uploading_state)
            camera->stopUploading();
        else
            camera->startUploading();
    }
}

void CameraState::updateState()
{
    assert(camera);
    camera_name_label->setText(camera->name().c_str());
    std::string link_image, streaming_image, recording_image, uploading_image;
    float battery_percent = 0;
    Camera::SystemState system_state;
    Camera::LiveState live_state;
    Camera::UploadState upload_state;
    if (!camera->connected()) { // disconnected
        link_image = ":images/gray-light.png";
        streaming_image = ":images/gray-light.png";
        recording_image = ":images/gray-light.png";
        uploading_image = ":images/gray-light.png";
        battery_percent = 0;
    }
    else if (camera->checkSystemState(system_state) &&
             camera->checkLiveState(live_state) &&
             camera->checkUploadState(upload_state)) { // connected
        link_image = ":images/green-light.png";
        bool streaming_state = live_state.streaming;
        streaming_image = streaming_state ? ":images/green-light.png" : ":images/gray-light.png";
        bool recording_state = live_state.recording;
        recording_image = recording_state ? ":images/red-light.png" : ":images/gray-light.png";
        bool uploading_state = upload_state.uploading;
        uploading_image = uploading_state ? ":images/green-light.png" : ":images/gray-light.png";
        battery_percent = system_state.battery;
    }
    else { // failure
        link_image = ":images/yellow-light.png";
        streaming_image = ":images/yellow-light.png";
        recording_image = ":images/yellow-light.png";
        uploading_image = ":images/yellow-light.png";
        battery_percent = 0;
    }

    link_state_label->setPixmap(QIcon(link_image.c_str()).pixmap(25, 25));
    streaming_state_label->setPixmap(QIcon(streaming_image.c_str()).pixmap(25, 25));
    recording_state_label->setPixmap(QIcon(recording_image.c_str()).pixmap(25, 25));
    uploading_state_label->setPixmap(QIcon(uploading_image.c_str()).pixmap(25, 25));
    char percent_text[16];
    sprintf(percent_text, "%.1f%%", battery_percent);
    battery_state_label->setText(percent_text);
}
