#include "hotkeytools.h"
#include <QString>
#include <QDebug>
#include <Windows.h>

QMap<QString, WORD> mKeyMap = {
    {"lctrl", VK_LCONTROL},
    {"lshift", VK_LSHIFT},
    {"lalt", VK_LMENU},
    {"lwin", VK_LWIN},
    {"rctrl", VK_RCONTROL},
    {"rshift", VK_RSHIFT},
    {"ralt", VK_RMENU},
    {"rwin", VK_RWIN},
    {"caps", VK_CAPITAL},
    {"tab", VK_TAB},
    {";", VK_OEM_1},
    {"/", VK_OEM_2},
    {"`", VK_OEM_3},
    {"[", VK_OEM_4},
    {"\\", VK_OEM_5},
    {"]", VK_OEM_6},
    {"'", VK_OEM_7},
    {"esc", VK_ESCAPE},
    {"back", VK_BACK},
    {"enter", VK_RETURN},
    {"space", VK_SPACE},
    {"del", VK_DELETE},
    {"insert", VK_INSERT},
    {"prior", VK_PRIOR},
    {"next", VK_NEXT},
    {"end", VK_END},
    {"home", VK_HOME},
    {"left", VK_LEFT},
    {"up", VK_UP},
    {"right", VK_RIGHT},
    {"down", VK_DOWN},
    {"f1", VK_F1},
    {"f2", VK_F2},
    {"f3", VK_F3},
    {"f4", VK_F4},
    {"f5", VK_F5},
    {"f6", VK_F6},
    {"f7", VK_F7},
    {"f8", VK_F8},
    {"f9", VK_F9},
    {"f10", VK_F10},
    {"f11", VK_F11},
    {"f12", VK_F12},
    {"numlock", VK_NUMLOCK},
    {"n0", VK_NUMPAD0},
    {"n1", VK_NUMPAD1},
    {"n2", VK_NUMPAD2},
    {"n3", VK_NUMPAD3},
    {"n4", VK_NUMPAD4},
    {"n5", VK_NUMPAD5},
    {"n6", VK_NUMPAD6},
    {"n7", VK_NUMPAD7},
    {"n8", VK_NUMPAD8},
    {"n9", VK_NUMPAD9},
    {"n/", VK_DIVIDE},
    {"n*", VK_MULTIPLY},
    {"n-", VK_SUBTRACT},
    {"n+", VK_ADD},
    {"n.", VK_DECIMAL},
    {"nenter", VK_SEPARATOR},
    {"0", 0x30},
    {"1", 0x31},
    {"2", 0x32},
    {"3", 0x33},
    {"4", 0x34},
    {"5", 0x35},
    {"6", 0x36},
    {"7", 0x37},
    {"8", 0x38},
    {"9", 0x39},
    {"a", 0x41},
    {"b", 0x42},
    {"c", 0x43},
    {"d", 0x44},
    {"e", 0x45},
    {"f", 0x46},
    {"g", 0x47},
    {"h", 0x48},
    {"i", 0x49},
    {"j", 0x4A},
    {"k", 0x4B},
    {"l", 0x4C},
    {"m", 0x4D},
    {"n", 0x4E},
    {"o", 0x4F},
    {"p", 0x50},
    {"q", 0x51},
    {"r", 0x52},
    {"s", 0x53},
    {"t", 0x54},
    {"u", 0x55},
    {"v", 0x56},
    {"w", 0x57},
    {"x", 0x58},
    {"y", 0x59},
    {"z", 0x5A}
};

