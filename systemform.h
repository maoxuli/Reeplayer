#ifndef SYSTEMFORM_H
#define SYSTEMFORM_H

#include <QWidget>
#include <QComboBox>
#include <QButtonGroup>

#include "camerasmanager.h"

class SystemForm : public QWidget
{
    Q_OBJECT

public:
    explicit SystemForm(CamerasManager *manager, QWidget *parent = 0);
    ~SystemForm();

public slots:
    void changeCamera(int idx);
    void applyMode();
    void applyPath();
    void showAddPath();

signals:
    void showForm(int idx);

private slots:
    void updateState();

private:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private:
    // cameras
    CamerasManager *cameras;

    // cameras list
    QComboBox *cameras_box;

    // camera working mode
    QButtonGroup *mode_group;
};

#endif // SYSTEMFORM_H
