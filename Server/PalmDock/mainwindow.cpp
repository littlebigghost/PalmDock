#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QTabBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QButtonGroup>
#include <QStackedWidget>
#include <QIcon>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QFileIconProvider>
#include <QTableWidgetItem>
#include "controls/switchbutton.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "utils/volume.h"
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <windows.h>
#include "controls/additemdialog.h"
#include "utils/dockitem.h"
#include "controls/squarebutton.h"
#include <QMessageBox>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QFileDialog>
#include "utils/hotkeytools.h"
#include "utils/brightness.h"
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
    loadConfig();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI() {
    QButtonGroup* button_group = new QButtonGroup;
    QStackedWidget* stacked_widget = new QStackedWidget;

    QVBoxLayout* btns_layout = new QVBoxLayout;
    QVBoxLayout* main_layout = new QVBoxLayout;

    // 侧边导航栏
    QWidget* btns_widget = new QWidget;
    btns_widget->setStyleSheet("QWidget {background-color: #0F40F5;}"
                              "QPushButton {background-origin: content;"
                              "background-repeat: no-repeat;"
                              "background-position: top;"
                              "padding-top: 10px;"
                              "text-align: bottom;"
                              "padding-bottom: 10px;"
                              "color: white;"
                              "border: none;}"
                              "QPushButton:checked { background-color: #4095E5;}");
    btns_widget->setLayout(btns_layout);

    QPushButton* general_btn = new QPushButton;
    general_btn->setCheckable(true);
    general_btn->setMinimumHeight(120);
    general_btn->setText(tr("常规设置"));
    general_btn->setFont(QFont("宋体", 16));
    general_btn->setStyleSheet("QPushButton {background-image: url(://image/general_settings.png);}");

    QPushButton* interface_btn = new QPushButton;
    interface_btn->setCheckable(true);
    interface_btn->setMinimumHeight(120);
    interface_btn->setText(tr("界面设置"));
    interface_btn->setFont(QFont("宋体", 16));
    interface_btn->setStyleSheet("QPushButton {background-image: url(://image/interface_settings.png);}");

    button_group->addButton(general_btn, 0);
    button_group->addButton(interface_btn, 1);

    for(int i = 0; i < button_group->buttons().count(); i++) {
        btns_layout->addWidget(button_group->button(i));
    }
    btns_layout->addStretch();
    btns_layout->setSpacing(0);
    btns_layout->setContentsMargins(0, 0, 0, 0);

    // 常规设置页面
    QWidget* main_widget = new QWidget;
    main_widget->setLayout(main_layout);

    ui->centralLayout->addWidget(btns_widget);
    ui->centralLayout->addWidget(main_widget);
    ui->centralLayout->setStretchFactor(btns_widget, 1);
    ui->centralLayout->setStretchFactor(main_widget, 5);

    QWidget* tab1 = new QWidget;
    QVBoxLayout* tab1_vertical_layout = new QVBoxLayout;
    QGridLayout* tab1_grid_layout = new QGridLayout;
    tab1_grid_layout->setVerticalSpacing(20);
    tab1_grid_layout->setHorizontalSpacing(20);
    QLabel* port_label = new QLabel("端口");
    port_label->setFont(QFont("宋体", 16));
    QLabel* auto_start_label = new QLabel("开机自启动");
    auto_start_label->setFont(QFont("宋体", 16));
    tab1_grid_layout->addWidget(port_label, 0, 0);
    tab1_grid_layout->addWidget(auto_start_label, 1, 0);
    m_port_line_edit = new QLineEdit;
    m_port_line_edit->setMinimumHeight(40);
    m_port_line_edit->setText("8888");
    m_port_line_edit->setFont(QFont("宋体", 15));
    m_switch_button = new SwitchButton;
    m_switch_button->setShowText(true);
    m_switch_button->setMinimumHeight(30);
    m_switch_button->setMaximumWidth(80);
    tab1_grid_layout->addWidget(m_port_line_edit, 0, 1);
    tab1_grid_layout->addWidget(m_switch_button, 1, 1);
    tab1_vertical_layout->addLayout(tab1_grid_layout);
    tab1_vertical_layout->addStretch();
    tab1->setLayout(tab1_vertical_layout);

    QWidget* tab2 = new QWidget;
    QVBoxLayout* tab2_vertical_layout = new QVBoxLayout;

    // 数据表格
    m_table_widget = new QTableWidget(0, 4);
    m_table_widget->setSelectionBehavior(QAbstractItemView::SelectRows); // 选中一行
    m_table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 所有不可编辑
    m_table_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_table_widget->setHorizontalHeaderLabels(QStringList() << "图标" << "类型" << "名称" << "路径/命令");
    m_table_widget->verticalHeader()->setHidden(true);  // 移除默认行号
    m_table_widget->verticalHeader()->setDefaultSectionSize(50); // 设置行高
    m_table_widget->horizontalHeader()->setMinimumHeight(50); // 设置表头高度
    m_table_widget->horizontalHeader()->setStretchLastSection(true); // 拉伸最后一列
    // m_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 拉伸每一列
    // m_table_widget>setColumnWidth(0, m_table_widget->width()*0.2); // 指定某一列的宽度
    // m_table_widget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 行高自适应

    QFont table_header_font = m_table_widget->horizontalHeader()->font();
    table_header_font.setBold(true);
    table_header_font.setPointSize(16);
    m_table_widget->horizontalHeader()->setFont(table_header_font);

    QWidget* tab2_table_btns_widget = new QWidget;
    tab2_table_btns_widget->setStyleSheet("QPushButton {background-color: #fdfdfd; border-radius:5px; border:1px solid #d2d2d2;font-family:Microsoft Yahei;font-size:16pt;}"
                                    "QPushButton:hover {border:1px solid #0c7ed6; background-color: #e0eef9;}");
    QHBoxLayout* tab2_horizontal_layout = new QHBoxLayout;
    SquareButton* add_item_btn = new SquareButton("+");
    SquareButton* del_item_btn = new SquareButton("−");
    SquareButton* up_item_btn = new SquareButton("△");
    SquareButton* down_item_btn = new SquareButton("▽");
    QPushButton* import_table_btn = new QPushButton;
    QPushButton* export_table_btn = new QPushButton;

    add_item_btn->setMinimumSize(QSize(30, 30));
    del_item_btn->setMinimumSize(QSize(30, 30));
    up_item_btn->setMinimumSize(QSize(30, 30));
    down_item_btn->setMinimumSize(QSize(30, 30));
    import_table_btn->setMinimumSize(QSize(30, 30));
    export_table_btn->setMinimumSize(QSize(30, 30));

    add_item_btn->setCursor(Qt::PointingHandCursor);
    del_item_btn->setCursor(Qt::PointingHandCursor);
    up_item_btn->setCursor(Qt::PointingHandCursor);
    down_item_btn->setCursor(Qt::PointingHandCursor);
    import_table_btn->setCursor(Qt::PointingHandCursor);
    export_table_btn->setCursor(Qt::PointingHandCursor);

    up_item_btn->setToolTip("上移");
    down_item_btn->setToolTip("下移");
    import_table_btn->setToolTip("导入");
    export_table_btn->setToolTip("导出");

    import_table_btn->setIcon(QIcon("://image/table-import.png"));
    export_table_btn->setIcon(QIcon("://image/table-export.png"));
    import_table_btn->setStyleSheet("QPushButton:hover {border:1px solid #0c7ed6; background-color: #e0eef9;} "
                                    "QPushButton {border:none;background-color:transparent;}");
    export_table_btn->setStyleSheet("QPushButton:hover {border:1px solid #0c7ed6; background-color: #e0eef9;} "
                                    "QPushButton {border:none;background-color:transparent;}");
    del_item_btn->setStyleSheet("QPushButton {color:white;background-color: #A4ADB3;}");

    tab2_horizontal_layout->setSpacing(0);
    tab2_horizontal_layout->addWidget(add_item_btn);
    tab2_horizontal_layout->addWidget(del_item_btn);
    tab2_horizontal_layout->addSpacing(10);
    tab2_horizontal_layout->addWidget(up_item_btn);
    tab2_horizontal_layout->addWidget(down_item_btn);
    tab2_horizontal_layout->addStretch();
    tab2_horizontal_layout->addWidget(import_table_btn);
    tab2_horizontal_layout->addSpacing(2);
    tab2_horizontal_layout->addWidget(export_table_btn);
    tab2_table_btns_widget->setLayout(tab2_horizontal_layout);

    tab2_vertical_layout->addWidget(m_table_widget);
    tab2_vertical_layout->addWidget(tab2_table_btns_widget);
    tab2->setLayout(tab2_vertical_layout);

    stacked_widget->addWidget(tab1);
    stacked_widget->addWidget(tab2);


    QWidget* main_btns_widget = new QWidget;
    main_btns_widget->setStyleSheet("QPushButton {background-color: #FFFFFF; border-radius:5px; border:1px solid #d2d2d2;font-family:Microsoft Yahei;font-size:16pt; padding: 5px 30px;}"
                                    "QPushButton:hover {border:1px solid #0c7ed6; background-color: #e0eef9;}");
    QHBoxLayout* main_btns_layout = new QHBoxLayout;

    QPushButton* confirm_btn = new QPushButton(tr("确认"));
    QPushButton* cancel_btn = new QPushButton(tr("取消"));
    QPushButton* apply_btn = new QPushButton(tr("应用"));
    confirm_btn->setStyleSheet("QPushButton {background-color: #DE868F; color: white;}");
    apply_btn->setStyleSheet("QPushButton {background-color: #0F40F5; color: white;}");

    main_btns_layout->addStretch();
    main_btns_layout->addWidget(confirm_btn);
    main_btns_layout->addWidget(cancel_btn);
    main_btns_layout->addWidget(apply_btn);
    main_btns_widget->setLayout(main_btns_layout);

    main_layout->addWidget(stacked_widget);
    main_layout->addWidget(main_btns_widget);
    main_layout->setStretchFactor(stacked_widget, 5);
    main_layout->setStretchFactor(main_btns_widget, 1);

    general_btn->setChecked(true);

    // 检查自动启动
    connect(m_switch_button, &SwitchButton::checkedChanged, this, [=](bool isChecked){
        updateAutoStart(isChecked);
    });
    // 表格项双击
    connect(m_table_widget, &QTableWidget::itemDoubleClicked, this, [=](){
        int current_row = m_table_widget->currentRow();
        AddItemDialog addItemDialog(m_dock_items.at(current_row));
        connect(&addItemDialog, &AddItemDialog::accept, this, [=](DockItem *newDock_item) {
            updateRow(newDock_item, current_row);
        });
        addItemDialog.exec();
    });
    // 添加
    connect(add_item_btn, &QPushButton::clicked, this, [=](){
        AddItemDialog add_item_dialog;
        add_item_dialog.setTitle("添加子项");
        connect(&add_item_dialog, &AddItemDialog::accept, this, [=](DockItem *newDock_item) {
            m_dock_items.append(newDock_item);
            insertRow(newDock_item);
        });
        add_item_dialog.exec();
    });
    // 删除
    connect(del_item_btn, &QPushButton::clicked, this, [=](){
        //弹出提示框，看是否删除数据
        QMessageBox message_box(QMessageBox::NoIcon, tr("提示"),
                                tr("是否删除本行数据?"));
        QPushButton* ok_btn = message_box.addButton(tr("确认"), QMessageBox::AcceptRole);
        message_box.addButton(tr("取消"), QMessageBox::RejectRole);

        message_box.exec();

        //如确认删除
        if(message_box.clickedButton() == ok_btn)
        {
            int current_row = m_table_widget->currentRow();
            m_table_widget->removeRow(current_row);
            m_dock_items.removeAt(current_row);
        }
    });
    // 上移
    connect(up_item_btn, &QPushButton::clicked, this, [=](){
        int current_row = m_table_widget->currentRow();
        int last_row = current_row - 1;
        if (current_row > 0 && current_row < m_table_widget->rowCount()) {
            DockItem* before_item = m_dock_items.at(current_row);
            m_dock_items.removeAt(current_row);
            m_table_widget->removeRow(current_row);
            m_table_widget->insertRow(last_row);
            insertRow(before_item, last_row);
            m_dock_items.insert(last_row, before_item);

            m_table_widget->selectRow(last_row);
        }
    });
    // 下移
    connect(down_item_btn, &QPushButton::clicked, this, [=](){
        int current_row = m_table_widget->currentRow();
        int after_row = current_row + 1;
        if (current_row < m_table_widget->rowCount() - 1) {
            DockItem* before_item = m_dock_items.at(current_row);
            m_dock_items.removeAt(current_row);
            m_table_widget->removeRow(current_row);
            m_table_widget->insertRow(after_row);
            insertRow(before_item, after_row);
            m_dock_items.insert(after_row, before_item);

            m_table_widget->selectRow(after_row);
        }
    });
    // 导入
    connect(import_table_btn, &QPushButton::clicked, this, &MainWindow::importTable);
    // 导出
    connect(export_table_btn, &QPushButton::clicked, this, &MainWindow::exportTable);
    // 按钮组单击
    connect(button_group, &QButtonGroup::buttonClicked, stacked_widget, [=](QAbstractButton* button) {
        stacked_widget->setCurrentIndex(button_group->id(button));
    });
    // 确认
    connect(confirm_btn, &QPushButton::clicked, this, [=]() {
        exportJson("config.json");
    });
    // 取消
    connect(cancel_btn, &QPushButton::clicked, this, [=]() {
        this->close();
    });
    // 应用
    connect(apply_btn, &QPushButton::clicked, this, [=]() {
        exportJson("config.json");
        if (m_server != nullptr) {
            if (m_server->getPort() != m_port_line_edit->text().toInt()) {
                stopService();
                startService();
            } else {
                m_server->sendData(m_config_json);
            }
        }
    });

    initSysTray();
}

