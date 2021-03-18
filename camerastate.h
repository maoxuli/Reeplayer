#ifndef CAMERASTATE_H
#define CAMERASTATE_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

#include "camera.h"

class CameraState : public QWidget
{
    Q_OBJECT

public:
    explicit CameraState(Camera *camera, QWidget *parent = 0);
    ~CameraState();

private slots:
    void updateState();

private:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private:
    // cameras
    Camera *camera;

    // update state
    QLabel *camera_name_label;
    QLabel *link_state_label;
    QLabel *recording_state_label;
    QLabel *uploading_state_label;
    QLabel *battery_state_label;
    QTimer *update_timer;
};

#endif // CAMERASTATE_H
