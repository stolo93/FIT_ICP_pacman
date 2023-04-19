#include "pacmanwindow.h"
#include "./ui_pacmanwindow.h"

PacManWindow::PacManWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PacManWindow)
{
    ui->setupUi(this);
}

PacManWindow::~PacManWindow()
{
    delete ui;
}

