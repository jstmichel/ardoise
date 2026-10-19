#pragma once
#include <QIcon>
namespace Ardoise
{
enum class Icon
{
    Search,
    Clear,
    Plus,
    MoreVertical,
    ChevronLeft,
    ChevronRight,
    ChevronDown,
    ChevronUp,
    Minimize,
    Maximize,
    Restore,
    Close,
    New,
    Open,
    Save,
    Folder,
    File,
    Wand,
    Dice,
    Fullscreen,
    Settings,
    Info,
    Quality,
    Light,
    Camera,
    Landscape,
    Person,
    ZoomOut,
    ZoomIn,
    ActualSize,
    Crop,
    Upscale,
    Downscale,
    Paperclip,
    Sparkles,
    IdentityEdit,
    Sharpen,
    Send,
    Upload,
    Image
};
enum class IconTone
{
    Default,
    Muted,
    Accent,
    Danger
};
class IconProvider
{
public:
    static QIcon icon(Icon icon, IconTone tone = IconTone::Default);
};
}
