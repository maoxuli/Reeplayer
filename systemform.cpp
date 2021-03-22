#include "systemform.h"

#include <cassert>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QFrame>
#include <QSpacerItem>
#include <QListWidget>

SystemForm::SystemForm(CamerasManager *manager, QWidget *parent) :
    QWidget(parent),
    cameras(manager)
{
    // main layout
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->setAlignment(Qt::AlignTop);

    // camera
    QLabel *cameras_lable = new QLabel("Current camera: ");
    cameras_box = new QComboBox();

    cameras_box->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    cameras_box->setFixedHeight(30);

    QVBoxLayout *cameras_layout = new QVBoxLayout();
    cameras_layout->addWidget(cameras_lable);
    cameras_layout->addWidget(cameras_box);

    main_layout->addLayout(cameras_layout);
    main_layout->addSpacerItem(new QSpacerItem(30, 30, QSizePolicy::Preferred, QSizePolicy::Fixed));

    // camera's working mode
    QLabel *mode_label = new QLabel("Camera's working mode:");
    QRadioButton *stitching_mode_button = new QRadioButton("Video stitching");
    QRadioButton *estimation_mode_button = new QRadioButton("Transfrom estimation");
    QRadioButton *left_calib_mode_button = new QRadioButton("Left camera calibration");
    QRadioButton *right_calib_mode_button = new QRadioButton("Right camera calibration");
    QPushButton *apply_mode_button = new QPushButton("Apply");

    mode_group = new QButtonGroup();
    mode_group->addButton(stitching_mode_button, 0);
    mode_group->addButton(estimation_mode_button, 1);
    mode_group->addButton(left_calib_mode_button, 2);
    mode_group->addButton(right_calib_mode_button, 3);
    stitching_mode_button->setChecked(true);

    QFrame *mode_buttons_frame = new QFrame();
    mode_buttons_frame->setFrameShape(QFrame::StyledPanel);
    QVBoxLayout *mode_futtons_layout = new QVBoxLayout(mode_buttons_frame);
    mode_futtons_layout->addWidget(stitching_mode_button);
    mode_futtons_layout->addWidget(estimation_mode_button);
    mode_futtons_layout->addWidget(left_calib_mode_button);
    mode_futtons_layout->addWidget(right_calib_mode_button);

    apply_mode_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    apply_mode_button->setFixedSize(80, 30);

    QVBoxLayout *mode_layout =new QVBoxLayout();
    mode_layout->addWidget(mode_label);
    mode_layout->addWidget(mode_buttons_frame);
    mode_layout->addWidget(apply_mode_button, 0, Qt::AlignCenter);

    main_layout->addLayout(mode_layout);
    main_layout->addSpacerItem(new QSpacerItem(30, 30, QSizePolicy::Preferred, QSizePolicy::Fixed));

    // video files location
    QLabel *path_label = new QLabel("Recording path:");
    QListWidget *path_list = new QListWidget();
    QPushButton *add_path_button = new QPushButton("New");
    QPushButton *apply_path_button = new QPushButton("Apply");

    path_list->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    path_list->setFixedHeight(200);

    add_path_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    add_path_button->setFixedSize(80, 30);

    apply_path_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    apply_path_button->setFixedSize(80, 30);

    QHBoxLayout *path_buttons_layout = new QHBoxLayout();
    path_buttons_layout->setAlignment(Qt::AlignCenter);
    path_buttons_layout->addWidget(add_path_button);
    path_buttons_layout->addWidget(apply_path_button);

    QVBoxLayout *path_layout = new QVBoxLayout();
    path_layout->addWidget(path_label);
    path_layout->addWidget(path_list);
    path_layout->addLayout(path_buttons_layout);

    main_layout->addLayout(path_layout);
    main_layout->addSpacerItem(new QSpacerItem(50, 10, QSizePolicy::Preferred, QSizePolicy::Fixed));

    // attach to cameras manager
    connect(cameras_box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SystemForm::changeCamera);
    connect(apply_mode_button, &QPushButton::clicked, this, &SystemForm::applyMode);
    connect(apply_path_button, &QPushButton::clicked, this, &SystemForm::applyPath);
    connect(add_path_button, &QPushButton::clicked, this, &SystemForm::showAddPath);
}

SystemForm::~SystemForm()
{

}

void SystemForm::showEvent(QShowEvent *event)
{
    assert(cameras);
    cameras_box->clear();
    Camera *camera = cameras->GetFirstCamera();
    while (camera) {
        cameras_box->addItem(camera->name().c_str(), camera->id());
        camera = cameras->GetNextCamera();
    }

    camera = cameras->GetCurrentCamera();
    if (camera) {
        int idx = cameras_box->findData(camera->id());
        if (idx >= 0)
          cameras_box->setCurrentIndex(idx);
    }
    else if (cameras_box->count() > 0){
        cameras_box->setCurrentIndex(0);
        int id = cameras_box->currentData().toInt();
        cameras->SetCurrentCamera(id);
    }

    camera = cameras->GetCurrentCamera();
    if (camera) {
        updateState();
    }

    QWidget::showEvent(event);
}

void SystemForm::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
}

void SystemForm::changeCamera(int idx)
{
    if (cameras_box->count() > 0) {
        int id = cameras_box->currentData().toInt();
        cameras->SetCurrentCamera(id);

        assert(cameras);
        Camera *camera = cameras->GetCurrentCamera();
        if (camera) {
            updateState();
        }
    }
}

void SystemForm::updateState()
{
    assert(cameras);
    Camera *camera = cameras->GetCurrentCamera();
    if (camera) {
        Camera::State state;
        if (camera->checkState(state)) {
            int mode = state.mode;
            mode_group->button(mode)->setChecked(true);
        }
    }
}

void SystemForm::applyMode()
{
    int mode = mode_group->checkedId();
    qDebug() << "apply mode: " << mode;
    assert(cameras);
    Camera *camera = cameras->GetCurrentCamera();
    if (camera) {
        camera->changeMode(mode);
        updateState();
    }
}

void SystemForm::applyPath()
{
    assert(cameras);
    Camera *camera = cameras->GetCurrentCamera();
    if (camera) {

    }
}

void SystemForm::showAddPath()
{
    assert(cameras);
    Camera *camera = cameras->GetCurrentCamera();
    if (camera) {

    }
}
