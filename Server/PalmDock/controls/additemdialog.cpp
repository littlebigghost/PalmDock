#include "additemdialog.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QBuffer>
#include <QFileDialog>
#include "../utils/picturetools.h"
#include <QKeyEvent>
#include <QKeySequence>
#include "../controls/keysequencelineedit.h"

AddItemDialog::AddItemDialog() {
    m_dock_item = new DockItem();
    initUI();
}

AddItemDialog::AddItemDialog(DockItem *dockItem) : m_dock_item(dockItem)
{
    initUI();
    initData();
}
void AddItemDialog::setTitle(QString title) {
    setWindowTitle(title);
}

void AddItemDialog::initUI() {
    setStyleSheet("QDialog {background-color: white;}"
                  "QPushButton {border-radius:5px; border:1px solid #d2d2d2;font-family:Microsoft Yahei;font-size:12pt;}"
                  "QPushButton:hover {border:1px solid #0c7ed6; background-color: #e0eef9;}");
    QGridLayout* mainGridLayout = new QGridLayout;
    mainGridLayout->setContentsMargins(20,20,20,20);
    mainGridLayout->setColumnMinimumWidth(0, 100);
    mainGridLayout->setColumnMinimumWidth(1, 200);
    mainGridLayout->setColumnMinimumWidth(2, 20);
    mainGridLayout->setRowMinimumHeight(5, 50);

    mainGridLayout->addWidget(new QLabel("类型:"), 0, 0);
    mainGridLayout->addWidget(new QLabel("名称:"), 1, 0);
    mainGridLayout->addWidget(new QLabel("参数:"), 2, 0);
    mainGridLayout->addWidget(new QLabel("路径:"), 3, 0);
    mainGridLayout->addWidget(new QLabel("图标路径:"), 4, 0);

    // 类型下拉选择框
    m_type_combo_box = new QComboBox;
    m_type_combo_box->addItems(QStringList() << "文件夹" << "应用程序" << "命令" << "快捷键" << "网页链接");
    // 名称输入框
    m_name_line_edit = new QLineEdit;
    // 参数输入框
    m_params_line_edit = new KeySequenceLineEdit;
    // 文件路径
    m_file_path_line_edit = new QLineEdit;
    // 图标路径
    m_icon_path_line_edit = new QLineEdit;

    // 图片
    m_icon_label = new QLabel;

    mainGridLayout->addWidget(m_type_combo_box, 0, 1);
    mainGridLayout->addWidget(m_name_line_edit, 1, 1);
    mainGridLayout->addWidget(m_params_line_edit, 2, 1);
    mainGridLayout->addWidget(m_file_path_line_edit, 3, 1);
    mainGridLayout->addWidget(m_icon_path_line_edit, 4, 1);
    mainGridLayout->addWidget(m_icon_label, 5, 1);

    m_file_path_selector_button = new QPushButton("...");
    QPushButton* iconPathSelectorButton = new QPushButton("...");

    m_params_line_edit->setEnabled(false);

    connect(m_type_combo_box, &QComboBox::currentIndexChanged, this, [=](int index){
        m_params_line_edit->setText("");
        switch (index) {
        case 0:
        case 1:
            changeUIEnable("", false, true);
            break;
        case 2:
            changeUIEnable("请输入命令，例 calc", true, false);
            break;
        case 3:
            changeUIEnable("请按下快捷键", true, false, true);
            break;
        case 4:
            changeUIEnable("请输入链接，例 https://www.baidu.com", true, false);
            break;
        default:
            break;
        }
    });

    // 文件/文件夹选择
    connect(m_file_path_selector_button, &QPushButton::clicked, this, [=](){
        QString path;
        if (m_type_combo_box->currentIndex() == 0) {
            path = QFileDialog::getExistingDirectory(nullptr, tr("选择目录"),"", QFileDialog::ShowDirsOnly);
        } else {
            path = QFileDialog::getOpenFileName(nullptr, tr("选择文件"), "", "应用程序(*.exe);;文件(*.*)");
        }

        if (path.isEmpty())
            return;

        m_file_path_line_edit->setText(path);

        QFileInfo fileInfo(path);
        QString name;
        name = fileInfo.baseName();

        m_name_line_edit->setText(name);

        QPixmap pixmap = PictureTools::appPathToPixmap(path);
        pixmap = pixmap.scaled(128, 128, Qt::IgnoreAspectRatio);
        m_icon_label->setPixmap(pixmap);
    });
    // 图标路径选择
    connect(iconPathSelectorButton, &QPushButton::clicked, this, [=](){
        QString filePath = QFileDialog::getOpenFileName(nullptr, tr("文件"), "", "图片(*.png *.jpg *.jpeg *.bmp)");
        m_icon_path_line_edit->setText(filePath);
        QPixmap pixmap;
        pixmap.load(filePath);
        pixmap = pixmap.scaled(128, 128, Qt::IgnoreAspectRatio);
        m_icon_label->setPixmap(pixmap);
    });

    mainGridLayout->addWidget(m_file_path_selector_button, 3, 2);
    mainGridLayout->addWidget(iconPathSelectorButton, 4, 2);


    QPushButton* confirmButton = new QPushButton(tr("确定"));
    QPushButton* cancelButton = new QPushButton(tr("取消"));
    confirmButton->setStyleSheet("QPushButton {color:white; background-color: #0F40F5;}");

    connect(confirmButton, &QPushButton::clicked, this, &AddItemDialog::accept_btn_clicked);
    connect(cancelButton, &QPushButton::clicked, this, [=](){
        this->close();
    });

    QHBoxLayout* buttonsHLayout = new QHBoxLayout;
    buttonsHLayout->setAlignment(Qt::AlignRight);
    buttonsHLayout->addSpacing(50);
    buttonsHLayout->addWidget(confirmButton);
    buttonsHLayout->addWidget(cancelButton);

    mainGridLayout->addLayout(buttonsHLayout, 6, 1, 1, 2);

    setLayout(mainGridLayout);
}

