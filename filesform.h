#ifndef FILESFORM_H
#define FILESFORM_H

#include <QWidget>
#include "camerasmanager.h"

class FilesForm : public QWidget
{
    Q_OBJECT

public:
    explicit FilesForm(CamerasManager *manager, QWidget *parent = 0);
    ~FilesForm();

private:
    // cameras
    CamerasManager *cameras;
};

#endif // FILESFORM_H
