#ifndef CAMERASTATE_H
#define CAMERASTATE_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

#include "clickablelabel.h"
#include "camera.h"

class CameraState : public QWidget
{
    Q_OBJECT

public:
    explicit CameraState(Camera *camera, QWidget *parent = 0);
    ~CameraState();

private slots:
    void streaming();
    void recording();
    void uploading();
    void updateState();

private:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private:
    // cameras
    Camera *camera;

    // update state
    QTimer *update_timer;
    QLabel *camera_name_label;
    QLabel *link_state_label;
    ClickableLabel *streaming_state_label;
    ClickableLabel *recording_state_label;
    ClickableLabel *uploading_state_label;
    QLabel *battery_state_label;
};

#endif // CAMERASTATE_H
