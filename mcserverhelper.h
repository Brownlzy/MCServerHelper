#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mcserverhelper.h"

class MCServerHelper : public QMainWindow
{
    Q_OBJECT

public:
    MCServerHelper(QWidget *parent = Q_NULLPTR);

private:
    Ui::MCServerHelperClass ui;
};
