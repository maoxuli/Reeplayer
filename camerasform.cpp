#include "camerasform.h"

#include <QVBoxLayout>

CamerasForm::CamerasForm(CamerasManager *manager, QWidget *parent) :
    QWidget(parent),
    cameras(manager)
{
    states_layout = new QVBoxLayout();
    states_layout->setAlignment(Qt::AlignTop);
    setLayout(states_layout);

    // attach to cameras manager
    // add or remove camera on signal
    //connect(cameras, &CamerasManager::cameraAdded, this, &VideosForm::addCamera);
    //connect(cameras, &CamerasManager::cameraRemoved, this, &VideosForm::removeCamera);

    // initial list of cameras
    Camera *camera = cameras->GetFirstCamera();
    while (camera) {
        CameraState *camera_state = new CameraState(camera);
        states_layout->addWidget(camera_state);
        states.insert(std::make_pair(camera->id(), camera_state));
        camera = cameras->GetNextCamera();
    }
}

CamerasForm::~CamerasForm()
{

}
