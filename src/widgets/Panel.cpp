#include <Ardoise/Panel.h>
#include <Ardoise/Theme.h>
#include <QPainter>
namespace Ardoise
{
Panel::Panel(QWidget* parent)
    : QFrame(parent)
    , m_radius(Theme::tokens().metrics.radiusMedium)
{
    auto p = palette();
    p.setColor(QPalette::Window, Theme::tokens().colors.panelBackground);
    setPalette(p);
}
void Panel::setCornerRadius(int radius)
{
    m_radius = qMax(0, radius);
    update();
}
void Panel::paintEvent(QPaintEvent*)
{
    const auto& t = Theme::tokens();
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(t.colors.borderNormal, t.metrics.borderWidth));
    p.setBrush(t.colors.panelBackground);
    const qreal half = t.metrics.borderWidth / 2.0;
    p.drawRoundedRect(QRectF(rect()).adjusted(half, half, -half, -half), m_radius, m_radius);
}
}
