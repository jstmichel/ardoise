#include <Ardoise/IconProvider.h>
#include <Ardoise/MainWindow.h>
#include <Ardoise/PrimaryButton.h>
#include <Ardoise/Theme.h>
#include <Ardoise/Version.h>
#include <QApplication>
#include <concepts>
#include <iostream>

static_assert(std::derived_from<Ardoise::MainWindow, QWidget>);
int main(int argc, char** argv)
{
    QApplication application(argc, argv);
    Ardoise::Theme::apply(application);
    Ardoise::MainWindow window;
    auto* button = new Ardoise::PrimaryButton(QStringLiteral("Ardoise"));
    button->setIcon(Ardoise::IconProvider::icon(Ardoise::Icon::Wand));
    window.setContentWidget(button);
    if (button->icon().pixmap(20, 20).isNull())
        return 1;
    std::cout << "Ardoise " << Ardoise::version() << '\n';
    return 0;
}
