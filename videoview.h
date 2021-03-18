#ifndef VIDEOVIEW_H
#define VIDEOVIEW_H

#include <QLabel>
#include <QTimer>

#include "imageview.h"
#include "videostream.h"
#include "camera.h"

class VideoView : public ImageView
{
    Q_OBJECT

public:
    explicit VideoView(Camera *camera, QWidget *parent = 0);
    ~VideoView();

public slots:
    void playVideo();
    void pauseVideo();
    void stopVideo();

private slots:

private:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private slots:
    void updateFrame();
    void updateState();

private:
    // camera
    Camera *camera;

    // video stream
    VideoStream *video_stream;
    int video_width, video_height;

    // update timer
    QLabel *camera_name_label;
    QLabel *camera_state_label;

    QTimer *video_timer;
    QTimer *state_timer;
};

#endif // VIDEOVIEW_H
