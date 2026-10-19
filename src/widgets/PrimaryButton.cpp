#include <Ardoise/PrimaryButton.h>
#include <Ardoise/Theme.h>
#include <QVariant>
namespace Ardoise
{
PrimaryButton::PrimaryButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent)
{
    setProperty("ardoisePrimary", true);
    setMinimumHeight(Theme::tokens().metrics.controlHeightLarge);
    setIconSize(QSize(Theme::tokens().metrics.iconSize, Theme::tokens().metrics.iconSize));
}
PrimaryButton::PrimaryButton(const QIcon& icon, const QString& text, QWidget* parent)
    : PrimaryButton(text, parent)
{
    setIcon(icon);
}
}
