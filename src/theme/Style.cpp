#include <Ardoise/Style.h>
#include <Ardoise/Theme.h>
#include <QPainter>
#include <QStyleFactory>
#include <QStyleOption>
#include <QWidget>
#include <QTreeView>
#include <QScrollBar>
#include <QScrollArea>

namespace Ardoise
{
Style::Style()
    : QProxyStyle(QStyleFactory::create(QStringLiteral("Fusion")))
{
    setObjectName(QStringLiteral("ArdoiseDark"));
}
QPalette Style::standardPalette() const { return Theme::palette(); }
void Style::polish(QWidget* widget)
{
    QProxyStyle::polish(widget);
    widget->setAttribute(Qt::WA_Hover);
    widget->setMouseTracking(true);
    if (auto* area = qobject_cast<QScrollArea*>(widget))
    {
        area->setAutoFillBackground(false);
        area->viewport()->setAutoFillBackground(false);
        if (area->widget())
            area->widget()->setAutoFillBackground(false);
    }
    // Also cover content attached after the scroll area has been polished.
    if (auto* parent = widget->parentWidget())
        if (auto* area = qobject_cast<QScrollArea*>(parent->parentWidget());
            area && (widget == area->viewport() || parent == area->viewport()))
            widget->setAutoFillBackground(false);
    if (qobject_cast<QScrollBar*>(widget))
    {
        widget->setAutoFillBackground(false);
        widget->setAttribute(Qt::WA_OpaquePaintEvent, false);
    }
}
int Style::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    const auto& m = Theme::tokens().metrics;
    switch (metric)
    {
    case PM_DefaultFrameWidth:
        return m.borderWidth;
    case PM_ButtonMargin:
        return m.spacing12;
    case PM_LayoutHorizontalSpacing:
    case PM_LayoutVerticalSpacing:
        return m.spacing8;
    case PM_LayoutLeftMargin:
    case PM_LayoutRightMargin:
    case PM_LayoutTopMargin:
    case PM_LayoutBottomMargin:
        return m.spacing12;
    case PM_SmallIconSize:
        return m.iconSizeSmall;
    case PM_ButtonIconSize:
        return m.iconSize;
    case PM_ScrollBarExtent:
        return m.scrollbarExtent;
    case PM_ScrollBarSliderMin:
        return m.scrollbarMinimumThumb;
    case PM_SplitterWidth:
        return m.splitterHandleWidth;
    case PM_IndicatorWidth:
    case PM_IndicatorHeight:
        return m.iconSizeSmall;
    case PM_SliderThickness:
        return m.iconSize;
    case PM_SliderLength:
        return m.iconSizeSmall;
    default:
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
}
int Style::styleHint(
    StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* data) const
{
    if (hint == SH_ComboBox_Popup || hint == SH_ScrollBar_Transient)
        return 0;
    if (hint == SH_ItemView_ShowDecorationSelected)
        return qobject_cast<const QTreeView*>(widget) ? 0 : 1;
    return QProxyStyle::styleHint(hint, option, widget, data);
}
QSize Style::sizeFromContents(
    ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    auto result = QProxyStyle::sizeFromContents(type, option, size, widget);
    const auto& m = Theme::tokens().metrics;
    switch (type)
    {
    case CT_PushButton:
    case CT_ToolButton:
        result.setHeight(qMax(result.height(), m.controlHeightLarge));
        break;
    case CT_LineEdit:
    case CT_ComboBox:
    case CT_SpinBox:
        result.setHeight(qMax(result.height(), m.controlHeight));
        break;
    case CT_ItemViewItem:
        result.setHeight(qMax(result.height(), m.treeRowHeight));
        break;
    default:
        break;
    }
    return result;
}
}
