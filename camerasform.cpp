#include "camerasform.h"

CamerasForm::CamerasForm(CamerasManager *manager, QWidget *parent) :
    QWidget(parent),
    cameras(manager)
{

}

CamerasForm::~CamerasForm()
{

}
