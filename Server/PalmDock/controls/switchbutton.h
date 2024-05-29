#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H
#include <QWidget>

class SwitchButton : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int m_space READ getSpace WRITE setSpace)
    Q_PROPERTY(int m_rectRadius READ getRectRadius  WRITE setRectRadius)
    Q_PROPERTY(bool m_checked READ getChecked WRITE setChecked)
    Q_PROPERTY(bool m_showText READ getShowText WRITE setShowText)
    Q_PROPERTY(bool m_animation READ getAnimation WRITE setAnimation)

    Q_PROPERTY(QColor m_bgColorOff READ getBgColorOff WRITE setBgColorOff)
    Q_PROPERTY(QColor m_bgColorOn READ getBgColorOn WRITE setBgColorOn)
    Q_PROPERTY(QColor m_sliderColorOff READ getSliderColorOff WRITE setSliderColorOff)
    Q_PROPERTY(QColor m_sliderColorOn READ getSliderColorOn WRITE setSliderColorOn)
    Q_PROPERTY(QColor m_textColorOff READ getTextColorOff WRITE setTextColorOff)
    Q_PROPERTY(QColor m_textColorOn READ getTextColorOn WRITE setTextColorOn)

    Q_PROPERTY(QString m_textOff READ getTextOff WRITE setTextOff)
    Q_PROPERTY(QString m_textOn READ getTextOn WRITE setTextOn)

    Q_PROPERTY(int m_step READ getStep WRITE setStep)
    Q_PROPERTY(int m_startX READ getStartX WRITE setStartX)
    Q_PROPERTY(int m_endX READ getEndX WRITE setEndX)

public:
    explicit SwitchButton(QWidget *parent = nullptr);

private:
    int m_space;              // 滑块离背景的间距
    int m_rectRadius;         // 圆角角度
    bool m_checked;           // 是否选中
    bool m_showText;          // 是否显示文字
    bool m_animation;         // 动画过度

    QColor m_bgColorOff;      // 关闭时的背景颜色
    QColor m_bgColorOn;       // 打开时的背景颜色
    QColor m_sliderColorOff;  // 关闭时的滑块颜色
    QColor m_sliderColorOn;   // 打开时的滑块颜色
    QColor m_textColorOff;    // 关闭时的文字颜色
    QColor m_textColorOn;     // 打开时的文字颜色

    QString m_textOff;        // 关闭时的显示文字
    QString m_textOn;         // 打开时的显示文字

    int m_step;               // 每次移动的步长
    int m_startX;             // 滑块开始的 x 轴坐标
    int m_endX;               // 滑块结束的 x 轴坐标
    QTimer* m_timer;          // 定时器

public:
    int getSpace();
    int getRectRadius();
    bool getChecked();
    bool getShowText();
    bool getAnimation();

    QColor getBgColorOff();
    QColor getBgColorOn();
    QColor getSliderColorOff();
    QColor getSliderColorOn();
    QColor getTextColorOff();
    QColor getTextColorOn();

    QString getTextOff();
    QString getTextOn();

    int getStep();
    int getStartX();
    int getEndX();

public Q_SLOTS:
    void setSpace(int space);
    void setRectRadius(int rectRadius);
    void setChecked(bool checked);
    void setShowText(bool showText);
    void setAnimation(bool animation);

    void setBgColorOff(QColor &bgColorOff);
    void setBgColorOn(QColor &bgColorOn);
    void setSliderColorOff(QColor &sliderColorOff);
    void setSliderColorOn(QColor &sliderColorOn);
    void setTextColorOff(QColor &textColorOff);
    void setTextColorOn(QColor &textColorOn);

    void setTextOff(QString &textOff);
    void setTextOn(QString &textOn);

    void setStep(int step);
    void setStartX(int startX);
    void setEndX(int endX);

private slots:
    void updateValue();

protected:
    void mousePressEvent(QMouseEvent* );
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent*);
    void drawBg(QPainter* painter);
    void drawSlider(QPainter* painter);

Q_SIGNALS:
    void checkedChanged(bool checked);

signals:
};

#endif // SWITCHBUTTON_H
