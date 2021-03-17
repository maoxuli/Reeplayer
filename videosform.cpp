#include "videosform.h"

#include <QVBoxLayout>
#include <QResizeEvent>

VideosForm::VideosForm(CamerasManager *manager, QWidget *parent) :
    QWidget(parent),
    cameras(manager)
{
    videos_layout = new QVBoxLayout();
    videos_layout->setAlignment(Qt::AlignTop);
    setLayout(videos_layout);

    // attach to cameras manager
    // add or remove camera on signal
//    connect(cameras, &CamerasManager::cameraAdded, this, &VideosForm::addCamera);
//    connect(cameras, &CamerasManager::cameraRemoved, this, &VideosForm::removeCamera);

    // initial list of cameras
    int width = size().width();
    int height = width * 1080 / 1920;
    Camera *camera = this->cameras->GetFirstCamera();
    while (camera) {
        VideoView * video_view = new VideoView(camera->GetVideoStream());
        //video_view->setStyleSheet("background-color: black");
        video_view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        video_view->setFixedHeight(height);
        videos_layout->addWidget(video_view);
        videos.insert(std::make_pair(camera->GetID(), video_view));
        camera = this->cameras->GetNextCamera();
    }
}

VideosForm::~VideosForm()
{

}

void VideosForm::resizeEvent(QResizeEvent *event)
{
    int width = event->size().width();
    int height = width * 1080 / 1920;

    for (auto it = videos.begin(); it != videos.end(); ++it) {
        VideoView *video_view = it->second;
        assert(video_view);
        video_view->setFixedHeight(height);
    }

    QWidget::resizeEvent(event);
}

void VideosForm::addCamera(int id)
{
    assert(cameras);
    Camera *camera = cameras->GetCamera(id);
    if (camera) {
        int width = size().width();
        int height = width * 1080 / 1920;
        VideoView * video_view = new VideoView(camera->GetVideoStream());
        //video_view->setStyleSheet("background-color: black");
        video_view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        video_view->setFixedHeight(height);
        videos_layout->addWidget(video_view);
        videos.insert(std::make_pair(id, video_view));
    }
}

void VideosForm::removeCamera(int id)
{
    auto it = videos.find(id);
    if (it != videos.end()) {
        VideoView *video_view = it->second;
        videos_layout->removeWidget(video_view);
        delete video_view;
        videos.erase(it);
    }
}
