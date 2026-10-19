#include <Ardoise/MainWindow.h>
#include <Ardoise/Theme.h>
#include <QVBoxLayout>
namespace Ardoise
{
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    auto* body = new QWidget(this);
    m_contentLayout = new QVBoxLayout(body);
    const int margin = Theme::tokens().metrics.spacing8;
    m_contentLayout->setContentsMargins(margin, margin, margin, margin);
    m_contentLayout->setSpacing(0);
    QMainWindow::setCentralWidget(body);
}
void MainWindow::setContentWidget(QWidget* widget)
{
    if (widget == m_content)
        return;
    if (m_content)
    {
        m_contentLayout->removeWidget(m_content);
        delete m_content;
    }
    m_content = widget;
    if (widget)
        m_contentLayout->addWidget(widget);
}
}
