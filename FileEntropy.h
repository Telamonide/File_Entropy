#ifndef FILEENTROPY_H
#define FILEENTROPY_H

#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QByteArray>
#include <QPainter>
#include <cmath>
#include <QDragEnterEvent>
#include <QMimeData>

#define arraySize 4096
#define symbNumber 256

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent *);

private:
    Ui::Widget *ui;

    QString entropyFilePath, squere;
    QStringList entropyFileName;
    QPaintEvent *pe;

    int abs, dataCounter, readedData[symbNumber];
    int convertData[symbNumber];
    int paintFlag;
    long int percByte;
    char byteData[arraySize];

private slots:
// Метод для определения файла для расчета энтропии
    void FindFilePath();

// Метод
    void ShowAmount();


private:
    int FindMax (int *, int);
    int RoundToInt(double);
    void mouseMoveEvent (QMouseEvent *);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // FILEENTROPY_H
