#ifndef HOTKEYTOOLS_H
#define HOTKEYTOOLS_

#include <QMap>

typedef unsigned short      WORD;
class QString;

class HotKeyTools
{
public:
    explicit HotKeyTools();

public:
    static bool sendKeyCombination(const QString &combination);
    static const QMap<WORD, QString>& getValueToKeyMap();
};

#endif // HOTKEYTOOLS_H
