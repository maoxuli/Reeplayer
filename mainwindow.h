#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <memory>
#include "GstAppSinkPipeline.h"
#include "GstreamerPipelines.h"
#include "imageview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void connect();
    void disconnect();
    void update_frame();

private:
    QTimer update_timer;
    ImageView *image_view_0;
    ImageView *image_view_1;
    std::shared_ptr<GstAppSinkPipeline> sink_pipeline;
    bool is_streaming;
    int image_width, image_height;
};

#endif // MAINWINDOW_H
