#include "picturewidget.h"
#include <QDebug>

PictureWidget::PictureWidget(QWidget *parent):QWidget (parent)
{

}

void PictureWidget::paintEvent(QPaintEvent *event) {
  QPainter painter(this);

  if (false == m_pixmap.isNull()) {
    QSize widgetSize = size();
    QImage scaledPixmap = m_pixmap.scaled(widgetSize, Qt::KeepAspectRatio);
    QPoint center((widgetSize.width() - scaledPixmap.width())/2,
                  (widgetSize.height() - scaledPixmap.height())/2);
    painter.drawImage(0,0,/*center,*/ scaledPixmap);
  }

  QWidget::paintEvent(event);
}
void PictureWidget::loadImage(const QString &fileName)
{
    if (!m_pixmap.load(fileName))
    {
        m_pixmap.load(":/resource/error_image.png");
    }
    update();
}

void PictureWidget::clear()
{
    update();
}
