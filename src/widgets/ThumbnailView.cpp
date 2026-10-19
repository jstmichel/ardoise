#include <Ardoise/Theme.h>
#include <Ardoise/ThumbnailDelegate.h>
#include <Ardoise/ThumbnailView.h>
namespace Ardoise
{
ThumbnailView::ThumbnailView(QWidget* parent)
    : QListView(parent)
{
    const auto& m = Theme::tokens().metrics;
    setItemDelegate(new ThumbnailDelegate(this));
    setViewMode(IconMode);
    setFlow(LeftToRight);
    setWrapping(false);
    setMovement(Static);
    setResizeMode(Adjust);
    setIconSize(m.thumbnailSize);
    setGridSize(m.thumbnailSize + QSize(m.spacing8, m.spacing8));
    setSpacing(m.spacing2);
    setSelectionMode(SingleSelection);
    setEditTriggers(NoEditTriggers);
    setHorizontalScrollMode(ScrollPerPixel);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setFixedHeight(m.thumbnailSize.height() + m.spacing16 + m.scrollbarExtent);
    auto p = palette();
    p.setColor(QPalette::Base, Qt::transparent);
    setPalette(p);
    setAutoFillBackground(false);
    viewport()->setAutoFillBackground(false);
}
}
