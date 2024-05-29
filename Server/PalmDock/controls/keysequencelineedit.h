#ifndef KEYSEQUENCELINEEDIT_H
#define KEYSEQUENCELINEEDIT_H

#include <QLineEdit>
#include <Windows.h>

class KeySequenceLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    KeySequenceLineEdit(QWidget* parent = nullptr);
    ~KeySequenceLineEdit();

    static void setHook();
    static void removeHook();
    static void setKeyboardListening(bool isListening);

protected:
    virtual void focusInEvent(QFocusEvent *e) override;
    virtual void focusOutEvent(QFocusEvent *e) override;

private:
    static bool s_is_focus;
    static bool s_is_listening;
    static HHOOK s_keyboard_hook;
    static KeySequenceLineEdit *s_instance;

    QSet<int> m_pressed_keys;

    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    void processKeyEvent(int key, bool pressed);
    bool isNumberOrCharacter(int key);
    QString keySequenceToString();
};

#endif // KEYSEQUENCELINEEDIT_H
