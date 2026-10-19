#pragma once
#include <QDialog>
class QVBoxLayout;
namespace Ardoise
{
class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(QWidget* parent = nullptr);
    void setContentWidget(QWidget* widget);

private:
    QVBoxLayout* m_body;
    QWidget* m_content = nullptr;
};
}
