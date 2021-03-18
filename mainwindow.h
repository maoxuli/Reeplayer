#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QFrame>

#include "camerasmanager.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // form index in page layout
    enum FormIndex
    {
        CAMERAS_FORM,
        VIDEOS_FORM,
        FILES_FORM,
        SYSTEM_FORM,
        LAST_FROM
    };

public slots:
    // the normal display is vertical screen
    // may rotate to horizontal in full screen
    void fullScreen(bool full = true, bool rotate = false);

    // switch display
    void showForm(int idx);

private slots:
    // buttons
    void showCameras();
    void showVideos();
    void showFiles();
    void showSystem();

private:
    // main window size
    void setVertical();
    void setHorizontal();

private:
    // layout
    QVBoxLayout *main_layout;
    QStackedLayout *page_layout;
    QFrame *buttons_frame;

    // cameras
    CamerasManager *cameras;
};

#endif // MAINWINDOW_H
