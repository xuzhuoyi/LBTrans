#include <QMovie>
#include "qgiflabel.h"

QGifLabel::QGifLabel(QWidget *parent)
    :QLabel(parent), m_movie(new QMovie(this))
{
    this->setMovie(m_movie);
}

QGifLabel::QGifLabel(const QString &filename, QWidget *parent) :
    QLabel(parent), m_movie(new QMovie(this))
{
    m_movie->setFileName(filename);
    this->setMovie(m_movie);
}

void QGifLabel::setFileName(const QString &filename)
{
    m_movie->setFileName(filename);
}

void QGifLabel::play()
{
    m_movie->start();
}

void QGifLabel::stop()
{
    m_movie->stop();
}
