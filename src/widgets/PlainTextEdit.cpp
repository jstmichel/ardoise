#include <Ardoise/PlainTextEdit.h>
#include <Ardoise/Theme.h>
#include <QLabel>
#include <QScrollBar>
#include <QTextDocument>
#include <QtMath>
namespace Ardoise
{
PlainTextEdit::PlainTextEdit(QWidget* parent)
    : QPlainTextEdit(parent)
    , m_counter(new QLabel(this))
{
    setFrameShape(QFrame::StyledPanel);
    setLineWrapMode(QPlainTextEdit::WidgetWidth);
    document()->setDocumentMargin(Theme::tokens().metrics.spacing8);
    Theme::applyTypography(m_counter, TextRole::Caption);
    m_counter->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    auto p = m_counter->palette();
    p.setColor(QPalette::WindowText, Theme::tokens().colors.textMuted);
    m_counter->setPalette(p);
    m_counter->setAutoFillBackground(false);
    m_counter->setAttribute(Qt::WA_TransparentForMouseEvents);
    connect(this, &QPlainTextEdit::textChanged, this, &PlainTextEdit::updateCounter);
    updateCounter();
}
void PlainTextEdit::setCounterVisible(bool visible)
{
    m_showCounter = visible;
    updateCounter();
}
void PlainTextEdit::setReferenceLength(int length)
{
    m_reference = qMax(0, length);
    updateCounter();
}
int PlainTextEdit::heightForLines(int lineCount) const
{
    const auto& m = Theme::tokens().metrics;
    const int counterHeight = m_showCounter ? m_counter->fontMetrics().height() + m.spacing6 : 0;
    const QFontMetricsF metrics(font());
    // Round each line up: fractional font heights otherwise clip the last line.
    const int lineHeight = qCeil(qMax(metrics.height(), metrics.lineSpacing()));
    return qMax(1, lineCount) * lineHeight + qCeil(2 * document()->documentMargin())
        + counterHeight + 2 * (frameWidth() + m.borderWidth) + m.spacing2;
}
void PlainTextEdit::resizeEvent(QResizeEvent* event)
{
    QPlainTextEdit::resizeEvent(event);
    updateCounter();
}
void PlainTextEdit::updateCounter()
{
    const auto& m = Theme::tokens().metrics;
    const int counterHeight = m_showCounter ? m_counter->fontMetrics().height() + m.spacing6 : 0;
    setViewportMargins(m.borderWidth, m.borderWidth, m.borderWidth, counterHeight + m.borderWidth);
    m_counter->setVisible(m_showCounter);
    m_counter->setText(m_reference > 0
            ? QStringLiteral("%1 / %2").arg(toPlainText().size()).arg(m_reference)
            : QString::number(toPlainText().size()));
    const int scrollbarWidth = verticalScrollBar()->isVisible() ? verticalScrollBar()->width() : 0;
    m_counter->setGeometry(m.spacing8, height() - counterHeight - m.borderWidth,
        qMax(0, width() - 2 * m.spacing8 - scrollbarWidth), counterHeight);
}
}
