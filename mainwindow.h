#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QButtonGroup>
#include <QStackedLayout>

class MainWindow : public QMainWindow
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
};

#endif // MAINWINDOW_H
