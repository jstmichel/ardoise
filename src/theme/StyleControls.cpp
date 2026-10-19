#include <Ardoise/Style.h>
#include <Ardoise/Theme.h>
#include <QFrame>
#include <QPainter>
#include <QPlainTextEdit>
#include <QPainterPath>
#include <QStyleOption>
#include <QWidget>
#include <QTreeView>

namespace Ardoise
{
namespace
{
    void surface(
        QPainter* p, QRectF rect, const QColor& fill, const QColor& border, int radius, int width = 1)
    {
        p->save();
        p->setRenderHint(QPainter::Antialiasing);
        rect.adjust(width / 2.0, width / 2.0, -width / 2.0, -width / 2.0);
        p->setPen(QPen(border, width));
        p->setBrush(fill);
        p->drawRoundedRect(rect, radius, radius);
        p->restore();
    }
    void arrow(QPainter* p, const QRect& rect, Qt::ArrowType type, QColor color)
    {
        const qreal s = qMin(rect.width(), rect.height()) / 5.0;
        const QPointF c = QRectF(rect).center();
        QPolygonF points;
        if (type == Qt::DownArrow)
            points = { c + QPointF(-s, -s / 2), c + QPointF(0, s / 2), c + QPointF(s, -s / 2) };
        if (type == Qt::UpArrow)
            points = { c + QPointF(-s, s / 2), c + QPointF(0, -s / 2), c + QPointF(s, s / 2) };
        if (type == Qt::RightArrow)
            points = { c + QPointF(-s / 2, -s), c + QPointF(s / 2, 0), c + QPointF(-s / 2, s) };
        if (type == Qt::LeftArrow)
            points = { c + QPointF(s / 2, -s), c + QPointF(-s / 2, 0), c + QPointF(s / 2, s) };
        p->save();
        p->setRenderHint(QPainter::Antialiasing);
        p->setPen(QPen(color, 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p->drawPolyline(points);
        p->restore();
    }
    QRect treeItemTextRect(const QStyle* style, const QStyleOptionViewItem* item, const QWidget* widget)
{
    const QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, item, widget);
    const int textWidth = qMin(textRect.width(), item->fontMetrics.horizontalAdvance(item->text)
        + 2 * Theme::tokens().metrics.spacing4);
    return QStyle::alignedRect(item->direction, item->displayAlignment,
        QSize(textWidth, textRect.height()), textRect);
}
QColor border(const QStyleOption* o)
    {
        const auto& c = Theme::tokens().colors;
        if (!(o->state & QStyle::State_Enabled))
            return c.borderSubtle;
        if (o->state & QStyle::State_HasFocus)
            return c.borderAccent;
        return o->state & QStyle::State_MouseOver ? c.borderHover : c.borderNormal;
    }
    void input(QPainter* p, const QStyleOption* o)
    {
        const auto& t = Theme::tokens();
        QColor fill = o->state & QStyle::State_HasFocus ? t.colors.inputBackgroundFocus
            : o->state & QStyle::State_MouseOver        ? t.colors.inputBackgroundHover
                                                        : t.colors.inputBackground;
        surface(p, o->rect, fill, border(o), t.metrics.radiusSmall,
            o->state & QStyle::State_HasFocus ? t.metrics.focusBorderWidth : t.metrics.borderWidth);
    }
}
void Style::drawPrimitive(
    PrimitiveElement element, const QStyleOption* o, QPainter* p, const QWidget* w) const
{
    const auto& t = Theme::tokens();
    const auto& c = t.colors;
    const auto& m = t.metrics;
    const bool enabled = o->state & State_Enabled, hover = o->state & State_MouseOver,
               pressed = o->state & State_Sunken;
    switch (element)
    {
    case PE_PanelButtonCommand:
    case PE_PanelButtonTool:
    {
        const bool primary = w && w->property("ardoisePrimary").toBool();
        QColor fill = primary ? (enabled ? (pressed        ? c.accentPressed
                                                   : hover ? c.accentHover
                                                           : c.accent)
                                         : c.accentMuted)
                              : (pressed        ? c.inputBackground
                                        : hover ? c.panelBackgroundHover
                                                : c.backgroundElevated);
        if (w && w->property("ardoiseDestructive").toBool() && hover)
            fill = c.danger;
        surface(p, o->rect, fill, primary && enabled ? c.accentHover : border(o), m.radiusSmall,
            o->state & State_HasFocus ? m.focusBorderWidth : m.borderWidth);
        return;
    }
    case PE_PanelLineEdit:
        if (const auto* frame = qstyleoption_cast<const QStyleOptionFrame*>(o);
            frame && frame->lineWidth == 0)
            return;
        input(p, o);
        return;
    case PE_FrameLineEdit:
    case PE_Frame:
        if (const auto* frame = qstyleoption_cast<const QStyleOptionFrame*>(o);
            frame && frame->lineWidth == 0)
            return;
        // Text edits reserve viewport margins for counters; fill the whole rounded
        // field so those margins and the text viewport share one surface.
        surface(p, o->rect, qobject_cast<const QPlainTextEdit*>(w) ? c.inputBackground : QColor(Qt::transparent),
            border(o), m.radiusSmall,
            o->state & State_HasFocus ? m.focusBorderWidth : m.borderWidth);
        return;
    case PE_FrameFocusRect:
        surface(p, o->rect, Qt::transparent, c.borderAccent, m.radiusSmall, m.focusBorderWidth);
        return;
    case PE_PanelMenu:
    case PE_PanelTipLabel:
        surface(p, o->rect, c.backgroundElevated, c.borderNormal, m.radiusMedium);
        return;
    case PE_IndicatorArrowDown:
    case PE_IndicatorArrowUp:
    case PE_IndicatorArrowLeft:
    case PE_IndicatorArrowRight:
        arrow(p, o->rect,
            element == PE_IndicatorArrowDown       ? Qt::DownArrow
                : element == PE_IndicatorArrowUp   ? Qt::UpArrow
                : element == PE_IndicatorArrowLeft ? Qt::LeftArrow
                                                   : Qt::RightArrow,
            enabled ? c.textSecondary : c.textDisabled);
        return;
    case PE_PanelItemViewItem:
    {
        QRect itemRect = o->rect;
        if (const auto* item = qstyleoption_cast<const QStyleOptionViewItem*>(o);
            item && qobject_cast<const QTreeView*>(w))
        {
            itemRect = treeItemTextRect(this, item, w);
        }
        if (o->state & State_Selected)
            surface(p, itemRect, c.selectionBackground, c.selectionBackground, m.radiusSmall);
        else if (hover)
            surface(p, itemRect, c.panelBackgroundHover, c.panelBackgroundHover, m.radiusSmall);
        return;
    }
    case PE_IndicatorBranch:
    {
        p->save();
        p->setPen(c.separator);
        const auto center = o->rect.center();
        if (o->state & State_Sibling)
            p->drawLine(center.x(), o->rect.top(), center.x(), o->rect.bottom());
        else if (o->state & State_Item)
            p->drawLine(center.x(), o->rect.top(), center.x(), center.y());
        if (o->state & State_Item)
            p->drawLine(center.x(), center.y(), o->rect.right(), center.y());
        if (o->state & State_Children)
        {
            QRect area(center.x() - m.iconSizeSmall / 2, center.y() - m.iconSizeSmall / 2, m.iconSizeSmall,
                m.iconSizeSmall);
            p->fillRect(area, c.panelBackground);
            arrow(p, area, o->state & State_Open ? Qt::DownArrow : Qt::RightArrow, c.textSecondary);
        }
        p->restore();
        return;
    }
    case PE_IndicatorCheckBox:
    {
        const bool checked = o->state & (State_On | State_NoChange);
        surface(p, o->rect, checked ? (enabled ? c.accent : c.accentMuted) : c.inputBackground, border(o),
            m.radiusSmall);
        if (checked)
        {
            p->save();
            p->setRenderHint(QPainter::Antialiasing);
            p->setPen(QPen(enabled ? c.selectionText : c.textDisabled, 1.6, Qt::SolidLine, Qt::RoundCap,
                Qt::RoundJoin));
            auto r = QRectF(o->rect).adjusted(m.spacing4, m.spacing4, -m.spacing4, -m.spacing4);
            if (o->state & State_NoChange)
                p->drawLine(
                    r.topLeft() + QPointF(0, r.height() / 2), r.topRight() + QPointF(0, r.height() / 2));
            else
                p->drawPolyline(QPolygonF { QPointF(r.left(), r.center().y()),
                    QPointF(r.center().x() - 1, r.bottom()), r.topRight() });
            p->restore();
        }
        return;
    }
    default:
        QProxyStyle::drawPrimitive(element, o, p, w);
        return;
    }
}
void Style::drawControl(
    ControlElement element, const QStyleOption* o, QPainter* p, const QWidget* w) const
{
    if (element == CE_ShapedFrame)
    {
        if (const auto* frame = qstyleoption_cast<const QStyleOptionFrame*>(o);
            frame && (frame->frameShape == QFrame::HLine || frame->frameShape == QFrame::VLine))
        {
            p->save();
            p->setPen(Theme::tokens().colors.separator);
            if (frame->frameShape == QFrame::HLine)
                p->drawLine(o->rect.left(), o->rect.center().y(), o->rect.right(), o->rect.center().y());
            else
                p->drawLine(o->rect.center().x(), o->rect.top(), o->rect.center().x(), o->rect.bottom());
            p->restore();
            return;
        }
    }
    if (element == CE_Splitter)
    {
        p->fillRect(o->rect, Theme::tokens().colors.backgroundPrimary);
        return;
    }
    if (element == CE_PushButtonLabel && w && w->property("ardoisePrimary").toBool())
    {
        if (const auto* button = qstyleoption_cast<const QStyleOptionButton*>(o))
        {
            auto copy = *button;
            copy.palette.setColor(QPalette::ButtonText,
                o->state & State_Enabled ? Theme::tokens().colors.selectionText
                                         : Theme::tokens().colors.textMuted);
            QProxyStyle::drawControl(element, &copy, p, w);
            return;
        }
    }
    QProxyStyle::drawControl(element, o, p, w);
}
QRect Style::subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const
{
    if (element == SE_ItemViewItemFocusRect && qobject_cast<const QTreeView*>(widget))
        if (const auto* item = qstyleoption_cast<const QStyleOptionViewItem*>(option))
            return treeItemTextRect(this, item, widget);
    return QProxyStyle::subElementRect(element, option, widget);
}
QRect Style::subControlRect(
    ComplexControl control, const QStyleOptionComplex* o, SubControl part, const QWidget* w) const
{
    const auto& m = Theme::tokens().metrics;
    if (control == CC_ScrollBar)
    {
        const auto* slider = qstyleoption_cast<const QStyleOptionSlider*>(o);
        if (!slider)
            return QProxyStyle::subControlRect(control, o, part, w);
        const bool horizontal = slider->orientation == Qt::Horizontal;
        const int length = horizontal ? o->rect.width() : o->rect.height();
        const qint64 range = qint64(slider->maximum) - slider->minimum;
        const qint64 total = range + slider->pageStep;
        const int thumbLength = range <= 0 ? length
            : qBound(0, qMax(m.scrollbarMinimumThumb,
                            total > 0 ? int(qint64(length) * slider->pageStep / total) : 0), length);
        const int position = sliderPositionFromValue(slider->minimum, slider->maximum,
            slider->sliderPosition, length - thumbLength, slider->upsideDown);
        const auto segment = [&](int start, int size)
        {
            return horizontal ? QRect(o->rect.left() + start, o->rect.top(), size, o->rect.height())
                              : QRect(o->rect.left(), o->rect.top() + start, o->rect.width(), size);
        };
        switch (part)
        {
        case SC_ScrollBarSlider: return segment(position, thumbLength);
        case SC_ScrollBarGroove: return o->rect;
        case SC_ScrollBarSubPage:
            return slider->upsideDown ? segment(position + thumbLength, length - position - thumbLength)
                                     : segment(0, position);
        case SC_ScrollBarAddPage:
            return slider->upsideDown ? segment(0, position)
                                     : segment(position + thumbLength, length - position - thumbLength);
        default: return {};
        }
    }
    if (control == CC_ComboBox)
    {
        QRect r;
        if (part == SC_ComboBoxArrow)
            r = QRect(
                o->rect.right() - m.controlHeight + 1, o->rect.top(), m.controlHeight, o->rect.height());
        else if (part == SC_ComboBoxEditField)
            r = o->rect.adjusted(m.spacing8, m.spacing2, -m.controlHeight, -m.spacing2);
        else
            return QProxyStyle::subControlRect(control, o, part, w);
        return visualRect(o->direction, o->rect, r);
    }
    if (control == CC_SpinBox)
    {
        const auto* spin = qstyleoption_cast<const QStyleOptionSpinBox*>(o);
        const int buttons
            = spin && spin->buttonSymbols == QAbstractSpinBox::NoButtons ? 0 : m.controlHeightSmall;
        QRect r;
        if (part == SC_SpinBoxUp)
            r = QRect(o->rect.right() - buttons + 1, o->rect.top(), buttons, o->rect.height() / 2);
        else if (part == SC_SpinBoxDown)
            r = QRect(o->rect.right() - buttons + 1, o->rect.top() + o->rect.height() / 2, buttons,
                o->rect.height() - o->rect.height() / 2);
        else if (part == SC_SpinBoxEditField)
            r = o->rect.adjusted(m.spacing8, m.spacing2, -buttons - m.spacing4, -m.spacing2);
        else
            return o->rect;
        return visualRect(o->direction, o->rect, r);
    }
    return QProxyStyle::subControlRect(control, o, part, w);
}
void Style::drawComplexControl(
    ComplexControl control, const QStyleOptionComplex* o, QPainter* p, const QWidget* w) const
{
    const auto& t = Theme::tokens();
    const auto& c = t.colors;
    const auto& m = t.metrics;
    if (control == CC_ComboBox || control == CC_SpinBox)
    {
        input(p, o);
        if (control == CC_ComboBox)
            arrow(p, subControlRect(control, o, SC_ComboBoxArrow, w), Qt::DownArrow,
                o->state & State_Enabled ? c.textSecondary : c.textDisabled);
        else
        {
            const auto* spin = qstyleoption_cast<const QStyleOptionSpinBox*>(o);
            for (auto part : { SC_SpinBoxUp, SC_SpinBoxDown })
            {
                bool enabled = (o->state & State_Enabled) && spin
                    && (spin->stepEnabled
                        & (part == SC_SpinBoxUp ? QAbstractSpinBox::StepUpEnabled
                                                : QAbstractSpinBox::StepDownEnabled));
                auto r = subControlRect(control, o, part, w);
                if ((o->activeSubControls & part) && (o->state & State_MouseOver))
                    p->fillRect(r.adjusted(1, 1, -1, -1), c.panelBackgroundHover);
                arrow(p, r, part == SC_SpinBoxUp ? Qt::UpArrow : Qt::DownArrow,
                    enabled ? c.textSecondary : c.textDisabled);
            }
        }
        return;
    }
    if (control == CC_ToolButton)
    {
        if (w && w->property("ardoiseSectionHeader").toBool())
        {
            if (o->state & State_MouseOver)
                surface(p, o->rect, c.panelBackgroundHover, Qt::transparent, m.radiusSmall, 0);
            if (const auto* button = qstyleoption_cast<const QStyleOptionToolButton*>(o))
            {
                auto iconRect = QRect(o->rect.left() + m.spacing4,
                    o->rect.center().y() - m.iconSizeSmall / 2, m.iconSizeSmall, m.iconSizeSmall);
                button->icon.paint(p, iconRect);
                p->save();
                p->setPen(c.textPrimary);
                p->setFont(w->font());
                p->drawText(o->rect.adjusted(m.iconSizeSmall + m.spacing12, 0, -m.spacing8, 0),
                    Qt::AlignLeft | Qt::AlignVCenter, button->text);
                p->restore();
            }
            if (o->state & State_HasFocus)
                surface(p, o->rect, Qt::transparent, c.borderAccent, m.radiusSmall, m.focusBorderWidth);
            return;
        }
        if (w && w->property("ardoiseFlat").toBool()) {
            if (o->state & (State_MouseOver | State_Sunken | State_On))
                surface(p, o->rect, o->state & State_On ? c.accentMuted : c.panelBackgroundHover,
                    Qt::transparent, m.radiusSmall, 0);
            if (o->state & State_HasFocus)
                surface(p, o->rect, Qt::transparent, c.borderAccent, m.radiusSmall, m.focusBorderWidth);
        } else
            drawPrimitive(PE_PanelButtonTool, o, p, w);
        QProxyStyle::drawControl(CE_ToolButtonLabel, o, p, w);
        return;
    }
    if (control == CC_ScrollBar)
    {
        const auto* slider = qstyleoption_cast<const QStyleOptionSlider*>(o);
        if (!slider)
            return;
        QRectF thumb = subControlRect(control, o, SC_ScrollBarSlider, w);
        const bool horizontal = slider->orientation == Qt::Horizontal;
        const qreal inset = qMax(0.0, ((horizontal ? thumb.height() : thumb.width()) - m.scrollbarThumbExtent) / 2.0);
        thumb.adjust(horizontal ? 0 : inset, horizontal ? inset : 0,
                     horizontal ? 0 : -inset, horizontal ? -inset : 0);
        const bool active = o->activeSubControls & SC_ScrollBarSlider;
        if (!thumb.isEmpty())
            surface(p, thumb,
                active && (o->state & State_Sunken) ? c.borderHover
                    : active && (o->state & State_MouseOver) ? c.borderNormal
                                                            : c.borderSubtle,
                Qt::transparent, m.scrollbarThumbExtent / 2, 0);
        return;
    }
    if (control == CC_Slider)
    {
        if (const auto* slider = qstyleoption_cast<const QStyleOptionSlider*>(o))
        {
            auto groove = subControlRect(control, o, SC_SliderGroove, w);
            auto handle = subControlRect(control, o, SC_SliderHandle, w);
            const bool horizontal = slider->orientation == Qt::Horizontal;
            if (horizontal)
                groove = QRect(groove.left(), groove.center().y() - m.spacing2, groove.width(), m.spacing4);
            else
                groove = QRect(groove.center().x() - m.spacing2, groove.top(), m.spacing4, groove.height());
            surface(p, groove, c.borderSubtle, Qt::transparent, m.spacing2, 0);
            auto active = groove;
            if (horizontal)
            {
                if (slider->upsideDown)
                    active.setLeft(handle.center().x());
                else
                    active.setRight(handle.center().x());
            }
            else
            {
                if (slider->upsideDown)
                    active.setTop(handle.center().y());
                else
                    active.setBottom(handle.center().y());
            }
            surface(p, active, o->state & State_Enabled ? c.accent : c.accentMuted, Qt::transparent,
                m.spacing2, 0);
            p->save();
            p->setRenderHint(QPainter::Antialiasing);
            p->setPen(QPen(o->state & State_HasFocus ? c.selectionText : c.borderAccent, m.borderWidth));
            p->setBrush(o->state & State_Enabled ? c.borderAccent : c.textDisabled);
            p->drawEllipse(handle.adjusted(1, 1, -1, -1));
            p->restore();
            return;
        }
    }
    QProxyStyle::drawComplexControl(control, o, p, w);
}
}
