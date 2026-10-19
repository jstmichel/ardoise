#include <Ardoise/IconButton.h>
#include <Ardoise/Theme.h>
#include <Ardoise/TitleBar.h>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QWindow>
namespace Ardoise
{
TitleBar::TitleBar(QWidget* parent)
    : QWidget(parent)
{
    const auto& m = Theme::tokens().metrics;
    setObjectName(QStringLiteral("ardoiseTitleBar"));
    setFixedHeight(qMax(m.titleBarHeight, fontMetrics().height() + m.spacing24));
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(m.spacing16, m.spacing4, m.spacing8, m.spacing4);
    layout->setSpacing(m.spacing12);
    m_icon = new QLabel(this);
    m_icon->setFixedSize(m.iconSizeLarge, m.iconSizeLarge);
    m_icon->setAttribute(Qt::WA_TransparentForMouseEvents);
    layout->addWidget(m_icon);
    m_title = new QLabel(this);
    Theme::applyTypography(m_title, TextRole::SectionTitle);
    m_title->setAttribute(Qt::WA_TransparentForMouseEvents);
    layout->addWidget(m_title);
    m_menu = new IconButton(Icon::MoreVertical, tr("Application menu"), this);
    m_menu->setObjectName(QStringLiteral("applicationMenuButton"));
    m_menu->setPopupMode(QToolButton::InstantPopup);
    m_menu->hide();
    layout->addWidget(m_menu);
    layout->addStretch();
    m_minimize = new IconButton(Icon::Minimize, tr("Minimize"), this);
    m_maximize = new IconButton(Icon::Maximize, tr("Maximize"), this);
    auto* close = new IconButton(Icon::Close, tr("Close"), this);
    close->setObjectName(QStringLiteral("windowCloseButton"));
    close->setTone(ButtonTone::Destructive);
    m_minimize->setObjectName(QStringLiteral("windowMinimizeButton"));
    m_maximize->setObjectName(QStringLiteral("windowMaximizeButton"));
    for (auto* button : { m_minimize, m_maximize, close })
    {
        button->setProperty("ardoiseFlat", true);
        button->setFixedWidth(m.windowButtonWidth);
        layout->addWidget(button);
    }
    connect(m_minimize, &QToolButton::clicked, this, &TitleBar::minimizeRequested);
    connect(m_maximize, &QToolButton::clicked, this, &TitleBar::maximizeRestoreRequested);
    connect(close, &QToolButton::clicked, this, &TitleBar::closeRequested);
}
void TitleBar::setTitle(const QString& title) { m_title->setText(title); }
void TitleBar::setIcon(const QIcon& icon)
{
    m_icon->setPixmap(icon.pixmap(m_icon->size(), devicePixelRatioF()));
}
void TitleBar::setApplicationMenu(QMenu* menu)
{
    m_menu->setMenu(menu);
    m_menu->setVisible(menu != nullptr);
}
void TitleBar::setWindowState(Qt::WindowStates state)
{
    const bool restore = state.testFlag(Qt::WindowMaximized) || state.testFlag(Qt::WindowFullScreen);
    m_maximize->setIcon(restore ? Icon::Restore : Icon::Maximize);
    m_maximize->setToolTip(restore ? tr("Restore") : tr("Maximize"));
    m_maximize->setAccessibleName(m_maximize->toolTip());
}
void TitleBar::setDialogControls(bool dialog)
{
    m_dialog = dialog;
    m_minimize->setVisible(!dialog);
    m_maximize->setVisible(!dialog);
}
void TitleBar::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && window()->windowHandle())
    {
        if (window()->windowHandle()->startSystemMove())
        {
            event->accept();
            return;
        }
        if (!QGuiApplication::platformName().contains(QStringLiteral("wayland")) && !window()->isMaximized()
            && !window()->isFullScreen())
        {
            m_dragging = true;
            m_dragOffset = event->globalPosition().toPoint() - window()->pos();
            event->accept();
            return;
        }
    }
    QWidget::mousePressEvent(event);
}
void TitleBar::mouseMoveEvent(QMouseEvent* event)
{
    if (m_dragging && event->buttons().testFlag(Qt::LeftButton))
    {
        window()->move(event->globalPosition().toPoint() - m_dragOffset);
        event->accept();
        return;
    }
    QWidget::mouseMoveEvent(event);
}
void TitleBar::mouseReleaseEvent(QMouseEvent* event)
{
    m_dragging = false;
    QWidget::mouseReleaseEvent(event);
}
void TitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && !m_dialog)
    {
        m_dragging = false;
        emit maximizeRestoreRequested();
        event->accept();
        return;
    }
    QWidget::mouseDoubleClickEvent(event);
}
}
