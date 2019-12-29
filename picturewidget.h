#ifndef PICTUREWIDGET_H
#define PICTUREWIDGET_H
#include <QWidget>
#include <QPainter>

class PictureWidget:public QWidget
{
    Q_OBJECT
public:    
    explicit PictureWidget(QWidget *parent = nullptr);    
    void clear();
public slots:
    void loadImage(const QString &fileName);
protected:
    void paintEvent(QPaintEvent *event);
    QImage m_Image;
};

#endif // PICTUREWIDGET_H
