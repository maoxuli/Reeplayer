#ifndef VIDEOVIEW_H
#define VIDEOVIEW_H

#include "imageview.h"

#include <memory>
#include "GstAppSinkPipeline.h"
#include "GstreamerPipelines.h"

#include <QTimer>
#include <QPushButton>

class VideoView : public ImageView
{
    Q_OBJECT

public:
    explicit VideoView(QWidget *parent = 0);
    ~VideoView();

private slots:
    void connect();
    void play();
    void pause();
    void stop();
    void zoom_in();
    void zoom_out();
    void actual_size();
    void fit_window();
    void update_frame();

private:
    void layout_v();
    void layout_h();

private:
    // gstreamer sink is the video source
    std::shared_ptr<GstAppSinkPipeline> sink_pipeline;
    int image_width, image_height;
    bool is_streaming;

    QTimer update_timer;
    QPushButton *connect_button;
    QPushButton *play_button;
    QPushButton *pause_button;
    QPushButton *stop_button;
    QPushButton *zoom_in_button;
    QPushButton *zoom_out_button;
    QPushButton *actual_size_button;
    QPushButton *fit_window_button;
};

#endif // VIDEOVIEW_H
