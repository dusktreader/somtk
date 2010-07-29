#pragma once

#include <QMainWindow>

#include "colorhsom.h"
#include "tools.hpp"
#include "cvtypesplus.hpp"

namespace Ui
{
    class MainWindowTest;
}

class MainWindowTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowTest(QWidget *parent = 0);
    ~MainWindowTest();

private:
    Ui::MainWindowTest *ui;
};

