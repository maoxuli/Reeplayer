#include "camerasmanager.h"

CamerasManager::CamerasManager(const std::string& config) :
    last_id(0),
    context_id(0)
{
//    if (!config.empty())
        LoadCameras(config);
}

CamerasManager::~CamerasManager()
{
    ClearCameras();
}

void CamerasManager::ClearCameras()
{
    for (int i = 0; i < cameras.size(); i++) {
        int camera_id = cameras[i]->id();
        delete cameras[i];
        emit cameraRemoved(camera_id);
    }
    cameras.clear();
    last_id = 0;
    context_id = 0;
}

bool CamerasManager::LoadCameras(const std::string &config)
{
    ClearCameras();
    for (int i = 0; i < 3; i++) {
        std::string camera_ip = "192.168.1.169";
        std::string camera_name = "Camera " + std::to_string(last_id+1);
        bool auto_connect = false;
        int camera_id = AddCamera(camera_ip, camera_name);
        if (auto_connect) {
            Camera *camera = GetCamera(camera_id);
            if (camera)
                camera->connectService();
        }
    }
    return true;
}

int CamerasManager::AddCamera(const std::string &ip, const std::string &name)
{
    last_id++;
    cameras.push_back(new Camera(last_id, ip, name));
    emit cameraAdded(last_id);
    return last_id;
}

void CamerasManager::RemoveCamera(int id)
{
    auto it = cameras.begin();
    while(it != cameras.end()) {
        if ((*it)->id() == id) {
            emit cameraRemoved(id);
            delete (*it);
            it = cameras.erase(it);
        }
        else
            ++it;
    }
}

Camera* CamerasManager::GetCamera(int id)
{
    for (int i = 0; i < cameras.size(); i++) {
        if (cameras[i]->id() == id)
            return cameras[i];
    }
}

Camera* CamerasManager::GetFirstCamera()
{
    context_id = 0;
    return cameras.size() > context_id ? cameras[context_id] : NULL;
}

Camera* CamerasManager::GetNextCamera()
{
    context_id++;
    return cameras.size() > context_id ? cameras[context_id] : NULL;
}
