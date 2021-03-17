#ifndef CAMERASFORM_H
#define CAMERASFORM_H

#include <QWidget>
#include "camerasmanager.h"


class CamerasForm : public QWidget
{
    Q_OBJECT

public:
    explicit CamerasForm(CamerasManager *manager, QWidget *parent = 0);
    ~CamerasForm();

private:
    // cameras
    CamerasManager *cameras;
};

#endif // CAMERASFORM_H
