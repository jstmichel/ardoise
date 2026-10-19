#include <Ardoise/Dialog.h>
#include <Ardoise/Theme.h>
#include <QVBoxLayout>
namespace Ardoise
{
Dialog::Dialog(QWidget* parent)
    : QDialog(parent)
{
    m_body = new QVBoxLayout(this);
    const int margin = Theme::tokens().metrics.spacing16;
    m_body->setContentsMargins(margin, margin, margin, margin);
    m_body->setSpacing(0);
}
void Dialog::setContentWidget(QWidget* widget)
{
    if (widget == m_content)
        return;
    if (m_content)
    {
        m_body->removeWidget(m_content);
        delete m_content;
    }
    m_content = widget;
    if (widget)
        m_body->addWidget(widget);
}
}
