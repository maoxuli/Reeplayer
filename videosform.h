#ifndef VIDEOSFORM_H
#define VIDEOSFORM_H

#include <QWidget>
#include <QVBoxLayout>

#include "camerasmanager.h"
#include "videoview.h"
#include <map>

class VideosForm : public QWidget
{
    Q_OBJECT

public:
    explicit VideosForm(CamerasManager *manager, QWidget *parent = 0);
    ~VideosForm();

public slots:
    void addCamera(int id);
    void removeCamera(int id);

private slots: 
    void resizeEvent(QResizeEvent *event);

private:
    // cameras
    CamerasManager *cameras;

    // videos
    QVBoxLayout *videos_layout;
    std::map<int, VideoView*> videos;
};

#endif // VIDEOSFORM_H
