#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "GstAppSinkPipeline.h"
#include "GstreamerPipelines.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void connect();
    void update_frame();

private:
    GstAppSinkPipeline sink_pipeline;
};

#endif // MAINWINDOW_H