void AddItemDialog::initData() {
    m_type_combo_box->setCurrentIndex(m_dock_item->type());
    if (!m_dock_item->name().isEmpty())
        m_name_line_edit->setText(m_dock_item->name());
    if (!m_dock_item->command().isEmpty())
        m_params_line_edit->setText(m_dock_item->command());
    if (!m_dock_item->path().isEmpty())
        m_file_path_line_edit->setText(m_dock_item->path());
    if (!m_dock_item->icon_path().isEmpty())
        m_icon_path_line_edit->setText(m_dock_item->icon_path());
    if (!m_dock_item->icon_data().isEmpty()) {
        QPixmap pixmap;
        pixmap.loadFromData(QByteArray::fromBase64(m_dock_item->icon_data().toUtf8()));
        pixmap = pixmap.scaled(128, 128);
        m_icon_label->setPixmap(pixmap);
    }
}

void AddItemDialog::changeUIEnable(QString paramsPlaceholderText, bool showParamsEdit, bool showFilePath, bool isListening) {
    m_params_line_edit->setPlaceholderText(paramsPlaceholderText);
    m_params_line_edit->setEnabled(showParamsEdit);
    m_file_path_line_edit->setEnabled(showFilePath);
    m_file_path_selector_button->setEnabled(showFilePath);
    KeySequenceLineEdit::setKeyboardListening(isListening);
}

void AddItemDialog::accept_btn_clicked(bool checked){
    if (!m_name_line_edit->text().isEmpty())
        m_dock_item->setName(m_name_line_edit->text());
    if (!m_params_line_edit->text().isEmpty())
        m_dock_item->setCommand(m_params_line_edit->text());
    if (!m_file_path_line_edit->text().isEmpty())
        m_dock_item->setPath(m_file_path_line_edit->text());
    if (!m_icon_path_line_edit->text().isEmpty())
        m_dock_item->setIcon_path(m_icon_path_line_edit->text());
    if (!m_icon_label->pixmap().isNull()){
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        m_icon_label->pixmap().save(&buffer, "PNG");
        m_dock_item->setIcon_data(QString(byteArray.toBase64()));
    }

    m_dock_item->setType(DockItem::ItemType(m_type_combo_box->currentIndex()));

    emit accept(m_dock_item);
    this->close();
}

DockItem *AddItemDialog::dockItem() const
{
    return m_dock_item;
}

void AddItemDialog::setDockItem(DockItem *dockItem)
{
    m_dock_item = dockItem;
}
