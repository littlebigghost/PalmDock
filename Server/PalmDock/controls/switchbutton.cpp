#include "switchbutton.h"

#include <QPainter>
#include <QPainterPath>
#include <QRect>
#include <QTimer>

SwitchButton::SwitchButton(QWidget *parent)
    : QWidget{parent}
{
    m_space = 2;
    m_rectRadius = 5;
    m_checked = false;
    m_showText = true;
    m_showText = false;
    m_animation = true;

    m_bgColorOn = QColor(21, 156, 119);
    m_bgColorOff = QColor(111, 122, 126);

    m_sliderColorOn = QColor(255, 255, 255);
    m_sliderColorOff = QColor(255, 255, 255);

    m_textColorOn = QColor(255, 255, 255);
    m_textColorOff = QColor(255, 255, 255);

    m_textOn = "开启";
    m_textOff = "关闭";

    m_step = 0;
    m_startX = 0;
    m_endX = 0;

    m_timer = new QTimer(this);
    m_timer->setInterval(30);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateValue()));
}

void SwitchButton::updateValue() {
    if (m_checked) {
        if (m_startX < m_endX) {
            m_startX += m_step;
        } else {
            m_startX = m_endX;
            m_timer->stop();
        }
    } else {
        if (m_startX > m_endX) {
            m_startX -= m_step;
        } else {
            m_startX = m_endX;
            m_timer->stop();
        }
    }
    update();
}

void SwitchButton::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing); // 启用抗锯齿

    drawBg(&painter); // 绘制背景
    drawSlider(&painter); // 绘制滑块
}

void SwitchButton::drawBg(QPainter* painter) {
    painter->save();
    painter->setPen(Qt::NoPen);

    QColor bgColor = m_checked ? m_bgColorOn : m_bgColorOff;
    if (!isEnabled()) bgColor.setAlpha(60);

    painter->setBrush(bgColor);

    QRect rect(0, 0, width(), height());
    int side = qMin(rect.width(), rect.height());

    // 左侧半圆
    QPainterPath path1;
    path1.addEllipse(rect.x(), rect.y(), side, side);
    // 右侧半圆
    QPainterPath path2;
    path2.addEllipse(rect.width() - side, rect.y(), side, side);
    // 中间矩形
    QPainterPath path3;
    path3.addRect(rect.x() + side / 2, rect.y(), rect.width() - side, rect.height());

    QPainterPath path;
    path = path3 + path2 + path1;
    painter->drawPath(path);

    if (m_showText) {
        int sliderWidth = qMin(width(), height()) - m_space * 2;
        sliderWidth -= 5;

        if (m_checked) {
            QRect textRect(0, 0, width() - sliderWidth, height());
            painter->setPen(m_textColorOn);
            painter->drawText(textRect, Qt::AlignCenter, m_textOn);
        } else {
            QRect textRect(sliderWidth, 0, width() - sliderWidth, height());
            painter->setPen(m_textColorOff);
            painter->drawText(textRect, Qt::AlignCenter, m_textOff);
        }
    }
    painter->restore();
}

void SwitchButton::drawSlider(QPainter* painter) {
    painter->save();
    painter->setPen(Qt::NoPen);

    if (!m_checked) painter->setBrush(m_sliderColorOff);
    else painter->setBrush(m_sliderColorOn);

    QRect rect(0, 0, width(), height());
    int sliderWidth = qMin(rect.width(), rect.height()) - m_space * 2;
    QRect sliderRect(m_startX + m_space, m_space, sliderWidth, sliderWidth);
    painter->drawEllipse(sliderRect);

    painter->restore();
}

void SwitchButton::mousePressEvent(QMouseEvent* ) {
    m_checked = !m_checked;
    emit checkedChanged(m_checked);

    m_step = width() / 10;

    if (m_checked)
        m_endX = width() - height();
    else
        m_endX = 0;

    if (m_animation)
        m_timer->start();
    else {
        m_startX = m_endX;
        update();
    }
}

