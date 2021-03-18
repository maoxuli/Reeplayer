#ifndef ADDCAMERAFORM_H
#define ADDCAMERAFORM_H

#include <QWidget>
#include <QPlainTextEdit>

#include "formindex.h"
#include "camerasmanager.h"

class AddCameraForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddCameraForm(CamerasManager *manager, QWidget *parent = 0);
    ~AddCameraForm();

public slots:

signals:
    void showForm(int idx);

private slots:
    void cancelTask();
    void doneTask();

private:
    virtual void showEvent(QShowEvent *);

private:
    // cameras
    CamerasManager *cameras;

    // camera info edit
    QPlainTextEdit *camera_name_edit;
    QPlainTextEdit *camera_ip_edit;
};

#endif // ADDCAMERAFORM_H
