#pragma once
#include <QColor>
#include <QSize>

namespace Ardoise
{
struct Colors
{
    QColor backgroundPrimary { "#0f1722" }, backgroundSecondary { "#141c29" },
        backgroundElevated { "#1b2332" };
    QColor panelBackground { "#131b28" }, panelBackgroundHover { "#1d2638" },
        panelBackgroundSelected { "#5737c2" };
    QColor inputBackground { "#151e2b" }, inputBackgroundHover { "#1a2434" },
        inputBackgroundFocus { "#182131" };
    QColor borderSubtle { "#2b3449" }, borderNormal { "#3a425a" }, borderHover { "#58617c" },
        borderAccent { "#9569fa" };
    QColor accent { "#7041e8" }, accentHover { "#8253f4" }, accentPressed { "#5d32c7" },
        accentMuted { "#342553" };
    QColor textPrimary { "#f0f2fa" }, textSecondary { "#bac5de" }, textMuted { "#98a7c2" },
        textDisabled { "#69748b" };
    QColor selectionBackground { "#5737c2" }, selectionText { "#ffffff" };
    QColor danger { "#ec7188" }, warning { "#e5b76e" }, success { "#77c9a4" }, separator { "#2a3448" };
};
struct Metrics
{
    int spacing2 = 2, spacing4 = 4, spacing6 = 6, spacing8 = 8, spacing12 = 12, spacing16 = 16,
        spacing20 = 20, spacing24 = 24;
    int radiusSmall = 4, radiusMedium = 6, radiusLarge = 10, borderWidth = 1, focusBorderWidth = 2;
    int controlHeightSmall = 28, controlHeight = 32, controlHeightLarge = 44;
    int iconSizeSmall = 16, iconSize = 20, iconSizeLarge = 28;
    int sidebarWidthDefault = 320, inspectorWidthDefault = 390, sidebarMinimumWidth = 240,
        inspectorMinimumWidth = 320;
    int titleBarHeight = 48, windowButtonWidth = 48, treeRowHeight = 26, treeIndentation = 20;
    int sectionHeaderHeight = 38, scrollbarExtent = 10, scrollbarThumbExtent = 6, scrollbarMinimumThumb = 28;
    int splitterHandleWidth = 6, resizeMargin = 6, thumbnailGap = 8, historyHeadingHeight = 32;
    QSize thumbnailSize { 112, 112 };
    int promptVisibleLines = 3;
    int fragmentDetailsPreferredHeight = 180, fragmentDetailsMaximumHeight = 240;
};
enum class TextRole
{
    Body,
    BodyStrong,
    Caption,
    SectionTitle,
    PageTitle
};
struct Typography
{
    qreal minimumBodyPoints = 10.5, captionOffset = -1, sectionOffset = 1, pageOffset = 5;
};
struct ThemeTokens
{
    Colors colors;
    Metrics metrics;
    Typography typography;
};
}
