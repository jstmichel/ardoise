#pragma once
#include <QPushButton>
namespace Ardoise
{
class PrimaryButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PrimaryButton(const QString& text, QWidget* parent = nullptr);
    PrimaryButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr);
};
}
