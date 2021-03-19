#ifndef ZOOMVIDEOFORM_H
#define ZOOMVIDEOFORM_H

#include <QWidget>
#include <QHBoxLayout>

#include "formindex.h"
#include "camerasmanager.h"
#include "videoview.h"

class ZoomVideoForm : public QWidget
{
    Q_OBJECT

public:
    explicit ZoomVideoForm(CamerasManager *manager, QWidget *parent = 0);
    ~ZoomVideoForm();

public slots:
    void tapVideo(int id);

signals:
    void showForm(int idx);
    void fullScreen(bool full, bool rotate);

private slots: 

private:
    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);

private:
    // cameras
    CamerasManager *cameras;

    // video view
    QHBoxLayout *video_layout;
    VideoView *video_view;
};

#endif // ZOOMVIDEOFORM_H
