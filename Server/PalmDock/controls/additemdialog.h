#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include "../utils/dockitem.h"

class QComboBox;
class QLineEdit;
class QLabel;
class QPushButton;
class KeySequenceLineEdit;

class AddItemDialog : public QDialog
{
    Q_OBJECT
public:
    AddItemDialog();
    AddItemDialog(DockItem *dockItem);

    DockItem *dockItem() const;
    void setDockItem(DockItem *dockItem);

    void setTitle(QString title);

private:
    DockItem* m_dock_item;

    QComboBox* m_type_combo_box;
    QLineEdit* m_name_line_edit;
    KeySequenceLineEdit* m_params_line_edit;
    QLineEdit* m_file_path_line_edit;
    QLineEdit* m_icon_path_line_edit;
    QLabel* m_icon_label;
    QPushButton* m_file_path_selector_button;

private:
    void initUI();
    void initData();
    void changeUIEnable(QString paramsPlaceholderText, bool showParamsEdit, bool showFilePath, bool isListening = false);
    void accept_btn_clicked(bool checked = false);

signals:
    void accept(DockItem* item);
};

#endif // ADDITEMDIALOG_H
