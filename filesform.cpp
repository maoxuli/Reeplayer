#include "filesform.h"

#include <cassert>
#include <QDebug>
#include <QVBoxLayout>

FilesForm::FilesForm(CamerasManager *manager, QWidget *parent) :
    QWidget(parent),
    cameras(manager)
{
    // layout for file states
    states_layout = new QVBoxLayout(this);
    states_layout->setAlignment(Qt::AlignTop);

    Camera *camera = cameras->GetFirstCamera();
    while (camera) {
        FileState *file_state = new FileState(camera);
        states.insert(std::make_pair(camera->id(), file_state));
        states_layout->addWidget(file_state);
        camera = cameras->GetNextCamera();
    }

    // attach to cameras manager
    connect(cameras, &CamerasManager::cameraAdded, this, &FilesForm::addCamera);
    connect(cameras, &CamerasManager::cameraRemoved, this, &FilesForm::removeCamera);
}

FilesForm::~FilesForm()
{

}

void FilesForm::addCamera(int id)
{
    if (states.find(id) != states.end()) {
        qDebug() << "Camera exists: " << id;
        return;
    }
    assert(cameras);
    Camera *camera = cameras->GetCamera(id);
    if (camera) {
        FileState *file_state = new FileState(camera);
        states.insert(std::make_pair(camera->id(), file_state));
        states_layout->addWidget(file_state);
    }
}

void FilesForm::removeCamera(int id)
{
    auto it = states.find(id);
    if (it != states.end()) {
        FileState *file_state = it->second;
        states_layout->removeWidget(file_state);
        states.erase(it);
        delete file_state;
    }
}
