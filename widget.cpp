#include <QMessageBox>
#include <QFileDialog>
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    channel(0)
{
    ui->setupUi(this);
    ui->equalizerLayout->addWidget(&visualEq);

    // Инициализация аудио - по умолчанию, 44100hz, stereo, 16 bits
    if (!BASS_Init(-1, 44100, 0, 0, 0)) {
        QMessageBox::critical(this, tr("Visualizer"),
            tr("Can't init audio!"), QMessageBox::Ok);
    }

    // Текущий режим: воспроизведение остановлено
    currentMode = Widget::Stop;

    ui->slider->setEnabled(false);

    connect(ui->openFileBtn, SIGNAL(clicked()), this, SLOT(openFileSlot()));
    connect(ui->playFileBtn, SIGNAL(clicked()), this, SLOT(playFileSlot()));
    connect(ui->stopFileBtn, SIGNAL(clicked()), this, SLOT(stopFileSlot()));
    connect(ui->pauseFileBtn, SIGNAL(clicked()), this, SLOT(pauseFileSlot()));
    connect(ui->slider, SIGNAL(sliderMoved(int)), this, SLOT(sliderMovedSlot(int)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
}

Widget::~Widget()
{
    delete ui;
    //Освобождаем объекты перед закрытием
    BASS_Stop();              // останавливаем проигрывание
    BASS_StreamFree(channel); // освобождаем звуковой канал
    BASS_Free();              // освобождаем ресурсы используемые Bass
}

void Widget::openFileSlot()
{
    stopFileSlot();
    BASS_StreamFree(channel);

    ui->slider->setEnabled(false);

    filename = QFileDialog::getOpenFileName(this,
        "Open audio file", QDir::currentPath(), "Audio files (*.wav *.mp3)");
    ui->openedFileLabel->setText(QString("Opened: ")+filename);

    // Пытаемся загрузить файл и получить дескриптор канала.
    channel = BASS_StreamCreateFile(FALSE, filename.toLocal8Bit().data(), 0, 0, 0);

    // Если дескриптор канала = 0 (файл по какой то причине не может быть загружен),
    // то выдаем сообщение об ошибке.
    if (channel == 0) {
        QMessageBox::warning(this, "Visualizer",
            "Can't load '"+filename+"'!", QMessageBox::Ok);
        return;
    }

    // Настраиваем полосу проигрывания.
    ui->slider->setEnabled(true);
    ui->slider->setMinimum(0);
    ui->slider->setMaximum(BASS_ChannelGetLength(channel, 0) - 1);
}

void Widget::playFileSlot()
{
    if (currentMode == Widget::Play)
        return;

    // Проверяем существует ли файл,
    // если файл не существует, то ругаемся
    QFile file(filename);

    if (!file.exists())
    {
        QMessageBox::warning(this, "Visualizer",
            "File '"+filename+"' does not exist!", QMessageBox::Ok);

        // Останавливаем и освобождаем канал воспроизведения.
        BASS_ChannelStop(channel);
        BASS_StreamFree(channel);
        return;
    }

    // Пытаемся воспроизвести файл, если это невозможно, то выдаем сообщение об ошибке.
    if (!BASS_ChannelPlay(channel, FALSE))
    {
        QMessageBox::warning(this, "Visualizer",
            "Can't to play '"+filename+"'!", QMessageBox::Ok);
        return;
    }

    // Устанавливаем режим: воспроизводится.
    currentMode = Widget::Play;

    // Запускаем таймер каждые 30 мс.
    timer.start(30);
}

void Widget::pauseFileSlot()
{
    if (currentMode == Widget::Play) {
        BASS_ChannelPause(channel);   // паузим воспроизведение
        currentMode = Widget::Pause;  // устанавливаем режим: воспроизведение запаузено
        visualEq.clearScene();        // очищаем эквалайзер
        visualEq.update();
    }
}

void Widget::stopFileSlot()
{
    BASS_ChannelStop(channel);   // останавливаем воспроизведение
    currentMode = Widget::Stop;  // устанавливаем режим: воспроизведение остановлено
    timer.stop();
    ui->slider->setSliderPosition(0); // сбрасываем полосу проигрывания в 0
    visualEq.clearScene();            // очищаем эквалайзер
    visualEq.update();
}

void Widget::sliderMovedSlot(int val)
{
  BASS_ChannelSetPosition(channel, val, 0);
}

void Widget::timerSlot()
{
    unsigned int currentPos = BASS_ChannelGetPosition(channel, 0);
    if (currentPos == BASS_ChannelGetLength(channel, 0))
    {
        stopFileSlot();
        return;
    }

    ui->slider->setSliderPosition(currentPos);

    // Получаем данные для визуализации.
    BASS_ChannelGetData(channel, visualEq.fftData, BASS_DATA_FFT256);
    visualEq.update();
}
