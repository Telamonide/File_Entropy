#include "FileEntropy.h"
#include "ui_FileEntropy.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
//Запрещение изменения размеров окна
    setWindowFlags( Qt::Window
    | Qt::WindowCloseButtonHint
    | Qt::CustomizeWindowHint);

    ui->setupUi(this);

//Разрешение контроля положения курсора на форме
    this->setMouseTracking(true);

    connect (ui->File_Button, SIGNAL(clicked()), this, SLOT(FindFilePath()));
    connect( ui->Cancel_Button, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(ShowAmount()));

    ui->labelBytesCount->setText(QString::number(0));
    setAcceptDrops(true);
    ui->frame->setAcceptDrops(true);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::FindFilePath()
{
// Получение путь к файлу
    entropyFilePath = QFileDialog::getOpenFileName();

// Выделение имени файла из пути
    entropyFileName = entropyFilePath.split("/");

    QWidget::setWindowTitle(QWidget::windowTitle()+" "+entropyFileName.last());

    QFile work_file(entropyFilePath);
    if (!work_file.open(QFile::ReadOnly))
    {
        QMessageBox *pmb=new QMessageBox (QMessageBox::Warning,tr("Error"), tr("Cannot open file"), QMessageBox::Ok);
        pmb->exec();
        delete pmb;
    }

// readedData - массив частот символов
    for(int i = 0; i < symbNumber; i++) readedData[i] = 0;

/* Цикл подсчета частот символов. Заканчивается если значение считанных символов dataCounter меньше,
   чем размер буфера для считывания arraySize */
    do
    {
// dataCounter хранит реально количество реально прочитанных байтов
        dataCounter = 0;
        dataCounter = work_file.read(byteData, arraySize);

// Приведение кодов считанных символов
        for(int i = 0; i < dataCounter; i++)
        {
            if (byteData[i] < 0)
            {
                abs = (int)byteData[i]+symbNumber;
            }
            else abs = (int)byteData[i];

        // увеличение счетчика частот символов
            readedData[abs]++;
        }
    }while(dataCounter == arraySize);
    work_file.close();

    ui->spinBox->setValue(0);
    ShowAmount();


    percByte = 0;
    for(int i = 0; i < symbNumber; i++)
    {
        convertData[i] = RoundToInt( log10((double)readedData[i] + 1)*100);
    }

    abs = convertData[FindMax(convertData, symbNumber)];
    for(int i = 0; i < symbNumber; ++i)
    {
        convertData[i] = RoundToInt(convertData[i]/(double)abs*100 );
        percByte = percByte + convertData[i];
    }
    percByte = RoundToInt( percByte/(double)symbNumber);
    paintFlag = 1;
    ui->frame->update();
    squere = QString::number(percByte) + " %";
    ui->label_entr_perc->setText(squere);
}

// Метод предназначен для определения максимального значения в массиве
// Принимает массив целых чисел и его размер
// Возвращает индекс элемента содержащего максимальное значение
int Widget::FindMax(int arr[], int arrsize)
{
    int max = 0;
    int index_of_max = 0;
    for (int i = 0; i < arrsize; i++)
    {
        if (max < arr[i])
        {
            index_of_max = i;
            max = arr[i];
        }
    }
    return index_of_max;
}

// Метод предназначен для округления чисел типа double до целого значения
// В качестве аргумента принимает значения числа, которое необходимо округлить
// Возвращает переменную типа int
int Widget::RoundToInt(double some_variable)
{
    int a;
    double b;

    b = floor(some_variable);
    if (some_variable >= 0)
    {
        if ((some_variable - b) >= 0.5)
        {
            a = (int)some_variable + 1;
        }
        else a = (int)some_variable;
    }
    else
    {
        if ((some_variable - b) <= 0.5)
        {
            a = (int)some_variable - 1;
        }
        else a = (int)some_variable;
    }
    return a;
}

void Widget::paintEvent(QPaintEvent *pe)
{
   if(paintFlag == 1)
   {
       QPainter mypainter(this);
       mypainter.setPen(Qt::darkRed);
       int hystHeight;
       mypainter.setRenderHint(QPainter::Antialiasing, true);
       mypainter.translate(10, 110);
       for (int i = 0; i < symbNumber; i++)
       {
           if (i == ui->spinBox->value())
           {
               mypainter.setPen(Qt::yellow);
               hystHeight = (convertData[i]>100)? -100: -convertData[i];
               mypainter.drawLine(QPointF(i, 0), QPointF(i, hystHeight));
               mypainter.setPen(Qt::darkRed);
           }
           else
           {
               hystHeight = (convertData[i]>100)? -100: -convertData[i];
               mypainter.drawLine(QPointF(i, 0), QPointF(i, hystHeight));
           }
       }
   }
}

void Widget::ShowAmount()
{
    int posX;
    posX = ui->spinBox->value();
    ui->labelBytesCount->setText(QString::number(readedData[posX]));
    ui->frame->update();
}

void Widget::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->pos().x()>9 && ev->pos().x()<266 && ev->pos().y()>9 && ev->pos().y()<112)
    {
        ui->spinBox->setValue(ev->pos().x()-10);
    }
}

//TODO Drag-and-drop
void Widget::dragEnterEvent(QDragEnterEvent *event)
{
        event->accept();
}

void Widget::dropEvent(QDropEvent *event)
{
    event->accept();
//    QMessageBox *pmb=new QMessageBox (QMessageBox::Warning,tr("Drop"), tr("Get dropped file"), QMessageBox::Ok);
//    pmb->exec();
//    delete pmb;
}
