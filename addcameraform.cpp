#include "addcameraform.h"

#include <cassert>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>

AddCameraForm::AddCameraForm(CamerasManager *manager, QWidget *parent) :
    QWidget(parent),
    cameras(manager)
{
    // buttons
    QPushButton *cancel_button = new QPushButton("Cancel");
    QPushButton *done_button = new QPushButton("Done");

    connect(cancel_button, &QPushButton::clicked, this, &AddCameraForm::cancelTask);
    connect(done_button, &QPushButton::clicked, this, &AddCameraForm::doneTask);

    // buttons in a frame
    QFrame *buttons_frame = new QFrame();
    QHBoxLayout *buttons_layout = new QHBoxLayout(buttons_frame);
    buttons_layout->setSpacing(0);
    buttons_layout->setMargin(0);
    buttons_layout->setAlignment(Qt::AlignTop);
    buttons_layout->addWidget(cancel_button);
    buttons_layout->addSpacerItem(new QSpacerItem(40, 40, QSizePolicy::Expanding, QSizePolicy::Preferred));
    buttons_layout->addWidget(done_button);

    buttons_frame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    buttons_frame->setFixedHeight(60);

    cancel_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    cancel_button->setFixedSize(70, 40);

    done_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    done_button->setFixedSize(70, 40);

    // camera info
    QLabel *camera_name_label = new QLabel("Camera name:");
    camera_name_edit = new QPlainTextEdit();
    camera_name_edit->setPlainText("Camera");

    QLabel *camera_ip_label = new QLabel("Camera IP:");
    camera_ip_edit = new QPlainTextEdit();
    camera_ip_edit->setPlainText("0.0.0.0");

    // camera info in a grid
    QGridLayout *grid_layout = new QGridLayout();
    grid_layout->setSpacing(0);
    grid_layout->setMargin(0);
    grid_layout->addWidget(camera_name_label, 0, 0, 1, 1);
    grid_layout->addWidget(camera_name_edit, 0, 1, 1, 1);
    grid_layout->addWidget(camera_ip_label, 1, 0, 1, 1);
    grid_layout->addWidget(camera_ip_edit, 1, 1, 1, 1);

    camera_name_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    camera_name_label->setFixedSize(120, 40);

    camera_name_edit->setStyleSheet("background-color: #E8F6F3");
    camera_name_edit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    camera_name_edit->setFixedHeight(32);

    camera_ip_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    camera_ip_label->setFixedSize(120, 40);

    camera_ip_edit->setStyleSheet("background-color: #E8F6F3");
    camera_ip_edit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    camera_ip_edit->setFixedHeight(32);

    // main layout
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addWidget(buttons_frame);
    main_layout->addLayout(grid_layout);
    main_layout->addSpacerItem(new QSpacerItem(40, 40, QSizePolicy::Preferred, QSizePolicy::Expanding));
}

AddCameraForm::~AddCameraForm()
{

}

void AddCameraForm::showEvent(QShowEvent *event)
{
    assert(cameras);
    std::string camera_name = "Camera " + std::to_string(cameras->GetNextCameraID());
    camera_name_edit->setPlainText(camera_name.c_str());

    QWidget::showEvent(event);
}

void AddCameraForm::cancelTask()
{
    emit showForm(CAMERAS_FORM);
}

void AddCameraForm::doneTask()
{
    std::string camera_name = camera_name_edit->toPlainText().toStdString();
    std::string camera_ip = camera_ip_edit->toPlainText().toStdString();
    assert(cameras);
    cameras->AddCamera(camera_ip, camera_name);
    emit showForm(CAMERAS_FORM);
}
