#include "videosform.h"

#include <QVBoxLayout>
#include <QResizeEvent>

VideosForm::VideosForm(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *videos_layout = new QVBoxLayout();
    videos_layout->setAlignment(Qt::AlignTop);
    setLayout(videos_layout);

    int width = size().width();
    int height = width * 1080 / 1920;

    for (int i = 0; i < 1; i++)
    {
        VideoView * video_view = new VideoView();
        //video_view->setStyleSheet("background-color: black");
        video_view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        video_view->setFixedHeight(height);
        videos_layout->addWidget(video_view);
        videos.push_back(video_view);
    }
}

VideosForm::~VideosForm()
{

}

void VideosForm::resizeEvent(QResizeEvent *event)
{
    int width = event->size().width();
    int height = width * 1080 / 1920;

    for (int i = 0; i < videos.size(); i++)
    {
        VideoView *video = videos[i];
        video->setFixedHeight(height);
    }

    QWidget::resizeEvent(event);
}
