#include "squarebutton.h"

SquareButton::SquareButton(QWidget* parent) : QPushButton(parent){};

SquareButton::SquareButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {}

void SquareButton::resizeEvent(QResizeEvent* event) {
    QPushButton::resizeEvent(event);
    int length = qMin(this->width(), this->height());
    this->setFixedSize(length, length);
}
