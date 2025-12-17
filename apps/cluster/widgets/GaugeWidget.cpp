#include "GaugeWidget.hpp"
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QtMath>

GaugeWidget::GaugeWidget(QWidget *parent) : QWidget(parent) {
    setMinimumSize(200, 200);
    
    // 부드러운 애니메이션을 위한 타이머
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        if (current_value_ != target_value_) {
            int diff = target_value_ - current_value_;
            current_value_ += diff / 5 + (diff > 0 ? 1 : -1);
            
            // 오버슈트 방지
            if ((diff > 0 && current_value_ > target_value_) ||
                (diff < 0 && current_value_ < target_value_)) {
                current_value_ = target_value_;
            }
            update();
        }
    });
    timer->start(16); // ~60 FPS
}

void GaugeWidget::setValue(int value) {
    target_value_ = qBound(min_value_, value, max_value_);
}

QSize GaugeWidget::sizeHint() const {
    return QSize(300, 300);
}

QSize GaugeWidget::minimumSizeHint() const {
    return QSize(150, 150);
}

void GaugeWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    drawBackground(painter);
    drawScale(painter);
    drawNeedle(painter);
    drawValue(painter);
    drawLabel(painter);
}

void GaugeWidget::drawBackground(QPainter &painter) {
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 300.0, side / 300.0);
    
    // 외곽 원
    QRadialGradient gradient(0, 0, 150);
    gradient.setColorAt(0.0, QColor(40, 40, 45));
    gradient.setColorAt(0.9, QColor(25, 25, 30));
    gradient.setColorAt(1.0, QColor(60, 60, 65));
    
    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(80, 80, 85), 3));
    painter.drawEllipse(QPoint(0, 0), 145, 145);
    
    // 내부 원
    QRadialGradient innerGradient(0, 0, 120);
    innerGradient.setColorAt(0.0, QColor(20, 20, 25));
    innerGradient.setColorAt(1.0, QColor(35, 35, 40));
    
    painter.setBrush(innerGradient);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPoint(0, 0), 130, 130);
    
    painter.restore();
}

void GaugeWidget::drawScale(QPainter &painter) {
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 300.0, side / 300.0);
    
    // 스케일 범위: -135도 ~ +135도 (270도)
    const double startAngle = 225.0; // 시작 각도
    const double spanAngle = 270.0;  // 전체 각도
    const int majorTicks = major_ticks_;  // 주요 눈금
    const int minorTicks = 5;        // 보조 눈금
    
    painter.setPen(QPen(QColor(200, 200, 200), 2));
    
    for (int i = 0; i <= majorTicks; ++i) {
        double angle = startAngle - (spanAngle * i / majorTicks);
        double rad = qDegreesToRadians(angle);
        
        int x1 = 110 * qCos(rad);
        int y1 = -110 * qSin(rad);
        int x2 = 125 * qCos(rad);
        int y2 = -125 * qSin(rad);
        
        painter.drawLine(x1, y1, x2, y2);
        
        // 숫자 표시 (짝수 인덱스에만 표시하여 간격을 2배로)
        if (i % 2 == 0) {
            int value = min_value_ + (max_value_ - min_value_) * i / majorTicks;
            QString text = QString::number(value);
            
            int tx = 95 * qCos(rad);
            int ty = -95 * qSin(rad);
            
            painter.save();
            painter.setPen(QColor(220, 220, 220));
            QFont font = painter.font();
            font.setPointSize(12);
            font.setBold(true);
            painter.setFont(font);
            
            QRect textRect(tx - 20, ty - 10, 40, 20);
            painter.drawText(textRect, Qt::AlignCenter, text);
            painter.restore();
        }
    }
    
    // 보조 눈금
    painter.setPen(QPen(QColor(150, 150, 150), 1));
    for (int i = 0; i < majorTicks; ++i) {
        for (int j = 1; j < minorTicks; ++j) {
            double angle = startAngle - (spanAngle * (i + j / (double)minorTicks) / majorTicks);
            double rad = qDegreesToRadians(angle);
            
            int x1 = 115 * qCos(rad);
            int y1 = -115 * qSin(rad);
            int x2 = 125 * qCos(rad);
            int y2 = -125 * qSin(rad);
            
            painter.drawLine(x1, y1, x2, y2);
        }
    }
    
    painter.restore();
}

void GaugeWidget::drawNeedle(QPainter &painter) {
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 300.0, side / 300.0);
    
    // 현재 값에 따른 각도 계산
    double ratio = (double)(current_value_ - min_value_) / (max_value_ - min_value_);
    double angle = 225.0 - (270.0 * ratio); // 225도(7시)에서 시작해서 -45도(5시)까지
    
    painter.rotate(-angle + 90); // +90도 오프셋으로 바늘이 위를 가리키도록 보정
    
    // 바늘 그리기
    QPainterPath needlePath;
    needlePath.moveTo(0, 0);
    needlePath.lineTo(-3, -10);
    needlePath.lineTo(-2, -100);
    needlePath.lineTo(2, -100);
    needlePath.lineTo(3, -10);
    needlePath.closeSubpath();
    
    // 바늘 색상 (속도에 따라 변경)
    QColor needleColor;
    if (ratio < 0.7) {
        needleColor = QColor(100, 200, 255); // 파란색
    } else if (ratio < 0.9) {
        needleColor = QColor(255, 200, 50); // 노란색
    } else {
        needleColor = QColor(255, 80, 80); // 빨간색
    }
    
    QLinearGradient needleGradient(0, -100, 0, 0);
    needleGradient.setColorAt(0.0, needleColor);
    needleGradient.setColorAt(1.0, needleColor.darker(150));
    
    painter.setBrush(needleGradient);
    painter.setPen(QPen(needleColor.darker(200), 1));
    painter.drawPath(needlePath);
    
    // 중심 원
    QRadialGradient centerGradient(0, 0, 12);
    centerGradient.setColorAt(0.0, QColor(180, 180, 180));
    centerGradient.setColorAt(0.5, QColor(120, 120, 120));
    centerGradient.setColorAt(1.0, QColor(60, 60, 60));
    
    painter.setBrush(centerGradient);
    painter.setPen(QPen(QColor(100, 100, 100), 2));
    painter.drawEllipse(QPoint(0, 0), 12, 12);
    
    painter.restore();
}

void GaugeWidget::drawValue(QPainter &painter) {
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 300.0, side / 300.0);
    
    // 현재 값 표시
    painter.setPen(QColor(255, 255, 255));
    QFont font = painter.font();
    font.setPointSize(24);
    font.setBold(true);
    painter.setFont(font);
    
    QString valueText = QString::number(current_value_);
    QRect valueRect(-60, 30, 120, 40);
    painter.drawText(valueRect, Qt::AlignCenter, valueText);
    
    // 단위 표시
    font.setPointSize(12);
    font.setBold(false);
    painter.setFont(font);
    painter.setPen(QColor(200, 200, 200));
    QRect unitRect(-60, 65, 120, 20);
    painter.drawText(unitRect, Qt::AlignCenter, unit_);
    
    painter.restore();
}

void GaugeWidget::drawLabel(QPainter &painter) {
    int side = qMin(width(), height());
    painter.save();
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 300.0, side / 300.0);
    
    // 라벨 표시
    painter.setPen(QColor(150, 200, 255));
    QFont font = painter.font();
    font.setPointSize(14);
    font.setBold(true);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
    painter.setFont(font);
    
    QRect labelRect(-80, -30, 160, 25);
    painter.drawText(labelRect, Qt::AlignCenter, label_);
    
    painter.restore();
}
