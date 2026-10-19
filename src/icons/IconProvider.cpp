#include <Ardoise/IconProvider.h>
#include <Ardoise/Theme.h>
#include <QIconEngine>
#include <QPainter>
#include <QResource>
#include <QSvgRenderer>
static void initializeIcons() { Q_INIT_RESOURCE(icons); }
namespace Ardoise
{
namespace
{
    class SvgIconEngine final : public QIconEngine
    {
    public:
        SvgIconEngine(QString path, IconTone tone)
            : m_path(std::move(path))
            , m_tone(tone)
        {
        }
        QIconEngine* clone() const override { return new SvgIconEngine(m_path, m_tone); }
        QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) override
        {
            return scaledPixmap(size, mode, state, 1);
        }
        QPixmap scaledPixmap(const QSize& size, QIcon::Mode mode, QIcon::State, qreal scale) override
        {
            QImage image(size * scale, QImage::Format_ARGB32_Premultiplied);
            image.setDevicePixelRatio(scale);
            image.fill(Qt::transparent);
            QPainter painter(&image);
            QSvgRenderer renderer(m_path);
            renderer.render(&painter, QRectF(QPointF(), QSizeF(size)));
            const auto& c = Theme::tokens().colors;
            const QColor color = mode == QIcon::Disabled ? c.textDisabled
                : m_tone == IconTone::Accent             ? c.borderAccent
                : m_tone == IconTone::Danger             ? c.danger
                : m_tone == IconTone::Muted              ? c.textMuted
                                                         : c.textPrimary;
            painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
            painter.fillRect(QRectF(QPointF(), QSizeF(size)), color);
            painter.end();
            return QPixmap::fromImage(image);
        }
        void paint(QPainter* p, const QRect& rect, QIcon::Mode mode, QIcon::State state) override
        {
            p->drawPixmap(rect, scaledPixmap(rect.size(), mode, state, p->device()->devicePixelRatioF()));
        }

    private:
        QString m_path;
        IconTone m_tone;
    };
}
QIcon IconProvider::icon(Icon value, IconTone tone)
{
    static const bool initialized = []
    {
        initializeIcons();
        return true;
    }();
    Q_UNUSED(initialized);
    static const char* names[] = { "Search", "Clear", "Plus", "MoreVertical", "ChevronLeft", "ChevronRight",
        "ChevronDown", "ChevronUp", "Minimize", "Maximize", "Restore", "Close", "New", "Open", "Save",
        "Folder", "File", "Wand", "Dice", "Fullscreen", "Settings", "Info", "Quality", "Light", "Camera",
        "Landscape", "Person", "ZoomOut", "ZoomIn", "ActualSize", "Crop", "Upscale", "Downscale",
        "Paperclip", "Sparkles", "IdentityEdit", "Sharpen", "Send", "Upload", "Image" };
    const int index = static_cast<int>(value);
    if (index < 0 || index >= static_cast<int>(sizeof(names) / sizeof(*names)))
        return { };
    return QIcon(new SvgIconEngine(
        QStringLiteral(":/ardoise/icons/%1.svg").arg(QString::fromLatin1(names[index])), tone));
}
}
