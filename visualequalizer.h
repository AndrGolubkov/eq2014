#ifndef VISUALEQUALIZER_H
#define VISUALEQUALIZER_H

#include <QWidget>

class VisualEqualizer : public QWidget
{
    Q_OBJECT
public:
    explicit VisualEqualizer(QWidget *parent = 0);
    float fftData[128];

    void clearScene();

protected:
    void paintEvent(QPaintEvent *event);
    
signals:
    
public slots:
    
};

#endif // VISUALEQUALIZER_H
