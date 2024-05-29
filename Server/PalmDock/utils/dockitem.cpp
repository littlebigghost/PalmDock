#include "dockitem.h"

QStringList DockItem::s_enum_strings  = {
    "文件夹",
    "程序",
    "命令",
    "热键",
    "网页链接"
};

DockItem::DockItem() {}

DockItem::DockItem(const QString &name, DockItem::ItemType type, const QString &path, const QString &iconPath, const QString &iconData, const QString &command) :
    m_name(name),
    m_type(type),
    m_path(path),
    m_icon_path(iconPath),
    m_icon_data(iconData),
    m_command(command)
{}

void DockItem::setDockItem(DockItem* item) {
    m_name = item->name();
    m_type = item->type();
    m_path = item->path();
    m_icon_path = item->icon_path();
    m_icon_data = item->icon_data();
    m_command = item->command();
}

DockItem::ItemType DockItem::type() const
{
    return m_type;
}

void DockItem::setType(ItemType newType)
{
    m_type = newType;
}

QString DockItem::path() const
{
    return m_path;
}

void DockItem::setPath(const QString &newPath)
{
    m_path = newPath;
}

QString DockItem::icon_path() const
{
    return m_icon_path;
}

void DockItem::setIcon_path(const QString &newIconPath)
{
    m_icon_path = newIconPath;
}

QString DockItem::icon_data() const
{
    return m_icon_data;
}

void DockItem::setIcon_data(const QString &newIconData)
{
    m_icon_data = newIconData;
}

QString DockItem::command() const
{
    return m_command;
}

void DockItem::setCommand(const QString &newCommand)
{
    m_command = newCommand;
}

QString DockItem::name() const
{
    return m_name;
}

void DockItem::setName(const QString &newName)
{
    m_name = newName;
}
