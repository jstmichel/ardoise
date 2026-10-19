#include <Ardoise/Style.h>
#include <Ardoise/Theme.h>
#include <QApplication>
#include <QFontDatabase>
#include <QResource>
#include <QTranslator>
#include <QWidget>
static void initializeTranslations() { Q_INIT_RESOURCE(ardoise_translations); }

namespace Ardoise
{
const ThemeTokens& Theme::tokens()
{
    static const ThemeTokens value;
    return value;
}
QPalette Theme::palette()
{
    const auto& c = tokens().colors;
    QPalette p;
    for (auto group : { QPalette::Active, QPalette::Inactive, QPalette::Disabled })
    {
        const bool disabled = group == QPalette::Disabled;
        p.setColor(group, QPalette::Window, c.backgroundPrimary);
        p.setColor(group, QPalette::WindowText, disabled ? c.textDisabled : c.textPrimary);
        p.setColor(group, QPalette::Base, c.inputBackground);
        p.setColor(group, QPalette::AlternateBase, c.backgroundElevated);
        p.setColor(group, QPalette::Text, disabled ? c.textDisabled : c.textPrimary);
        p.setColor(group, QPalette::Button, c.backgroundElevated);
        p.setColor(group, QPalette::ButtonText, disabled ? c.textDisabled : c.textPrimary);
        p.setColor(group, QPalette::Highlight, disabled ? c.accentMuted : c.selectionBackground);
        p.setColor(group, QPalette::HighlightedText, disabled ? c.textMuted : c.selectionText);
        p.setColor(group, QPalette::PlaceholderText, c.textMuted);
        p.setColor(group, QPalette::ToolTipBase, c.backgroundElevated);
        p.setColor(group, QPalette::ToolTipText, c.textPrimary);
        p.setColor(group, QPalette::Light, c.borderHover);
        p.setColor(group, QPalette::Midlight, c.borderNormal);
        p.setColor(group, QPalette::Mid, c.borderNormal);
        p.setColor(group, QPalette::Dark, c.borderSubtle);
        p.setColor(group, QPalette::Shadow, c.backgroundPrimary);
        p.setColor(group, QPalette::Link, c.borderAccent);
        p.setColor(group, QPalette::BrightText, c.selectionText);
    }
    return p;
}
QFont Theme::font(TextRole role)
{
    QFont value = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    const auto& t = tokens().typography;
    qreal points = qMax(t.minimumBodyPoints, value.pointSizeF());
    if (role == TextRole::Caption)
        points = qMax(qreal(9), points + t.captionOffset);
    if (role == TextRole::SectionTitle)
        points += t.sectionOffset;
    if (role == TextRole::PageTitle)
        points += t.pageOffset;
    value.setPointSizeF(points);
    if (role == TextRole::BodyStrong || role == TextRole::SectionTitle)
        value.setWeight(QFont::DemiBold);
    return value;
}
void Theme::applyTypography(QWidget* widget, TextRole role) { widget->setFont(font(role)); }
void Theme::apply(QApplication& application)
{
    application.setStyle(new Style);
    application.setPalette(palette());
    application.setFont(font());
    installTranslations(application);
}

void Theme::installTranslations(QApplication& application, const QLocale& locale)
{
    initializeTranslations();
    auto* old = application.findChild<QTranslator*>(
        QStringLiteral("ardoiseTranslator"), Qt::FindDirectChildrenOnly);
    if (old)
    {
        application.removeTranslator(old);
        delete old;
    }
    auto* translator = new QTranslator(&application);
    translator->setObjectName(QStringLiteral("ardoiseTranslator"));
    const QString language
        = locale.language() == QLocale::French ? QStringLiteral("fr") : QStringLiteral("en");
    if (translator->load(QStringLiteral(":/ardoise/translations/ardoise_%1.qm").arg(language)))
        application.installTranslator(translator);
}
}
