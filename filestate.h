#ifndef FILESTATE_H
#define FILESTATE_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

#include "clickablelabel.h"
#include "camera.h"

class FileState : public QWidget
{
    Q_OBJECT

public:
    explicit FileState(Camera *camera, QWidget *parent = 0);
    ~FileState();

private slots:
    void uploading();
    void updateState();

private:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private:
    // cameras
    Camera *camera;

    // update state
    QLabel *camera_name_label;
    ClickableLabel *camera_state_label;
    QLabel *file_state_label;
    QTimer *update_timer;
};

#endif // FILESTATE_H
