#ifndef PACMANWINDOW_H
#define PACMANWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PacManWindow; }
QT_END_NAMESPACE

class PacManWindow : public QMainWindow
{
    Q_OBJECT

public:
    PacManWindow(QWidget *parent = nullptr);
    ~PacManWindow();

private:
    Ui::PacManWindow *ui;
};
#endif // PACMANWINDOW_H
