#include "imageview.h"
#include <QApplication>
#include <QStyle>
#include <QKeyEvent>
#include <QDebug>

ImageView::ImageView(QWidget *parent)
    : QGraphicsView(parent)
{
    setFrameShape(QFrame::NoFrame);
    //setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    //setBackgroundBrush(QApplication::style()->standardPalette().brush(QPalette::Background));
    setScene(&_scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setDragMode(QGraphicsView::ScrollHandDrag);

    _scene.addItem(&_im_item);
    _scene.addItem(&_label_item);
    _label_item.setDefaultTextColor(Qt::red);
}

ImageView::~ImageView()
{

}

void ImageView::resizeEvent(QResizeEvent *event)
{
    int width = _im_item.pixmap().width();
    int height = _im_item.pixmap().height();

    if(width > 0 && height > 0)
    {
        fitInView(0, 0, width, height, Qt::KeepAspectRatio);
    }

    QGraphicsView::resizeEvent(event);
}

void ImageView::zoomIn()
{
    float factor = 0.1;
    if (factor > 0)
        SetScale(1 + factor, 1 + factor);
}

void ImageView::zoomOut()
{
    float factor = 0.1;
    if (factor > 0 && factor < 1)
        SetScale(1 - factor, 1- factor);
}

// set fit window
void ImageView::fitWindow()
{
    int width = _im_item.pixmap().width();
    int height = _im_item.pixmap().height();
    if(width > 0 && height > 0)
    {
        fitInView(0, 0, width, height, Qt::KeepAspectRatio);
    }
}

void ImageView::actualSize()
{
    SetScale(1.0, 1.0);
}

// reset to a initial size
void ImageView::Reset(int width, int height)
{
    // set image and label
    QPixmap pm(width, height);
    _im_item.setPixmap(pm);
    QPointF offset = QPointF(-width/2.0, -height/2.0);
    _im_item.setOffset(offset);
    _scene.setSceneRect(offset.x(), offset.y(), width, height);
    setSceneRect(offset.x(), offset.y(), width, height);
    translate(1.0, 1.0);
    SetScale(1.0, 1.0);

    _label_item.setPlainText("");
    _label_item.setTextWidth(70.0);
    _label_item.setPos(-offset.x() - 80.0, offset.y() + 10.0);
}

void ImageView::GetScale(float &sx, float &sy)
{
    sx = transform().m11();
    sy = transform().m22();
}

void ImageView::SetScale(float sx, float sy)
{
    if (abs(sx - 1.0) < 0.05 && abs(sy - 1.0) < 0.05)
        QGraphicsView::resetMatrix(); // remove cumulative error
    else
        QGraphicsView::scale(sx, sy);
}

// update image,
void ImageView::UpdateImage(const QImage& im, const std::string& label)
{
    QPixmap pm = QPixmap::fromImage(im);
    _im_item.setPixmap(pm);
    if(!label.empty())
        _label_item.setPlainText(label.c_str());
}
