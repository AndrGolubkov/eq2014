#include <QPainter>
#include <QPaintEvent>
#include <math.h>
#include "visualequalizer.h"

VisualEqualizer::VisualEqualizer(QWidget *parent) :
    QWidget(parent)
{
    clearScene();
}

void VisualEqualizer::paintEvent(QPaintEvent *event)
{
    QPen pen(Qt::black, 1, Qt::SolidLine);
    QPainter painter(this);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::darkGreen));
    painter.translate(0, event->rect().bottom());

    int xOffset = 0;
    for (int i=0; i<128; ++i) {
        painter.drawRect(xOffset, 0, 4, fabs(fftData[i])*(-300));
        xOffset += 4;
    }
}

void VisualEqualizer::clearScene()
{
    for (int i=0; i < 128; ++i)
        fftData[i] = 0;
}
