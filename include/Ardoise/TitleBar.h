#pragma once
#include <QIcon>
#include <QWidget>
class QLabel;
class QMenu;
namespace Ardoise
{
class IconButton;
class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget* parent = nullptr);
    void setTitle(const QString& title);
    void setIcon(const QIcon& icon);
    void setApplicationMenu(QMenu* menu);
    void setWindowState(Qt::WindowStates state);
    void setDialogControls(bool dialog);
signals:
    void minimizeRequested();
    void maximizeRestoreRequested();
    void closeRequested();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    QLabel* m_icon;
    QLabel* m_title;
    IconButton* m_menu;
    IconButton* m_minimize;
    IconButton* m_maximize;
    bool m_dialog = false;
    bool m_dragging = false;
    QPoint m_dragOffset;
};
}