void SwitchButton::resizeEvent(QResizeEvent*) {
    update();
}

int SwitchButton::getSpace() {
    return m_space;
}

int SwitchButton::getRectRadius() {
    return m_rectRadius;
}

bool SwitchButton::getChecked() {
    return m_checked;
}

bool SwitchButton::getShowText() {
    return m_showText;
}

bool SwitchButton::getAnimation() {
    return m_animation;
}

QColor SwitchButton::getBgColorOff() {
    return m_bgColorOff;
}

QColor SwitchButton::getBgColorOn() {
    return m_bgColorOn;
}

QColor SwitchButton::getSliderColorOff() {
    return m_sliderColorOff;
}

QColor SwitchButton::getSliderColorOn() {
    return m_sliderColorOn;
}

QColor SwitchButton::getTextColorOff() {
    return m_textColorOff;
}

QColor SwitchButton::getTextColorOn() {
    return m_textColorOn;
}

QString SwitchButton::getTextOff() {
    return m_textOff;
}

QString SwitchButton::getTextOn() {
    return m_textOn;
}

int SwitchButton::getStep() {
    return m_step;
}

int SwitchButton::getStartX() {
    return m_startX;
}

int SwitchButton::getEndX() {
    return m_endX;
}

void SwitchButton::setSpace(int space) {
    if (m_space != space) {
        m_space = space;
        update();
    }
}
void SwitchButton::setRectRadius(int rectRadius) {
    if (m_rectRadius != rectRadius) {
        m_rectRadius = rectRadius;
        update();
    }
}

void SwitchButton::setChecked(bool checked) {
    if (m_checked != checked) {
        m_checked = checked;
        emit checkedChanged(m_checked);

        m_step = width() / 10;

        if (m_checked)
            m_endX = width() - height();
        else
            m_endX = 0;

        if (m_animation)
            m_timer->start();
        else {
            m_startX = m_endX;
            update();
        }
    }
}

void SwitchButton::setShowText(bool showText) {
    if (m_showText != showText) {
        m_showText = showText;
        update();
    }
}

void SwitchButton::setAnimation(bool animation) {
    if (m_animation != animation) {
        m_animation = animation;
        update();
    }
}

void SwitchButton::setBgColorOff(QColor &bgColorOff) {
    if (m_bgColorOff != bgColorOff) {
        m_bgColorOff = bgColorOff;
        update();
    }
}

void SwitchButton::setBgColorOn(QColor &bgColorOn) {
    if (m_bgColorOn != bgColorOn) {
        m_bgColorOn = bgColorOn;
        update();
    }
}

void SwitchButton::setSliderColorOff(QColor &sliderColorOff) {
    if (m_sliderColorOff != sliderColorOff) {
        m_sliderColorOff = sliderColorOff;
        update();
    }
}

void SwitchButton::setSliderColorOn(QColor &sliderColorOn) {
    if (m_sliderColorOn != sliderColorOn) {
        m_sliderColorOn = sliderColorOn;
        update();
    }
}

void SwitchButton::setTextColorOff(QColor &textColorOff) {
    if (m_textColorOff != textColorOff) {
        m_textColorOff = textColorOff;
        update();
    }
}

void SwitchButton::setTextColorOn(QColor &textColorOn) {
    if (m_textColorOn != textColorOn) {
        m_textColorOn = textColorOn;
        update();
    }
}

void SwitchButton::setTextOff(QString &textOff) {
    if (m_textOff != textOff) {
        m_textOff = textOff;
        update();
    }
}

void SwitchButton::setTextOn(QString &textOn) {
    if (m_textOn != textOn) {
        m_textOn = textOn;
        update();
    }
}

void SwitchButton::setStep(int step) {
    if (m_step != step) {
        m_step = step;
        update();
    }
}

void SwitchButton::setStartX(int startX) {
    if (m_startX != startX) {
        m_startX = startX;
        update();
    }
}

void SwitchButton::setEndX(int endX) {
    if (m_endX != endX) {
        m_endX = endX;
        update();
    }
}