void MainWindow::initSysTray() {
    // 设置系统托盘
    m_system_tray = new QSystemTrayIcon(this);
    m_system_tray->setIcon(QIcon("://image/PalmDock-256.png"));
    m_system_tray->setToolTip(tr("PalmDock"));

    QAction* option_action = new QAction("选项", this);
    m_start_action = new QAction("启动服务", this);
    m_stop_action = new QAction("停止服务", this);
    m_stop_action->setDisabled(true);
    QAction* close_action = new QAction("退出", this);

    QMenu* menu = new QMenu;
    menu->addAction(option_action);
    menu->addSeparator();
    menu->addAction(m_start_action);
    menu->addAction(m_stop_action);
    menu->addSeparator();
    menu->addAction(close_action);

    m_system_tray->setContextMenu(menu);
    m_system_tray->show();

    connect(option_action, &QAction::triggered, this, &MainWindow::showNormal);
    connect(m_start_action, &QAction::triggered, this, &MainWindow::startService);
    connect(m_stop_action, &QAction::triggered, this, &MainWindow::stopService);
    connect(close_action, &QAction::triggered, qApp, &QApplication::quit);
    connect(m_system_tray, &QSystemTrayIcon::activated, this, &MainWindow::on_activatedSysTrayIcon);
}

void MainWindow::loadConfig() {
    QString path = "config.json";
    QFileInfo file(path);
    if (file.isFile()) {
        importJson(path);
    }
}

