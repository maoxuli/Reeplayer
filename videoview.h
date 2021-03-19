#ifndef VIDEOVIEW_H
#define VIDEOVIEW_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

#include "imageview.h"
#include "camera.h"

#include <memory>
#include "GstAppSinkPipeline.h"
#include "GstreamerPipelines.h"

class VideoView : public ImageView
{
    Q_OBJECT

public:
    explicit VideoView(Camera *camera = NULL, QWidget *parent = 0);
    ~VideoView();

    void attachCamera(Camera *camera);
    void enableControl(bool enable = true);

public slots:
    void play();
    void pause();
    void stop();

signals:
    void tapVideo(int id);

private slots:
    void updateFrame();
    void updateState();

private:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual void mousePressEvent(QMouseEvent *event);

private:
    // camera
    Camera *camera;

    // controls
    QFrame *header_frame;
    QLabel *camera_name_label;
    QLabel *camera_state_label;

    QFrame *buttons_frame;
    QPushButton *play_button;
    QPushButton *pause_button;
    QPushButton *stop_button;
    QPushButton *zoom_in_button;
    QPushButton *zoom_out_button;
    QPushButton *actual_size_button;
    QPushButton *fit_window_button;

    // update timer
    QTimer *video_timer;
    QTimer *state_timer;

    // GStreamer pipeline
    int video_width, video_height;
    std::shared_ptr<GstAppSinkPipeline> pipeline;
};

#endif // VIDEOVIEW_H
