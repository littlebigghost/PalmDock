#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "utils/server.h"
#include <QSystemTrayIcon>

class QTableWidget;
class QLineEdit;
class SwitchButton;
class DockItem;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum ReceiveDataType {
        PullConfig = 0,     // 拉取配置
        Volume = 1,         // 音量
        Brightness = 2,     // 亮度
        ButtonGroup = 3     // 按钮组
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Server *m_server;
    bool m_is_start = false;

    QLineEdit* m_port_line_edit;
    SwitchButton* m_switch_button;
    QTableWidget* m_table_widget;
    QSystemTrayIcon* m_system_tray;
    QList<DockItem *> m_dock_items;
    QString m_config_json;

    QAction* m_start_action;
    QAction* m_stop_action;

private:
    void closeEvent(QCloseEvent *event);
    void startService();
    void stopService();
    void newConnect();
    void handlerData(QString data);
    void initUI();
    void initSysTray();
    void insertRow(DockItem *item, int row = -1);
    void updateRow(DockItem *item, int row);
    void loadConfig();
    void exportJson(QString path, bool isTableConfig = false);
    void importJson(QString path, bool isTableConfig = false);
    void updateAutoStart(bool isAutoStart);

private slots:
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void exportTable();
    void importTable();
};
#endif // MAINWINDOW_H
