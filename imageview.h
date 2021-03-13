#ifndef IMAGE_VEIW_H
#define IMAGE_VEIW_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>

class ImageView : public QGraphicsView
{
    Q_OBJECT

public:
    ImageView(QWidget *parent = 0);
    ~ImageView();

    // scale on current size
    void SetScale(float s);

    // display as actual size
    // or fit to window size
    void Reset(int width = 0, int height = 0, bool actual = false);

    // update image the label text
    void UpdateImage(const QImage& im, const std::string& label = "");

    void SetDrawingMode(bool set);

signals:

private slots:

private:
    QGraphicsScene _scene;
    QGraphicsPixmapItem _im_item;
    QGraphicsTextItem _label_item;
    double _initial_scale;
};

#endif // ifndef IMAGE_VEIW_H
