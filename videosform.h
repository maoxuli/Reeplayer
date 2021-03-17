#ifndef VIDEOSFORM_H
#define VIDEOSFORM_H

#include <QWidget>

#include "videoview.h"
#include <vector>

class VideosForm : public QWidget
{
    Q_OBJECT

public:
    explicit VideosForm(QWidget *parent = 0);
    ~VideosForm();

private slots: 
    void resizeEvent(QResizeEvent *event);

private:
    // gstreamer pipeline
    std::vector<VideoView*> videos;
};

#endif // VIDEOSFORM_H
