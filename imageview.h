#ifndef IMAGEVEIW_H
#define IMAGEVEIW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>

class ImageView : public QGraphicsView
{
    Q_OBJECT

public:
    ImageView(QWidget *parent = 0);
    ~ImageView();

    // Reset with a initial size
    void Reset(int width = 0, int height = 0);
    // Scale
    void SetScale(float sx, float sy);
    void GetScale(float &sx, float &sy);

    // update image with the label text
    void UpdateImage(const QImage& image, const std::string& text = "");

    // update text label
    void UpdateText(const std::string& text);

public slots:
    // zoom by factor
    void zoomIn();
    void zoomOut();
    // scale to fit window
    void fitWindow();
    // reset to actual size
    void actualSize();

signals:

private slots:
    void resizeEvent(QResizeEvent *event);

private:
    void fitInView_fixed(const QRectF &rect, Qt::AspectRatioMode aspectRatioMode);

private:
    QGraphicsScene _scene;
    QGraphicsPixmapItem _image_item;
    QGraphicsTextItem _text_item;
};

#endif // ifndef IMAGEVEIW_H
