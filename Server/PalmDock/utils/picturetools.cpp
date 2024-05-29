#include "picturetools.h"
#include <QPixmap>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QIcon>

PictureTools::PictureTools(QObject *parent)
    : QObject{parent}
{}

QPixmap PictureTools::appPathToPixmap(QString path, int width, int height) {
    QFileIconProvider iconProvider;
    QIcon icon = iconProvider.icon(QFileInfo(path));
    return QPixmap(icon.pixmap(width, height));
}
