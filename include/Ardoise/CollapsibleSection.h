#pragma once
#include <QWidget>
class QToolButton;
class QVBoxLayout;
class QHBoxLayout;
namespace Ardoise
{
class CollapsibleSection : public QWidget
{
    Q_OBJECT
public:
    CollapsibleSection(
        const QString& title, QWidget* content, bool expanded = true, QWidget* parent = nullptr);
    void setTitle(const QString& title);
    void setExpanded(bool expanded);
    bool isExpanded() const;
    void setContentWidget(QWidget* content);
    void setAccessoryWidget(QWidget* accessory);
    void setSeparatorVisible(bool visible);
public slots:
    void toggle();
signals:
    void expandedChanged(bool expanded);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    bool m_separatorVisible = true;
    QToolButton* m_header;
    QWidget* m_content = nullptr;
    QWidget* m_accessory = nullptr;
    QVBoxLayout* m_layout;
    QHBoxLayout* m_headerLayout;
};
}
