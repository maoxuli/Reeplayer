#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QStackedLayout>

#include "camerasmanager.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void clickedButton(QAbstractButton *button);

private:
    QButtonGroup *button_group;
    QStackedLayout *forms_layout;

    // cameras manager
    std::string cameras_config;
    CamerasManager *cameras;
};

#endif // MAINWINDOW_H
