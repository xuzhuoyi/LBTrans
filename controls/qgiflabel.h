#ifndef QGIFLABEL_H
#define QGIFLABEL_H

#include <QLabel>
class QMovie;

class QGifLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QGifLabel(QWidget *parent = 0);
    explicit QGifLabel(const QString &filename , QWidget *parent = 0);

    void setFileName(const QString &filename);

    void play();
    void stop();

signals:

public slots:

private:
    QMovie *m_movie;

};

#endif // QGIFLABEL_H
