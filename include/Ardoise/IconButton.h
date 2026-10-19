#pragma once
#include <Ardoise/IconProvider.h>
#include <QToolButton>
namespace Ardoise
{
enum class ButtonTone
{
    Normal,
    Destructive
};
class IconButton : public QToolButton
{
    Q_OBJECT
public:
    explicit IconButton(Icon icon, const QString& label, QWidget* parent = nullptr);
    using QToolButton::setIcon;
    void setIcon(Icon icon);
    void setTone(ButtonTone tone);
};
}
