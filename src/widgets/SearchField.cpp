#include <Ardoise/IconProvider.h>
#include <Ardoise/SearchField.h>
#include <Ardoise/Theme.h>
#include <QAction>
namespace Ardoise
{
SearchField::SearchField(QWidget* parent)
    : QLineEdit(parent)
{
    const auto& m = Theme::tokens().metrics;
    setMinimumHeight(m.controlHeight + m.spacing8);
    setTextMargins(m.spacing4, 0, m.spacing4, 0);
    addAction(IconProvider::icon(Icon::Search), LeadingPosition);
    setClearButtonEnabled(true);
}
}
