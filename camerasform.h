#ifndef CAMERASFORM_H
#define CAMERASFORM_H

#include <QWidget>
#include <QVBoxLayout>

#include "camerasmanager.h"
#include "camerastate.h"

class CamerasForm : public QWidget
{
    Q_OBJECT

public:
    explicit CamerasForm(CamerasManager *manager, QWidget *parent = 0);
    ~CamerasForm();

private:
    // cameras
    CamerasManager *cameras;

    // states
    QVBoxLayout *states_layout;
    std::map<int, CameraState*> states;
};

#endif // CAMERASFORM_H
