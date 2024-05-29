#include "keysequencelineedit.h"
#include <QDebug>
#include "../utils/hotkeytools.h"

HHOOK KeySequenceLineEdit::s_keyboard_hook = nullptr;
KeySequenceLineEdit* KeySequenceLineEdit::s_instance = nullptr;
bool KeySequenceLineEdit::s_is_listening = false;
bool KeySequenceLineEdit::s_is_focus = false;

KeySequenceLineEdit::KeySequenceLineEdit(QWidget* parent) : QLineEdit(parent) {
    s_instance = this;
    setHook();
}

KeySequenceLineEdit::~KeySequenceLineEdit() {
    removeHook();
    s_instance = nullptr;
}

void KeySequenceLineEdit::setKeyboardListening(bool isListening) {
    s_is_listening = isListening;
}

void KeySequenceLineEdit::setHook() {
    if (!s_keyboard_hook) {
        s_keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeySequenceLineEdit::KeyboardProc, nullptr, 0);
        if (!s_keyboard_hook) qWarning() << "Failed to install keyboard hook!";
    }
}

void KeySequenceLineEdit::removeHook() {
    if (s_keyboard_hook) {
        UnhookWindowsHookEx(s_keyboard_hook);
        s_keyboard_hook= nullptr;
    }
}

LRESULT CALLBACK KeySequenceLineEdit::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0) {
        KBDLLHOOKSTRUCT* kbdStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        int key = kbdStruct->vkCode;
        bool pressed = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
        if (s_instance && s_is_listening && s_is_focus)
            s_instance->processKeyEvent(key, pressed);
    }
    return CallNextHookEx(s_keyboard_hook, nCode, wParam, lParam);
}

void KeySequenceLineEdit::processKeyEvent(int key, bool pressed) {
    if (pressed) {
        m_pressed_keys.insert(key);
        setText(keySequenceToString());
    } else
        m_pressed_keys.remove(key);
}

QString KeySequenceLineEdit::keySequenceToString(){
    QStringList keyNames;
    for (int key : m_pressed_keys) {
        if (isNumberOrCharacter(key))
            keyNames << HotKeyTools::getValueToKeyMap()[key];
        else
            keyNames.insert(0, HotKeyTools::getValueToKeyMap()[key]);
    }
    return keyNames.join("+");
}

bool KeySequenceLineEdit::isNumberOrCharacter(int key) {
    if ((key >= 0x1B && key <= 0x5A) ||(key >= 0x60 && key <= 0x87) ||(key >= 0xBA && key <= 0xDE))
        return true;
    else
        return false;
}

void KeySequenceLineEdit::focusInEvent(QFocusEvent *e) {
    s_is_focus = true;
    if (s_is_listening)
        setReadOnly(true);
    QLineEdit::focusInEvent(e);
}

void KeySequenceLineEdit::focusOutEvent(QFocusEvent *e) {
    s_is_focus = false;
    if (s_is_listening)
        setReadOnly(false);
    QLineEdit::focusOutEvent(e);
}
