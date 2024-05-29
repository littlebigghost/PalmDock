#ifndef DOCKITEM_H
#define DOCKITEM_H

#include <QString>
#include <QStringList>

class DockItem
{
public:
    enum ItemType {
        Dir = 0,
        App = 1,
        CMD = 2,
        HotKey = 3,
        WebLink = 4
    };
    static QStringList s_enum_strings;

public:
    DockItem();
    DockItem(const QString &name, ItemType type, const QString &path, const QString &iconPath, const QString &iconData, const QString &command);

    void setDockItem(DockItem* item);

private:
    QString m_name;
    ItemType m_type;
    QString m_path;
    QString m_icon_path;
    QString m_icon_data;
    QString m_command;

public:
    QString name() const;
    void setName(const QString &newName);
    ItemType type() const;
    void setType(ItemType newType);
    QString path() const;
    void setPath(const QString &newPath);
    QString icon_path() const;
    void setIcon_path(const QString &newIconPath);
    QString icon_data() const;
    void setIcon_data(const QString &newIconData);
    QString command() const;
    void setCommand(const QString &newCommand);
};

#endif // DOCKITEM_H
