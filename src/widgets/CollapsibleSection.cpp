#include <Ardoise/CollapsibleSection.h>
#include <Ardoise/IconProvider.h>
#include <Ardoise/Theme.h>
#include <QApplication>
#include <QHBoxLayout>
#include <QPainter>
#include <QToolButton>
#include <QVBoxLayout>
namespace Ardoise
{
CollapsibleSection::CollapsibleSection(
    const QString& title, QWidget* content, bool expanded, QWidget* parent)
    : QWidget(parent)
{
    const auto& m = Theme::tokens().metrics;
    // Extra height belongs to the containing layout, not between header and content.
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, m.spacing8);
    m_layout->setSpacing(m.spacing4);
    m_headerLayout = new QHBoxLayout;
    m_headerLayout->setContentsMargins(0, 0, 0, 0);
    m_layout->addLayout(m_headerLayout);
    m_header = new QToolButton(this);
    m_header->setCheckable(true);
    m_header->setChecked(expanded);
    m_header->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_header->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_header->setMinimumHeight(m.sectionHeaderHeight);
    m_header->setFocusPolicy(Qt::StrongFocus);
    m_header->setProperty("ardoiseSectionHeader", true);
    Theme::applyTypography(m_header, TextRole::BodyStrong);
    m_headerLayout->addWidget(m_header, 1);
    setTitle(title);
    connect(m_header, &QToolButton::toggled, this,
        [this](bool value)
        {
            if (m_content)
            {
                if (!value && m_content->isAncestorOf(QApplication::focusWidget()))
                    m_header->setFocus();
                m_content->setVisible(value);
            }
            m_header->setIcon(IconProvider::icon(value ? Icon::ChevronDown : Icon::ChevronRight));
            emit expandedChanged(value);
        });
    setContentWidget(content);
    setExpanded(expanded);
}
void CollapsibleSection::setTitle(const QString& title)
{
    m_header->setText(title);
    m_header->setAccessibleName(title);
}
void CollapsibleSection::setExpanded(bool expanded)
{
    m_header->setChecked(expanded);
    if (m_content)
        m_content->setVisible(expanded);
    m_header->setIcon(IconProvider::icon(expanded ? Icon::ChevronDown : Icon::ChevronRight));
}
bool CollapsibleSection::isExpanded() const { return m_header->isChecked(); }
void CollapsibleSection::toggle() { setExpanded(!isExpanded()); }
void CollapsibleSection::setContentWidget(QWidget* content)
{
    if (content == m_content)
        return;
    if (m_content)
    {
        m_layout->removeWidget(m_content);
        delete m_content;
    }
    m_content = content;
    if (content)
    {
        content->setParent(this);
        m_layout->addWidget(content);
        content->setVisible(isExpanded());
    }
}
void CollapsibleSection::setAccessoryWidget(QWidget* accessory)
{
    if (accessory == m_accessory)
        return;
    if (m_accessory)
    {
        m_headerLayout->removeWidget(m_accessory);
        delete m_accessory;
    }
    m_accessory = accessory;
    if (accessory)
        m_headerLayout->addWidget(accessory);
}
void CollapsibleSection::setSeparatorVisible(bool visible)
{
    if (m_separatorVisible == visible)
        return;
    m_separatorVisible = visible;
    update();
}
void CollapsibleSection::paintEvent(QPaintEvent*)
{
    if (!m_separatorVisible)
        return;
    QPainter p(this);
    p.setPen(Theme::tokens().colors.separator);
    p.drawLine(0, height() - 1, width(), height() - 1);
}
}