QMap<WORD, QString> mValueMap = {
    {VK_LCONTROL, "lCtrl"},
    {VK_LSHIFT, "lShift"},
    {VK_LMENU, "lAlt"},
    {VK_LWIN, "lWin"},
    {VK_RCONTROL, "rCtrl"},
    {VK_RSHIFT, "rShift"},
    {VK_RMENU, "rAlt"},
    {VK_RWIN, "rWin"},
    {VK_CAPITAL, "Caps"},
    {VK_TAB, "Tab"},
    {VK_OEM_1, ";"},
    {VK_OEM_2, "/"},
    {VK_OEM_3, "`"},
    {VK_OEM_4, "["},
    {VK_OEM_5, "\\"},
    {VK_OEM_6, "]"},
    {VK_OEM_7, "'"},
    {VK_ESCAPE, "Esc"},
    {VK_BACK, "Back"},
    {VK_RETURN, "Enter"},
    {VK_SPACE, "Space"},
    {VK_DELETE, "Del"},
    {VK_INSERT, "Insert"},
    {VK_PRIOR, "Prior"},
    {VK_NEXT, "Next"},
    {VK_END, "End"},
    {VK_HOME, "Home"},
    {VK_LEFT, "Left"},
    {VK_UP, "Up"},
    {VK_RIGHT, "Right"},
    {VK_DOWN, "Down"},
    {VK_F1, "F1"},
    {VK_F2, "F2"},
    {VK_F3, "F3"},
    {VK_F4, "F4"},
    {VK_F5, "F5"},
    {VK_F6, "F6"},
    {VK_F7, "F7"},
    {VK_F8, "F8"},
    {VK_F9, "F9"},
    {VK_F10, "F10"},
    {VK_F11, "F11"},
    {VK_F12, "F12"},
    {VK_NUMLOCK, "Numlock"},
    {VK_NUMPAD0, "N0"},
    {VK_NUMPAD1, "N1"},
    {VK_NUMPAD2, "N2"},
    {VK_NUMPAD3, "N3"},
    {VK_NUMPAD4, "N4"},
    {VK_NUMPAD5, "N5"},
    {VK_NUMPAD6, "N6"},
    {VK_NUMPAD7, "N7"},
    {VK_NUMPAD8, "N8"},
    {VK_NUMPAD9, "N9"},
    {VK_DIVIDE, "N/"},
    {VK_MULTIPLY, "N*"},
    {VK_SUBTRACT, "N-"},
    {VK_ADD, "N+"},
    {VK_DECIMAL, "N."},
    {VK_SEPARATOR, "NEnter"},
    {0x30, "0"},
    {0x31, "1"},
    {0x32, "2"},
    {0x33, "3"},
    {0x34, "4"},
    {0x35, "5"},
    {0x36, "6"},
    {0x37, "7"},
    {0x38, "8"},
    {0x39, "9"},
    {0x41, "A"},
    {0x42, "B"},
    {0x43, "C"},
    {0x44, "D"},
    {0x45, "E"},
    {0x46, "F"},
    {0x47, "G"},
    {0x48, "H"},
    {0x49, "I"},
    {0x4A, "J"},
    {0x4B, "K"},
    {0x4C, "L"},
    {0x4D, "M"},
    {0x4E, "N"},
    {0x4F, "O"},
    {0x50, "P"},
    {0x51, "Q"},
    {0x52, "R"},
    {0x53, "S"},
    {0x54, "T"},
    {0x55, "U"},
    {0x56, "V"},
    {0x57, "W"},
    {0x58, "X"},
    {0x59, "Y"},
    {0x5A, "Z"}
};

HotKeyTools::HotKeyTools()
{}
bool HotKeyTools::sendKeyCombination(const QString &combination) {
    QStringList keys = combination.toLower().split("+");
    QList<WORD> vkCodes;

    for(const QString &key : keys) {
        if (mKeyMap.contains(key)) {
            vkCodes.append(mKeyMap[key]);
        } else {
            qWarning() << "Unknow key : " << key;
            return false;
        }
    }

    int keyCount = vkCodes.size();
    INPUT *inputs = new INPUT[keyCount * 2];
    int inputIndex = 0;
    // 按下所有按键
    for (int i = 0; i < keyCount; i++) {
        inputs[inputIndex].type = INPUT_KEYBOARD;
        inputs[inputIndex].ki.wVk = vkCodes[i];
        inputs[inputIndex].ki.dwFlags = 0;
        inputIndex++;
    }
    // 释放所有按键（按相反顺序）
    for (int i = keyCount - 1; i >= 0; i--) {
        inputs[inputIndex].type = INPUT_KEYBOARD;
        inputs[inputIndex].ki.wVk = vkCodes[i];
        inputs[inputIndex].ki.dwFlags = KEYEVENTF_KEYUP;
        inputIndex++;
    }

    UINT sent = SendInput(inputIndex, inputs, sizeof(INPUT));
    delete[] inputs;
    return sent == inputIndex;
}

const QMap<WORD, QString>& HotKeyTools::getValueToKeyMap() {
    return mValueMap;
}
