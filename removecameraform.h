#ifndef REMOVECAMERAFORM_H
#define REMOVECAMERAFORM_H

#include <QWidget>
#include <QListWidget>

#include "formindex.h"
#include "camerasmanager.h"

class RemoveCameraForm : public QWidget
{
    Q_OBJECT

public:
    explicit RemoveCameraForm(CamerasManager *manager, QWidget *parent = 0);
    ~RemoveCameraForm();

public slots:

signals:
    void showForm(int idx);

private slots:
    void checkItemState(QListWidgetItem *item);
    void checkItemSelection(QListWidgetItem *item);
    void cancelTask();
    void doneTask();

private:
    virtual void showEvent(QShowEvent *);

private:
    // cameras
    CamerasManager *cameras;

    // cameras list
    QListWidget *camera_list;
};

#endif // REMOVECAMERAFORM_H
