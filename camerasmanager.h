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

    // Add a new camera, return the ID of the new camera
    // return 0 or negative number for failures
    int AddCamera(const std::string& ip,
                  const std::string& name = "",
                  bool auto_connect = false);

    // Remove a camera with given ID
    void RemoveCamera(int id);

    // Get a camera with given ID
    Camera* GetCamera(int id);

    // Traverse for cameras in order
    Camera* GetFirstCamera();
    Camera* GetNextCamera();

    void SetCurrentCamera(int id) { current_id = id; }
    Camera* GetCurrentCamera();

    int GetNextCameraID() const { return last_id + 1; }

public slots:

signals:
    void cameraAdded(int id);
    void cameraRemoved(int id);

private:
    int last_id;
    int context_id;
    int current_id;
    std::vector<Camera*> cameras;
    void ClearCameras();

    std::string config_file;
    bool LoadCameras();
    bool SaveCameras();
};

#endif // CAMERASMANAGER_H
