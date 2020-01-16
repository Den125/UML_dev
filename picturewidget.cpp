#include "picturewidget.h"
#include <QDebug>

PictureWidget::PictureWidget(QWidget *parent):QWidget (parent)
{

}

void PictureWidget::loadImage(const QString &fileName)
{
    if(!fileName.isNull())
    {
        m_Image.load(fileName);
        this->update();
        setMinimumSize(m_Image.size());
    }
}

void PictureWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    if(!m_Image.isNull())
    {
        p.drawImage(QPoint(0,0),m_Image);
    }
}

void PictureWidget::clear()
{
    this->update();
}
