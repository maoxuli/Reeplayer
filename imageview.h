#ifndef IMAGEVEIW_H
#define IMAGEVEIW_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>

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
    // zoom by factor
    void ZoomIn(float factor);
    void ZoomOut(float factor);
    // Scale to fit window
    void FitWindow();

    // update image with the label text
    void UpdateImage(const QImage& im, const std::string& label = "");

signals:

private slots:
    void resizeEvent(QResizeEvent *event);

private:
    QGraphicsScene _scene;
    QGraphicsPixmapItem _im_item;
    QGraphicsTextItem _label_item;
    double _initial_scale;
};

#endif // ifndef IMAGEVEIW_H
