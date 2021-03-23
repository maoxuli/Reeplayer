#include "camerasmanager.h"
#include <json/json.h>
#include <iostream>
#include <fstream>

#include <QDebug>

#if defined(__APPLE__) && defined(__MACH__)
#include "CoreFoundation/CoreFoundation.h"
std::string GetBundlePath()
{
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyBundleURL(mainBundle);
    CFStringRef cfStr = CFURLCopyFileSystemPath(resourcesURL, kCFURLPOSIXPathStyle);
    CFRelease(resourcesURL);
    char bundlePathStr[PATH_MAX];
    CFStringGetCString(cfStr, bundlePathStr, FILENAME_MAX, kCFStringEncodingASCII);
    CFRelease(cfStr);
    return std::string(bundlePathStr);
}
#endif

CamerasManager::CamerasManager(const std::string& config) :
    last_id(0),
    context_id(0),
    current_id(0),
    config_file(config)
{
    if (config_file.empty()) {
#if defined(__APPLE__) && defined(__MACH__)
        std::string bundle_path = GetBundlePath();
        config_file = bundle_path.substr(0, bundle_path.rfind("/"));
        config_file += "/Reeplayer.json";
#else
        config_file = "Reeplayer.json";
#endif
    }
    qDebug() << "Config file: " << config_file.c_str();
    LoadCameras();
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

bool CamerasManager::SaveCameras()
{
    Json::Value camera_array(Json::arrayValue);
    for (int i = 0; i < cameras.size(); i++) {
        Camera *camera = cameras[i];
        Json::Value camera_info;
        camera_info["camera_name"] = camera->name();
        camera_info["camera_ip"] = camera->ip();
        camera_info["auto_connect"] = camera->is_auto();
        camera_array.append(camera_info);
    }
    Json::Value root;
    root["cameras"] = camera_array;

    std::ofstream f(config_file);
    if (!f.is_open()) {
        std::cerr << "Failed open file for write: " << config_file << std::endl;
        return false;
    }
    f << root << std::endl;
    return true;
}

bool CamerasManager::LoadCameras()
{
    //ClearCameras();
    std::ifstream f(config_file);
    if (!f.is_open()) {
        std::cerr << "Failed open file for read: " << config_file << std::endl;
        return false;
    }
    Json::Value root, camera_array;
    f >> root;
    camera_array = root["cameras"];
    for (auto it = camera_array.begin(); it != camera_array.end(); ++it) {
        Json::Value camera_info = *it;
        std::string camera_name = camera_info["camera_name"].asString();
        std::string camera_ip = camera_info["camera_ip"].asString();
        bool auto_connect = camera_info["auto_connect"].asBool();
        int camera_id = AddCamera(camera_ip, camera_name, auto_connect);
    }
    return true;
}

int CamerasManager::AddCamera(const std::string &ip,
                              const std::string &name, bool auto_connect)
{
    last_id++;
    cameras.push_back(new Camera(last_id, ip, name, auto_connect));
    emit cameraAdded(last_id);
    SaveCameras();
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
            SaveCameras();
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
    return NULL;
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

Camera* CamerasManager::GetCurrentCamera()
{
    return GetCamera(current_id);
}
