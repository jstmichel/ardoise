#include <Ardoise/Theme.h>
#include <Ardoise/ThumbnailDelegate.h>
#include <QIcon>
#include <QPainter>
#include <QPainterPath>
namespace Ardoise
{
QSize ThumbnailDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const
{
    const auto& m = Theme::tokens().metrics;
    return m.thumbnailSize + QSize(m.spacing8, m.spacing8);
}
void ThumbnailDelegate::paint(QPainter* p, const QStyleOptionViewItem& o, const QModelIndex& index) const
{
    const auto& t = Theme::tokens();
    const auto& m = t.metrics;
    const auto& c = t.colors;
    const bool selected = o.state & QStyle::State_Selected;
    const QRect rect = o.rect.adjusted(m.spacing2, m.spacing2, -m.spacing2, -m.spacing2);
    p->save();
    p->setRenderHint(QPainter::Antialiasing);
    p->setRenderHint(QPainter::SmoothPixmapTransform);
    p->setPen(QPen(selected ? c.accent : c.borderNormal, selected ? m.focusBorderWidth : m.borderWidth));
    p->setBrush(c.inputBackground);
    p->drawRoundedRect(rect, m.radiusMedium, m.radiusMedium);
    const QRect imageRect = rect.adjusted(m.spacing4, m.spacing4, -m.spacing4, -m.spacing4);
    const auto icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    const auto pixmap = icon.pixmap(imageRect.size(), p->device()->devicePixelRatioF());
    QSize size = pixmap.size() / pixmap.devicePixelRatio();
    size.scale(imageRect.size(), Qt::KeepAspectRatio);
    QRect target(QPoint(), size);
    target.moveCenter(imageRect.center());
    QPainterPath clip;
    clip.addRoundedRect(imageRect, m.radiusSmall, m.radiusSmall);
    p->setClipPath(clip);
    p->drawPixmap(target, pixmap);
    p->setClipping(false);
    if (o.state & QStyle::State_HasFocus)
    {
        p->setBrush(Qt::NoBrush);
        p->setPen(QPen(c.borderAccent, m.borderWidth));
        p->drawRoundedRect(rect.adjusted(-1, -1, 1, 1), m.radiusMedium, m.radiusMedium);
    }
    p->restore();
}
}
