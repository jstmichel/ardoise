#pragma once
#include <QProxyStyle>
namespace Ardoise
{
class Style final : public QProxyStyle
{
    Q_OBJECT
public:
    Style();
    QPalette standardPalette() const override;
    void polish(QWidget* widget) override;
    using QProxyStyle::polish;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr,
        const QWidget* widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption* option = nullptr, const QWidget* widget = nullptr,
        QStyleHintReturn* data = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size,
        const QWidget* widget = nullptr) const override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter,
        const QWidget* widget = nullptr) const override;
    QRect subElementRect(SubElement element, const QStyleOption* option,
        const QWidget* widget = nullptr) const override;
    QRect subControlRect(ComplexControl control, const QStyleOptionComplex* option, SubControl subControl,
        const QWidget* widget = nullptr) const override;
};
}
