#include "filestate.h"
#include <json/json.h>

#include <cassert>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QIcon>
#include <QTimer>

FileState::FileState(Camera *camera, QWidget *parent) :
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
    camera_state_label = new ClickableLabel();
    camera_state_label->setStyleSheet("background-color: #A9CCE3");
    camera_state_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    camera_state_label->setFixedSize(40, 40);
    camera_state_label->setPixmap(QIcon(":images/gray-light.png").pixmap(25, 25));

    // file list
    QLabel *file_name_label = new QLabel("file.mp4");
    file_name_label->setStyleSheet("padding-left:3px; background-color: #E8F6F3");
    file_name_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    file_name_label->setFixedHeight(40);

    file_state_label = new QLabel();
    file_state_label->setStyleSheet("background-color: #E8F6F3");
    file_state_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    file_state_label->setFixedSize(40, 40);
    file_state_label->setPixmap(QIcon(":images/gray-light.png").pixmap(25, 25));

    QSpacerItem *spacer = new QSpacerItem(40, 40, QSizePolicy::Preferred, QSizePolicy::Expanding);

    QGridLayout *state_grid = new QGridLayout();
    state_grid->setSpacing(0);
    state_grid->setMargin(0);
    state_grid->addWidget(camera_name_label, 0, 0, 1, 1);
    state_grid->addWidget(camera_state_label, 0, 1, 1, 1);
    state_grid->addWidget(file_name_label, 1, 0, 1, 1);
    state_grid->addWidget(file_state_label, 1, 1, 1, 1);
    state_grid->addItem(spacer, 4, 0, 1, 2);
    setLayout(state_grid);

    connect(camera_state_label, &ClickableLabel::clicked, this, &FileState::uploading);

    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, &FileState::updateState);
}

FileState::~FileState()
{
    delete update_timer;
}

void FileState::showEvent(QShowEvent *event)
{
    updateState();
    update_timer->start(1000);
    QWidget::showEvent(event);
}

void FileState::hideEvent(QHideEvent *event)
{
    update_timer->stop();
    QWidget::hideEvent(event);
}

void FileState::uploading()
{
    assert(camera);
    Camera::State state;
    if (camera->checkState(state)) {
        bool uploading_state = state.uploading;
        if (uploading_state)
            camera->stopUploading();
        else
            camera->startUploading();
    }
}

void FileState::updateState()
{
    assert(camera);
    camera_name_label->setText(camera->name().c_str());
    Camera::State state;
    if (camera->checkState(state)) {
        bool uploading_state = state.uploading;
        std::string image = uploading_state ? ":images/green-light.png" : ":images/gray-light.png";
        camera_state_label->setPixmap(QIcon(image.c_str()).pixmap(25, 25));
        // files

    }
    else {
        bool link_state = false;
        std::string image = link_state ? ":images/green-light.png" : ":images/yellow-light.png";
        camera_state_label->setPixmap(QIcon(image.c_str()).pixmap(25, 25));
    }
}
