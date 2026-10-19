#include <Ardoise/Theme.h>
#include <Ardoise/TreeView.h>
namespace Ardoise
{
TreeView::TreeView(QWidget* parent)
    : QTreeView(parent)
{
    setHeaderHidden(true);
    setEditTriggers(NoEditTriggers);
    setSelectionMode(SingleSelection);
    setUniformRowHeights(true);
    setIndentation(Theme::tokens().metrics.treeIndentation);
    setFrameShape(QFrame::NoFrame);
    setMouseTracking(true);
    setHorizontalScrollMode(ScrollPerPixel);
    setVerticalScrollMode(ScrollPerPixel);
    auto p = palette();
    p.setColor(QPalette::Base, Qt::transparent);
    setPalette(p);
    setAutoFillBackground(false);
    viewport()->setAutoFillBackground(false);
}
}
