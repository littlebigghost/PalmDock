#include "brightness.h"
#include <QProcess>
#include <QDebug>

QString Brightness::runWmiCommand(const QString &command) {
    QProcess process;
    process.start("powershell", QStringList() << "-Command" << command);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    return output.trimmed();
}

int Brightness::getBrightness() {
    QString command = "(Get-WmiObject -Namespace root/WMI -Class WmiMonitorBrightness).CurrentBrightness";
    QString output = runWmiCommand(command);

    bool ok;
    int brightness = output.toInt(&ok);
    if (ok) {
        return brightness;
    }
    return -1; // 表示获取失败
}

bool Brightness::setBrightness(int brightness) {
    if (brightness < 0 || brightness > 100) {
        return false; // 亮度值必须在 0 到 100 之间
    }
    QString command = QString("(Get-WmiObject -Namespace root/WMI -Class WmiMonitorBrightnessMethods).WmiSetBrightness(1,%1)").arg(brightness);
    QString output = runWmiCommand(command);
    return output.isEmpty(); // PowerShell 命令成功执行时通常没有输出
}
