#pragma once
#include <QMainWindow>
class QVBoxLayout;
namespace Ardoise
{
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    void setContentWidget(QWidget* widget);
    QWidget* contentWidget() const { return m_content; }

private:
    QVBoxLayout* m_contentLayout;
    QWidget* m_content = nullptr;
};
}
