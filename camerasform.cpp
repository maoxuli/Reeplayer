#include "camerasform.h"

#include <cassert>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>

CamerasForm::CamerasForm(CamerasManager *manager, QWidget *parent) :
    QWidget(parent),
    cameras(manager)
{
    // layout for camera states
    states_layout = new QVBoxLayout();
    states_layout->setAlignment(Qt::AlignTop);

    Camera *camera = cameras->GetFirstCamera();
    while (camera) {
        CameraState *camera_state = new CameraState(camera);
        states.insert(std::make_pair(camera->id(), camera_state));
        states_layout->addWidget(camera_state);
        camera = cameras->GetNextCamera();
    }

    // buttons in frame
    QPushButton *add_camera_button = new QPushButton("Add camera");
    QPushButton *remove_camera_button = new QPushButton("Remove camera");

    connect(add_camera_button, &QPushButton::clicked, this, &CamerasForm::showAddCamera);
    connect(remove_camera_button, &QPushButton::clicked, this, &CamerasForm::showRemoveCamera);

    //add_camera_button->setStyleSheet("border: none; background-color: #A9CCE3");
    //remove_camera_button->setStyleSheet("border: none; background-color: #A9CCE3");

    QFrame *buttons_frame = new QFrame();
    buttons_frame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    buttons_frame->setFixedHeight(50);

    add_camera_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    add_camera_button->setFixedSize(150, 30);

    remove_camera_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    remove_camera_button->setFixedSize(150, 30);

    QHBoxLayout *buttons_layout = new QHBoxLayout(buttons_frame);
    buttons_layout->addWidget(add_camera_button);
    buttons_layout->addWidget(remove_camera_button);

    // main layout
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->setAlignment(Qt::AlignTop);
    main_layout->addLayout(states_layout);
    main_layout->addWidget(buttons_frame);

    // attach to cameras manager
    connect(cameras, &CamerasManager::cameraAdded, this, &CamerasForm::addCamera);
    connect(cameras, &CamerasManager::cameraRemoved, this, &CamerasForm::removeCamera);
}

CamerasForm::~CamerasForm()
{

}

void CamerasForm::addCamera(int id)
{
    if (states.find(id) != states.end()) {
        qDebug() << "Camera exists: " << id;
        return;
    }
    assert(cameras);
    Camera *camera = cameras->GetCamera(id);
    if (camera) {
        CameraState *camera_state = new CameraState(camera);
        states.insert(std::make_pair(camera->id(), camera_state));
        states_layout->addWidget(camera_state);
    }
}

void CamerasForm::removeCamera(int id)
{
    auto it = states.find(id);
    if (it != states.end()) {
        CameraState *camera_state = it->second;
        states_layout->removeWidget(camera_state);
        states.erase(it);
        delete camera_state;
    }
}

void CamerasForm::showAddCamera()
{
    emit showForm(ADD_CAMERA_FORM);
}

void CamerasForm::showRemoveCamera()
{
    emit showForm(REMOVE_CAMERA_FORM);
}
