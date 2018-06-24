#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

// Check UDP-Connection

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startTimer(ui->spinBoxPeriod->value());
}

MainWindow::~MainWindow()
{
    stop();
    delete ui;
}

void MainWindow::start()
{
    if(ui->pushButtonMode->isChecked()){
        bool f = socket.bind(QHostAddress::Any, ui->spinBoxPort->value());
        ui->plainTextEdit->appendPlainText(f ? "OPENED" : "BAD BIND");
    }
}

void MainWindow::stop()
{
    socket.close();
}

void MainWindow::timerEvent(QTimerEvent *)
{
    QString s, t;
    QStringList states = { "UnconnectedState", "HostLookupState",  "ConnectingState", "ConnectedState", "BoundState", "ListeningState" };

    s += t.sprintf("%i  %s (%i) ", c, states[socket.state()].toLocal8Bit().data(), socket.state());

    if(ui->pushButtonRestart->isChecked()){
        QByteArray data;
        if(!ui->pushButtonMode->isChecked()){
            if(ui->checkBoxCounter->isChecked())
                data += c%256;
            if(ui->checkBoxText->isChecked())
                data += ui->lineEditData->text().toLocal8Bit();
            else{
                bool even=false;
                unsigned char last = 0;
                foreach (char b, ui->lineEditData->text().toLower().toLocal8Bit()) {
                    if(b>='0' && b<='9') last |= b-'0';
                    else if(b>='a' && b<='f') last |= b-'a'+10;
                    else if(b>='A' && b<='F') last |= b-'A'+10;
                    else continue;

                    if(even) data+=last, last=0;
                    else last<<=4;
                    even=!even;
                }
            }

            int n = socket.writeDatagram(
                        data,
                        QHostAddress(ui->lineEditIp->text().toLatin1().data()),
                        ui->spinBoxPort->value()
                        );
            s += t.sprintf(" WRITE:%i", n);
            if(n<=0) data.clear();
        }
        else
        {
            s += t.sprintf(" Avalible: %i", socket.bytesAvailable());

            while (socket.hasPendingDatagrams())
            {
                data.resize(socket.pendingDatagramSize());
                int n = socket.readDatagram(data.data(), data.size());
                s += t.sprintf(" READ:%i", n);
            }
        }
        if(data.size()>0){
            s+= "\n";
            foreach (char b, data)  s+=t.sprintf("%02x ", (unsigned char)b);
        }
    }
    ui->plainTextEdit->appendPlainText(s);
    c++;
}

void MainWindow::on_spinBoxPeriod_valueChanged(int period)
{
    startTimer(period);
}

void MainWindow::on_spinBoxPort_valueChanged(int )
{
    stop();
    start();
}

void MainWindow::on_pushButtonMode_toggled(bool checked)
{
    bool f = ui->pushButtonRestart->isChecked();
    on_pushButtonRestart_clicked(false);

    ui->pushButtonMode->setText(!checked ? "TRANMITER" : "RECEIVER");

    on_pushButtonRestart_clicked(f);
}

void MainWindow::on_pushButtonRestart_clicked(bool checked)
{
    ui->pushButtonRestart->setText(!checked ? "START" : "STOP");
    if(checked) start();
    else stop();
}
