#ifndef SQUAREBUTTON_H
#define SQUAREBUTTON_H

#include <QPushButton>

class SquareButton : public QPushButton
{
    Q_OBJECT
public:
    explicit SquareButton(QWidget* parent = nullptr);
    explicit SquareButton(const QString &text, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* event) override;
};

#endif // SQUAREBUTTON_H
