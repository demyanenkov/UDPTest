#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QUdpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_spinBoxPeriod_valueChanged(int arg1);
    void on_spinBoxPort_valueChanged(int arg1);
    void on_pushButtonMode_toggled(bool checked);
    void on_pushButtonRestart_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    void timerEvent(QTimerEvent *e);
    void start();
    void stop();

    QUdpSocket socket;
    int c = 0;
};

#endif // MAINWINDOW_H