void MainWindow::startService() {
    m_server = new Server();
    connect(m_server, &Server::newConnect, this, &MainWindow::newConnect);
    connect(m_server, &Server::receiveData, this, &MainWindow::handlerData);
    if (!m_server->isStart()) {
        m_is_start = true;
        m_server->initServer(m_port_line_edit->text().toInt());
        m_server->startServer();
        m_start_action->setDisabled(true);
        m_stop_action->setDisabled(false);
    }
}

void MainWindow::stopService() {
    if (m_server->isStart()) {
        m_is_start = false;
        m_server->stopServer();
        m_start_action->setDisabled(false);
        m_stop_action->setDisabled(true);
    }
}

void MainWindow::insertRow(DockItem *item, int row) {
    QPixmap pixmap;
    QLabel* icon_label = new QLabel;
    QTableWidgetItem* type_table_item = new QTableWidgetItem(DockItem::s_enum_strings.at(item->type()));
    QTableWidgetItem* name_table_item = new QTableWidgetItem(item->name());

    pixmap.loadFromData(QByteArray::fromBase64(item->icon_data().toUtf8()));
    pixmap = pixmap.scaled(24,24);
    icon_label->setPixmap(pixmap);
    icon_label->setAlignment(Qt::AlignCenter);
    type_table_item->setTextAlignment(Qt::AlignCenter);
    name_table_item->setTextAlignment(Qt::AlignCenter);

    int current_row;
    if (row == -1) {
        current_row = m_table_widget->rowCount();
        m_table_widget->insertRow(m_table_widget->rowCount());
    } else {
        current_row = row;
    }
    m_table_widget->setCellWidget(current_row, 0, icon_label);
    m_table_widget->setItem(current_row, 1, type_table_item);
    m_table_widget->setItem(current_row, 2, name_table_item);

    switch (item->type()) {
    case DockItem::Dir:
    case DockItem::App:
        m_table_widget->setItem(current_row, 3, new QTableWidgetItem(item->path()));
        break;
    case DockItem::CMD:
    case DockItem::HotKey:
    case DockItem::WebLink:
        m_table_widget->setItem(current_row, 3, new QTableWidgetItem(item->command()));
        break;
    default:
        break;
    }
}

