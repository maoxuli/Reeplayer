#ifndef CAMERASFORM_H
#define CAMERASFORM_H

#include <QWidget>
#include <QVBoxLayout>

#include "formindex.h"
#include "camerasmanager.h"
#include "camerastate.h"

class CamerasForm : public QWidget
{
    Q_OBJECT

public:
    explicit CamerasForm(CamerasManager *manager, QWidget *parent = 0);
    ~CamerasForm();

public slots:
    void addCamera(int id);
    void removeCamera(int id);

signals:
    void showForm(int idx);

private slots:
    void showAddCamera();
    void showRemoveCamera();

private:
    // cameras
    CamerasManager *cameras;

    // states
    QVBoxLayout *states_layout;
    std::map<int, CameraState*> states;
};

#endif // CAMERASFORM_H
