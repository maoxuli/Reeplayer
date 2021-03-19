#include "videosform.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QResizeEvent>

VideosForm::VideosForm(CamerasManager *manager, QWidget *parent) :
    QWidget(parent),
    cameras(manager)
{
    // layout for camera list
    videos_layout = new QVBoxLayout(this);
    videos_layout->setAlignment(Qt::AlignTop);

    int width = size().width();
    int height = width * 1080 / 1920;
    Camera *camera = cameras->GetFirstCamera();
    while (camera) {
        VideoView *video_view = new VideoView(camera);
        video_view->enableControl(false);
        //video_view->setStyleSheet("background-color: black");
        video_view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        video_view->setFixedHeight(height);
        videos.insert(std::make_pair(camera->id(), video_view));
        videos_layout->addWidget(video_view);
        connect(video_view, &VideoView::tapVideo, this, &VideosForm::tapVideo);
        camera = cameras->GetNextCamera();
    }

    // attach to cameras manager
    connect(cameras, &CamerasManager::cameraAdded, this, &VideosForm::addCamera);
    connect(cameras, &CamerasManager::cameraRemoved, this, &VideosForm::removeCamera);
}

VideosForm::~VideosForm()
{

}

void VideosForm::showEvent(QShowEvent *event)
{
    for (auto it = videos.begin(); it != videos.end(); ++it) {
        VideoView *video_view = it->second;
        video_view->play();
    }

    QWidget::showEvent(event);
}

void VideosForm::hideEvent(QHideEvent *event)
{
    // VideoVideo has done the stop in hideEvent
    QWidget::hideEvent(event);
}

void VideosForm::resizeEvent(QResizeEvent *event)
{
    int width = event->size().width();
    int height = width * 1080 / 1920;

    for (auto it = videos.begin(); it != videos.end(); ++it) {
        VideoView *video_view = it->second;
        video_view->setFixedHeight(height);
    }

    QWidget::resizeEvent(event);
}

void VideosForm::addCamera(int id)
{
    if (videos.find(id) != videos.end()) {
        qDebug() << "Camera exists: " << id;
        return;
    }
    assert(cameras);
    Camera *camera = cameras->GetCamera(id);
    if (camera) {
        int width = size().width();
        int height = width * 1080 / 1920;
        VideoView *video_view = new VideoView(camera);
        video_view->enableControl(false);
        //video_view->setStyleSheet("background-color: black");
        video_view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        video_view->setFixedHeight(height);
        videos.insert(std::make_pair(id, video_view));
        videos_layout->addWidget(video_view);
        connect(video_view, &VideoView::tapVideo, this, &VideosForm::tapVideo);
    }
}

void VideosForm::removeCamera(int id)
{
    auto it = videos.find(id);
    if (it != videos.end()) {
        VideoView *video_view = it->second;
        disconnect(video_view, &VideoView::tapVideo, this, &VideosForm::tapVideo);
        videos_layout->removeWidget(video_view);
        videos.erase(it);
        delete video_view;
    }
}

// single video view for camera with given ID
void VideosForm::tapVideo(int id)
{
    // set current camera in cameras manager
    cameras->SetCurrentCamera(id);
    emit showForm(ZOOM_VIDEO_FORM);
    emit fullScreen(true, true);
}
