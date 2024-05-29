#ifndef PICTURETOOLS_H
#define PICTURETOOLS_H

#include <QObject>

class QPixmap;
class PictureTools : public QObject
{
    Q_OBJECT
public:
    explicit PictureTools(QObject *parent = nullptr);

public:
    static QPixmap appPathToPixmap(QString path, int width = 24, int height = 24);

signals:
};

#endif // PICTURETOOLS_H