void MainWindow::updateRow(DockItem *item, int row){
    m_dock_items.removeAt(row);
    m_table_widget->removeRow(row);
    m_table_widget->insertRow(row);
    insertRow(item, row);
    m_dock_items.insert(row, item);
}

void MainWindow::exportTable() {
    QVariantList variantList;
    for (const DockItem *item : m_dock_items) {
        QVariantMap map;
        map["name"] = item->name();
        map["type"] = DockItem::s_enum_strings.at(item->type());
        map["path"] = item->path();
        map["icon_path"] = item->icon_path();
        map["icon_data"] = item->icon_data();
        map["command"] = item->command();
        variantList.append(map);
    }
    QJsonDocument doc = QJsonDocument::fromVariant(variantList);

    QString path = QFileDialog::getExistingDirectory(nullptr, tr("选择目录"), "", QFileDialog::ShowDirsOnly);
    exportJson(path + "/config.json", true);
    QMessageBox message_box;
    message_box.setText("导出成功");
    message_box.exec();
}

void MainWindow::importTable() {
    QString path = QFileDialog::getOpenFileName(nullptr, tr("选择文件"), "", "JSON 文件(*.json)");
    importJson(path, true);
    QMessageBox message_box;
    message_box.setText("导入成功");
    message_box.exec();
}

