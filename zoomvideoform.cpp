#include "zoomvideoform.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QResizeEvent>

ZoomVideoForm::ZoomVideoForm(CamerasManager *manager, QWidget *parent) :
    QWidget(parent),
    cameras(manager)
{
    video_layout = new QHBoxLayout(this);
    video_view = new VideoView();
    //video_view->setStyleSheet("background-color: black");
    video_view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    video_layout->addWidget(video_view);
    connect(video_view, &VideoView::tapVideo, this, &ZoomVideoForm::tapVideo);
}

ZoomVideoForm::~ZoomVideoForm()
{

}

void ZoomVideoForm::showEvent(QShowEvent *event)
{
    assert(cameras);
    Camera *camera = cameras->GetCurrentCamera();
    if (camera) {
        assert(video_view);
        video_view->attachCamera(camera);
        video_view->play();
    }

    QWidget::showEvent(event);
}

void ZoomVideoForm::hideEvent(QHideEvent *event)
{
    // VideoVideo has done the stop in hideEvent
    QWidget::hideEvent(event);
}

void ZoomVideoForm::tapVideo(int id)
{
    cameras->SetCurrentCamera(0);
    emit showForm(VIDEOS_FORM);
    emit fullScreen(false, false);
}
