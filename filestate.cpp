#include "filestate.h"
#include <json/json.h>

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
    camera_name_label->setFont(QFont( "Arial", 13, QFont::Bold));
    camera_name_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    camera_name_label->setFixedHeight(40);

    // link_state
    camera_state_label = new QLabel();
    camera_state_label->setStyleSheet("background-color: #A9CCE3");
    camera_state_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    camera_state_label->setFixedSize(40, 40);
    camera_state_label->setPixmap(QIcon(":images/gray-light.png").pixmap(25, 25));

    // file list
    QLabel *file_name_label = new QLabel("file.mp4");
    file_name_label->setStyleSheet("padding-left:3px; background-color: #FEF5E7");
    file_name_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    file_name_label->setFixedHeight(40);

    file_state_label = new QLabel();
    file_state_label->setStyleSheet("background-color: #FEF5E7");
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

    update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, &FileState::updateState);
}

FileState::~FileState()
{
    delete update_timer;
}

void FileState::showEvent(QShowEvent *event)
{
    update_timer->start(1000);
    QWidget::showEvent(event);
}

void FileState::hideEvent(QHideEvent *event)
{
    update_timer->stop();
    QWidget::hideEvent(event);
}

void FileState::updateState()
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
            // link and uploading state
            bool link_state = root["link_state"].asBool();
            bool uploading_state = root["uploading_state"].asBool();
            if (!link_state)
                image = ":images/yellow-light.png";
            else if (uploading_state)
                image = ":images/green-light.png";
            else
                image = ":images/gray-light.png";
            camera_state_label->setPixmap(QIcon(image.c_str()).pixmap(25, 25));
        }
    }
}