void MainWindow::exportJson(QString path, bool isTableConfig) {
    QVariantList variantList;
    for (const DockItem *item : m_dock_items) {
        QVariantMap map;
        map["name"] = item->name();
        map["type"] = DockItem::s_enum_strings.at(item->type());
        map["path"] = item->path();
        map["icon_path"] = item->icon_path();
        map["icon_data"] = item->icon_data();
        map["command"] = item->command();
        variantList.append(map);
    }
    QJsonArray array = QJsonArray::fromVariantList(variantList);
    QJsonObject root;
    root.insert("port", m_port_line_edit->text());
    root.insert("is_auto_start", m_switch_button->getChecked());
    root.insert("is_start_server", m_is_start);
    root.insert("table_config", array);

    QJsonDocument doc;
    doc.setArray(array);
    m_config_json = doc.toJson(QJsonDocument::Compact);
    // 如果输出是完整的配置文件
    if (!isTableConfig) {
        doc.setObject(root);
    }

    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << doc.toJson(QJsonDocument::Compact);
        file.close();
    }
}

void MainWindow::importJson(QString path, bool isTableConfig) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file";
        return;
    }

    QByteArray file_data = file.readAll();
    file.close();

    QJsonDocument doc(QJsonDocument::fromJson(file_data));
    if (doc.isNull()) {
        qWarning() << "Failed to create JOSN doc.";
        return;
    }
    if (!isTableConfig) {
        QJsonObject root = doc.object();
        m_port_line_edit->setText(root["port"].toString());
        m_switch_button->setChecked(root["is_auto_start"].toBool());
        if (root["is_start_server"].toBool())
            startService();
        doc.setArray(root["table_config"].toArray());
    }
    m_config_json = doc.toJson(QJsonDocument::Compact);
    m_table_widget->setRowCount(0);
    m_dock_items.clear();
    QJsonArray array = doc.array();
    for (const QJsonValue &value : array) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            DockItem* item = new DockItem(
                obj["name"].toString(),
                DockItem::ItemType(DockItem::s_enum_strings.indexOf(obj["type"].toString())),
                obj["path"].toString(),
                obj["icon_path"].toString(),
                obj["icon_data"].toString(),
                obj["command"].toString());
            insertRow(item);
            m_dock_items << item;
        }
    }
}

