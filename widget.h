#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPen>
#include <windows.h>
#include "visualequalizer.h"
#include "bass/bass.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    enum Mode {
        Play,
        Pause,
        Stop
    };

    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private:
    Ui::Widget *ui;
    QTimer          timer;
    QString         filename;
    VisualEqualizer visualEq;
    DWORD           channel;
    Mode            currentMode;

private slots:
    void openFileSlot();
    void playFileSlot();
    void pauseFileSlot();
    void stopFileSlot();
    void sliderMovedSlot(int val);
    void timerSlot();
};

#endif // WIDGET_H
