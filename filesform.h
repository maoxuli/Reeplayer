#ifndef FILESFORM_H
#define FILESFORM_H

#include <QWidget>
#include <QVBoxLayout>

#include "camerasmanager.h"
#include "filestate.h"

class FilesForm : public QWidget
{
    Q_OBJECT

public:
    explicit FilesForm(CamerasManager *manager, QWidget *parent = 0);
    ~FilesForm();

public slots:
    void addCamera(int id);
    void removeCamera(int id);

private:
    // cameras
    CamerasManager *cameras;

    // states
    QVBoxLayout *states_layout;
    std::map<int, FileState*> states;
};

#endif // FILESFORM_H
