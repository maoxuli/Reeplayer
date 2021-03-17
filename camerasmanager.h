#ifndef CAMERASMANAGER_H
#define CAMERASMANAGER_H

#include <QObject>
#include "camera.h"

class CamerasManager : public QObject
{
    Q_OBJECT

public:
    explicit CamerasManager(const std::string& config = "");
    ~CamerasManager();

    // Load cameras from config file
    bool LoadCameras(const std::string& config);

    // Add a new camera, return the ID of the new camera
    // return 0 or negative number for failures
    int AddCamera(const std::string& ip, const std::string& name = "");

    // Remove a camera with given ID
    void RemoveCamera(int id);

    // Get a camera with given ID
    Camera* GetCamera(int id);

    // Traverse for cameras in order
    Camera* GetFirstCamera();
    Camera* GetNextCamera();

public slots:

signals:
    void cameraAdded(int id);
    void cameraRemoved(int id);

private:
    int last_id;
    int context_id;
    std::vector<Camera*> cameras;

    void ClearCameras();
};

#endif // CAMERASMANAGER_H
