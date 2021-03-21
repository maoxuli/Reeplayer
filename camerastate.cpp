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
    link_state_label = new QLabel();
    link_state_label->setStyleSheet("background-color: #A9CCE3");
    link_state_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    link_state_label->setFixedSize(40, 40);
    link_state_label->setPixmap(QIcon(":images/gray-light.png").pixmap(25, 25));

    // Recording state
    QLabel *recording_label = new QLabel("Recording");
    recording_label->setStyleSheet("padding-left:3px; background-color: #E8F6F3");
    recording_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    recording_label->setFixedHeight(40);

    recording_state_label = new QLabel();
    recording_state_label->setStyleSheet("background-color: #E8F6F3");
    recording_state_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    recording_state_label->setFixedSize(40, 40);
    recording_state_label->setPixmap(QIcon(":images/gray-light.png").pixmap(25, 25));

    // Uploading state
    QLabel *uploading_label = new QLabel("Uploading");
    uploading_label->setStyleSheet("padding-left:3px; background-color: #FEF5E7");
    uploading_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    uploading_label->setFixedHeight(40);

    uploading_state_label = new QLabel();
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
    battery_state_label->setPixmap(QIcon(":images/gray-light.png").pixmap(25, 25));

    QSpacerItem *spacer = new QSpacerItem(40, 40, QSizePolicy::Preferred, QSizePolicy::Expanding);

    QGridLayout *state_grid = new QGridLayout();
    state_grid->setSpacing(0);
    state_grid->setMargin(0);
    state_grid->addWidget(camera_name_label, 0, 0, 1, 1);
    state_grid->addWidget(link_state_label, 0, 1, 1, 1);
    state_grid->addWidget(recording_label, 1, 0, 1, 1);
    state_grid->addWidget(recording_state_label, 1, 1, 1, 1);
    state_grid->addWidget(uploading_label, 2, 0, 1, 1);
    state_grid->addWidget(uploading_state_label, 2, 1, 1, 1);
    state_grid->addWidget(battery_label, 3, 0, 1, 1);
    state_grid->addWidget(battery_state_label, 3, 1, 1, 1);
    state_grid->addItem(spacer, 4, 0, 1, 2);
    setLayout(state_grid);

    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, &CameraState::updateState);
}

CameraState::~CameraState()
{
    delete update_timer;
}

void CameraState::showEvent(QShowEvent *event)
{
    update_timer->start(1000);
    QWidget::showEvent(event);
}

void CameraState::hideEvent(QHideEvent *event)
{
    update_timer->stop();
    QWidget::hideEvent(event);
}

void CameraState::updateState()
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
            // recording state
            bool link_state = root["link_state"].asBool();
            image = link_state ? ":images/green-light.png" : ":images/gray-light.png";
            link_state_label->setPixmap(QIcon(image.c_str()).pixmap(25, 25));
            bool recording_state = root["recording_state"].asBool();
            image = recording_state ? ":images/red-light.png" : ":images/gray-light.png";
            recording_state_label->setPixmap(QIcon(image.c_str()).pixmap(25, 25));
            bool uploading_state = root["uploading_state"].asBool();
            image = uploading_state ? ":images/green-light.png" : ":images/gray-light.png";
            uploading_state_label->setPixmap(QIcon(image.c_str()).pixmap(25, 25));
            bool battery_state = root["battery_state"].asBool();
            image = battery_state ? ":images/green-light.png" : ":images/yellow-light.png";
            battery_state_label->setPixmap(QIcon(image.c_str()).pixmap(25, 25));
        }
    }
}

