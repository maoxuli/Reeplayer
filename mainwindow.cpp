#include "mainwindow.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QWidget>
#include <QScrollArea>
#include <QStackedLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "camerasform.h"
#include "videosform.h"
#include "filesform.h"

#define NUM_FORMS 4

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // window size (18:9)
    QRect rc = QDesktopWidget().availableGeometry();
    qDebug() << "screen: " << rc;
    int w = (rc.height() - 100) * 9 / 18;
    //qDebug() << "w: " << w;
    QRect window_rc((rc.width() - w)/2, rc.top(), w, rc.height() - 100);
    qDebug() << "window: " << window_rc;
    setGeometry(window_rc);
    setWindowTitle("Reeplayer");

    // client area
    QWidget *center_widget = new QWidget();
    setCentralWidget(center_widget);

    // cameras manager
    cameras = new CamerasManager(cameras_config);
    assert(cameras);

    // forms
    QWidget *forms[NUM_FORMS];
    assert(NUM_FORMS >= 4);
    forms[0] = new CamerasForm(cameras);
    forms[1] = new VideosForm(cameras);
    forms[2] = new FilesForm(cameras);
    forms[3] = new QWidget();

    // buttons
    QPushButton *buttons[NUM_FORMS];
    buttons[0] = new QPushButton("Cameras");
    buttons[1] = new QPushButton("Videos");
    buttons[2] = new QPushButton("Files");
    buttons[3] = new QPushButton("System");

    // layout
    forms_layout = new QStackedLayout();
    button_group = new QButtonGroup();

    QFrame *buttons_frame = new QFrame(center_widget);
    QHBoxLayout *buttons_layout= new QHBoxLayout();
    buttons_frame->setLayout((buttons_layout));
    buttons_frame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    buttons_frame->setFixedHeight(50);

    for (int i = 0; i < NUM_FORMS; i++)
    {
        // wrap the form with scroll area
        // and add the scroll area to the stacked layout
        QScrollArea *scroll_area = new QScrollArea();
        scroll_area->setFrameShape(QFrame::NoFrame);
        scroll_area->setWidgetResizable(true);
        scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scroll_area->setWidget(forms[i]);
        forms_layout->addWidget(scroll_area);
        // add button to group
        // with index as the button id
        button_group->addButton(buttons[i], i);
        // add button to frame
        // so can control the height of the buttons region
        buttons[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        buttons[i]->setFixedHeight(40);
        buttons_layout->addWidget(buttons[i]);
    }
    forms_layout->setCurrentIndex(0);

    // main layout
    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->addLayout(forms_layout);
    main_layout->addWidget(buttons_frame);
    center_widget->setLayout(main_layout);

    // signal from buttons
    connect(button_group, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::clickedButton);
}

MainWindow::~MainWindow()
{

}

void MainWindow::clickedButton(QAbstractButton *button)
{
    int id = button_group->id(button);
    forms_layout->setCurrentIndex(id);
}
