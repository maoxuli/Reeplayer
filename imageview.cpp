#include "imageview.h"
#include <QScrollBar>
#include <QKeyEvent>
#include <cassert>
#include <QDebug>

ImageView::ImageView(QWidget *parent)
    : QGraphicsView(parent)
    , _initial_scale(1.0)
{
    setFrameShape(QFrame::NoFrame);
    setScene(&_scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    SetDrawingMode(false);

    _scene.addItem(&_im_item);
    _scene.addItem(&_label_item);
    _label_item.setDefaultTextColor(Qt::red);
}

ImageView::~ImageView()
{

}

void ImageView::SetScale(float s)
{
    if(s == 1.0)
    {
        QGraphicsView::resetMatrix();

        int min = horizontalScrollBar()->minimum();
        int max = horizontalScrollBar()->maximum();
        horizontalScrollBar()->setValue(min + (max - min)/2.0);

        min = verticalScrollBar()->minimum();
        max = verticalScrollBar()->maximum();
        verticalScrollBar()->setValue(min + (max - min)/2.0);
        return;
    }
    QGraphicsView::scale(s, s);
}

void ImageView::Reset(int width, int height, bool actual)
{
    SetDrawingMode(false);

    // set image and label
    QPixmap pm(width, height);
    _im_item.setPixmap(pm);
    QPointF offset = QPointF(-width/2.0, -height/2.0);
    _im_item.setOffset(offset);
    _scene.setSceneRect(offset.x(), offset.y(), width, height);
    setSceneRect(offset.x(), offset.y(), width, height);
    translate(1.0, 1.0);

    SetScale(1.0);
    _initial_scale = 1.0;
    if(!actual && width > 0 && height > 0)
    {
        QRectF view_rc = viewport()->rect(); //VisibleRect();
        double h_scale = view_rc.width() / width;
        double v_scale = view_rc.height() / height;
        _initial_scale = h_scale;
        if(v_scale < _initial_scale)
            _initial_scale = v_scale;

    }
    SetScale(_initial_scale);

    _label_item.setPlainText("");
    _label_item.setTextWidth(70.0);
    _label_item.setPos(-offset.x() - 80.0, offset.y() + 10.0);
}

void ImageView::SetDrawingMode(bool set)
{
    if(set)
    {
        setDragMode(QGraphicsView::RubberBandDrag);
        viewport()->setCursor(Qt::CrossCursor);
    }
    else
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
}

// update image,
void ImageView::UpdateImage(const QImage& im, const std::string& label)
{
    SetDrawingMode(false);
    // update image
    QPixmap pm = QPixmap::fromImage(im);
    _im_item.setPixmap(pm);

    // timestamp
    if(!label.empty())
        _label_item.setPlainText(label.c_str());
}
