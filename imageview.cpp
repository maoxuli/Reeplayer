#include "imageview.h"
#include <QApplication>
#include <QStyle>
#include <QKeyEvent>
#include <QDebug>

ImageView::ImageView(QWidget *parent)
    : QGraphicsView(parent)
{
    setFrameShape(QFrame::NoFrame);
    setBackgroundBrush(QBrush(Qt::darkGray, Qt::SolidPattern));
    //setBackgroundBrush(QApplication::style()->standardPalette().brush(QPalette::Background));
    setScene(&_scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setDragMode(QGraphicsView::ScrollHandDrag);

    _scene.addItem(&_image_item);
    _scene.addItem(&_text_item);
    _text_item.setDefaultTextColor(Qt::red);
}

ImageView::~ImageView()
{

}

// fix the problem of fitInView with a margin
void ImageView::fitInView_fixed(const QRectF &rect, Qt::AspectRatioMode aspectRatioMode)
{
    if (!scene() || rect.isNull())
        return;
    auto unity = transform().mapRect(QRectF(0, 0, 1, 1));
    if (unity.isEmpty())
        return;
    scale(1/unity.width(), 1/unity.height());
    auto viewRect = viewport()->rect();
    if (viewRect.isEmpty())
        return;
    auto sceneRect = transform().mapRect(rect);
    if (sceneRect.isEmpty())
        return;
    qreal xratio = viewRect.width() / sceneRect.width();
    qreal yratio = viewRect.height() / sceneRect.height();

    // Respect the aspect ratio mode.
    switch (aspectRatioMode) {
    case Qt::KeepAspectRatio:
        xratio = yratio = qMin(xratio, yratio);
        break;
    case Qt::KeepAspectRatioByExpanding:
        xratio = yratio = qMax(xratio, yratio);
        break;
    case Qt::IgnoreAspectRatio:
        break;
    }
    scale(xratio, yratio);
    centerOn(rect.center());
}

void ImageView::resizeEvent(QResizeEvent *event)
{
    int width = _image_item.pixmap().width();
    int height = _image_item.pixmap().height();

    if(width > 0 && height > 0)
    {
        fitInView_fixed(QRect(0, 0, width, height), Qt::KeepAspectRatio);
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
    int width = _image_item.pixmap().width();
    int height = _image_item.pixmap().height();
    if(width > 0 && height > 0)
    {
        fitInView_fixed(QRect(0, 0, width, height), Qt::KeepAspectRatio);
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
    _image_item.setPixmap(pm);
    QPointF offset = QPointF(-width/2.0, -height/2.0);
    _image_item.setOffset(offset);
    _scene.setSceneRect(offset.x(), offset.y(), width, height);
    setSceneRect(offset.x(), offset.y(), width, height);
    translate(1.0, 1.0);
    SetScale(1.0, 1.0);

    _text_item.setPlainText("");
    _text_item.setTextWidth(70.0);
    _text_item.setPos(-offset.x() - 80.0, offset.y() + 10.0);
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
void ImageView::UpdateImage(const QImage& image, const std::string& text)
{
    QPixmap pixmap = QPixmap::fromImage(image);
    _image_item.setPixmap(pixmap);
    if(!text.empty())
        _text_item.setPlainText(text.c_str());
}

// update text
void ImageView::UpdateText(const std::string& text)
{
    _text_item.setPlainText(text.c_str());
}
