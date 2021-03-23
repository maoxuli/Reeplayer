#include "removecameraform.h"

#include <cassert>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QListWidgetItem>

RemoveCameraForm::RemoveCameraForm(CamerasManager *manager, QWidget *parent) :
    QWidget(parent),
    cameras(manager)
{
    // buttons
    QPushButton *cancel_button = new QPushButton("Cancel");
    QPushButton *done_button = new QPushButton("Done");

    connect(cancel_button, &QPushButton::clicked, this, &RemoveCameraForm::cancelTask);
    connect(done_button, &QPushButton::clicked, this, &RemoveCameraForm::doneTask);

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
    buttons_frame->setFixedHeight(50);

    cancel_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    cancel_button->setFixedSize(70, 40);

    done_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    done_button->setFixedSize(70, 40);

    // info label
    QLabel *info_label = new QLabel("Select cameras to remove:");
    info_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    info_label->setFixedHeight(40);

    // cameras list
    camera_list = new QListWidget();
    //camera_list->setFrameShape(QFrame::NoFrame);
    //camera_list->setStyleSheet("background-color: #E8F6F3");
    camera_list->setSelectionMode(QListWidget::MultiSelection);

    connect(camera_list, &QListWidget::itemChanged, this, &RemoveCameraForm::checkItemState);
    connect(camera_list, &QListWidget::itemClicked, this, &RemoveCameraForm::checkItemSelection);

    // main layout
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->setAlignment(Qt::AlignTop);
    main_layout->addWidget(buttons_frame);
    main_layout->addWidget(info_label);
    main_layout->addWidget(camera_list);
    //main_layout->addSpacerItem(new QSpacerItem(40, 40, QSizePolicy::Preferred, QSizePolicy::Expanding));
}

RemoveCameraForm::~RemoveCameraForm()
{

}

void RemoveCameraForm::showEvent(QShowEvent *event)
{
    assert(cameras);
    camera_list->clear();
    Camera *camera = cameras->GetFirstCamera();
    while (camera) {
        std::string camera_name = camera->name();
        QListWidgetItem* item = new QListWidgetItem(camera_name.c_str(), camera_list);
        item->setData(Qt::UserRole, camera->id());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        item->setSizeHint(QSize(item->sizeHint().width(), 30));
        item->setFont(QFont("Arial", 12));
        camera = cameras->GetNextCamera();
    }

    QWidget::showEvent(event);
}

void RemoveCameraForm::checkItemState(QListWidgetItem *item)
{
    if (item->checkState() == Qt::Checked) {
        if (!item->isSelected())
            item->setSelected(true);
    }
    else {
        if (item->isSelected())
            item->setSelected(false);
    }
}

void RemoveCameraForm::checkItemSelection(QListWidgetItem *item)
{
    if (item->isSelected()) {
        if (item->checkState() != Qt::Checked)
            item->setCheckState(Qt::Checked);
    }
    else {
        if (item->checkState() == Qt::Checked)
            item->setCheckState(Qt::Unchecked);
    }
}

void RemoveCameraForm::cancelTask()
{
    emit showForm(CAMERAS_FORM);
}

void RemoveCameraForm::doneTask()
{
    auto selected_items = camera_list->selectedItems();
    for (int i = 0; i < selected_items.count(); i++) {
        QListWidgetItem *item = selected_items[i];
        int camera_id = item->data(Qt::UserRole).toInt();
        cameras->RemoveCamera(camera_id);
    }
    emit showForm(CAMERAS_FORM);
}
