#pragma once
#include <QFrame>
namespace Ardoise
{
class Panel : public QFrame
{
    Q_OBJECT
public:
    explicit Panel(QWidget* parent = nullptr);
    void setCornerRadius(int radius);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    int m_radius;
};
}
