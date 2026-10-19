#pragma once
#include <Ardoise/ThemeTokens.h>
#include <QFont>
#include <QLocale>
#include <QPalette>
class QApplication;
class QWidget;
namespace Ardoise
{
class Theme
{
public:
    static const ThemeTokens& tokens();
    static QPalette palette();
    static QFont font(TextRole role = TextRole::Body);
    static void applyTypography(QWidget* widget, TextRole role);
    static void apply(QApplication& application);
    static void installTranslations(QApplication& application, const QLocale& locale = QLocale());
};
}
