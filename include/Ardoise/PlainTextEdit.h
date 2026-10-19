#pragma once
#include <QPlainTextEdit>
class QLabel;
namespace Ardoise
{
class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit PlainTextEdit(QWidget* parent = nullptr);
    void setCounterVisible(bool visible);
    void setReferenceLength(int length);
    int heightForLines(int lineCount) const;

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void updateCounter();
    QLabel* m_counter;
    int m_reference = 0;
    bool m_showCounter = false;
};
}
