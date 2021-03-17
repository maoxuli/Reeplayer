#ifndef VIDEOVIEW_H
#define VIDEOVIEW_H

#include "imageview.h"
#include "videostream.h"

class VideoView : public ImageView
{
    Q_OBJECT

public:
    explicit VideoView(VideoStream *stream, QWidget *parent = 0);
    ~VideoView();

public slots:
    void playVideo();
    void pauseVideo();
    void stopVideo();
    void updateFrame();

private slots:

private:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private:
    VideoStream *video_stream;
    int video_width, video_height;
};

#endif // VIDEOVIEW_H