void MainWindow::updateAutoStart(bool isAutoStart) {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (isAutoStart) {
        qDebug() << " true ";
        if (!settings.contains("PalmDock")) {
            settings.setValue("PalmDock", QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
        }
    } else {
        qDebug() << " false ";
        if (settings.contains("PalmDock")) {
            settings.remove("PalmDock");
        }
    }
}

void MainWindow::newConnect() {
    // m_server->sendData(m_config_json);
}

void MainWindow::handlerData(QString data) {
    qDebug() << data;

    QJsonParseError json_error;
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8(), &json_error);
    if (json_error.error == QJsonParseError::NoError) {
        QJsonObject obj = doc.object();
        ReceiveDataType type = ReceiveDataType(obj["type"].toInt());
        int value = obj["value"].toInt();
        switch (type) {
        case PullConfig:
            // 发送配置到客户端
            m_server->sendData(m_config_json);
            break;
        case Volume:
            // 设置音量大小
            Volume::setSysVolume(value);
            break;
        case Brightness:
            // 设置屏幕亮度
            Brightness::setBrightness(value);
            break;
        case ButtonGroup:
            qDebug() << "ButtonGroup"; {
                DockItem* item = m_dock_items.at(value);
                switch (item->type()) {
                case DockItem::Dir:     // 打开文件夹
                {
                    QString folder_prefix = "file:///";
                    QDesktopServices::openUrl(QUrl(folder_prefix + item->path()));
                    break;
                }
                case DockItem::App:     // 打开程序
                {
                    QProcess process;
                    process.startDetached(item->path());
                    break;
                }
                case DockItem::CMD:     // 打开命令
                {
                    QProcess process;
                    process.setProgram("cmd");
                    QStringList argument;
                    argument << "/c" << item->command();
                    process.setArguments(argument);
                    process.startDetached();
                    break;
                }
                case DockItem::HotKey:  // 调用快捷键
                {
                    HotKeyTools::sendKeyCombination(item->command());
                    break;
                }
                case DockItem::WebLink: // 打开 web 链接
                    QDesktopServices::openUrl(QUrl(item->command()));
                    break;
                default:
                    break;
                }

            }
            break;
        default:
            break;
        }
    }
}

void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
        this->show();
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (m_system_tray->isVisible()) {
        this->hide();
        event->ignore();
    } else {
        event->accept();
    }
}
