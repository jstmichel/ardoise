#include <Ardoise/IconButton.h>
#include <Ardoise/Theme.h>
#include <QVariant>
namespace Ardoise
{
IconButton::IconButton(Icon icon, const QString& label, QWidget* parent)
    : QToolButton(parent)
{
    const auto& m = Theme::tokens().metrics;
    setIcon(icon);
    setToolTip(label);
    setAccessibleName(label);
    setIconSize(QSize(m.iconSize, m.iconSize));
    setFixedSize(m.controlHeight, m.controlHeight);
    setFocusPolicy(Qt::StrongFocus);
}
void IconButton::setIcon(Icon value) { QToolButton::setIcon(IconProvider::icon(value)); }
void IconButton::setTone(ButtonTone tone)
{
    setProperty("ardoiseDestructive", tone == ButtonTone::Destructive);
    update();
}
}
