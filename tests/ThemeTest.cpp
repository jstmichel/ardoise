#include <Ardoise/Style.h>
#include <Ardoise/Theme.h>
#include <Ardoise/IconButton.h>
#include <Ardoise/PrimaryButton.h>
#include <QApplication>
#include <QLineEdit>
#include <QStyleFactory>
#include <QSpinBox>
#include <QToolButton>
#include <QHBoxLayout>
#include <QtTest>
class ThemeTest : public QObject
{
    Q_OBJECT
private slots:
    void consistentButtonHeights()
    {
        Ardoise::Theme::apply(*qApp);
        QWidget container;
        auto* layout = new QHBoxLayout(&container);
        Ardoise::PrimaryButton primary(QStringLiteral("Add model"));
        QPushButton secondary(QStringLiteral("Delete"));
        QPushButton optional(QStringLiteral("Optional files..."));
        QToolButton tool;
        tool.setText(QStringLiteral("Action"));
        QSpinBox field;
        Ardoise::IconButton randomize(Ardoise::Icon::Plus, QStringLiteral("Randomize"));
        QCOMPARE(randomize.height(), Ardoise::Theme::tokens().metrics.controlHeight);
        randomize.setFixedSize(field.sizeHint().height(), field.sizeHint().height());
        QWidget* controls[] = { &primary, &secondary, &optional, &tool, &field, &randomize };
        for (QWidget* widget : controls)
            layout->addWidget(widget);
        container.show();
        QCoreApplication::processEvents();
        QCOMPARE(primary.height(), secondary.height());
        QCOMPARE(primary.height(), optional.height());
        QCOMPARE(primary.height(), tool.height());
        QCOMPARE(primary.height(), Ardoise::Theme::tokens().metrics.controlHeightLarge);
        QCOMPARE(randomize.height(), field.height());
    }
    void overridesStartingStyleAndPalette()
    {
        QImage reference;
        for (const QString& style : { QStringLiteral("Fusion"), QStringLiteral("Windows") })
        {
            QApplication::setStyle(QStyleFactory::create(style));
            auto palette = QApplication::palette();
            palette.setColor(QPalette::Base, Qt::yellow);
            QApplication::setPalette(palette);
            Ardoise::Theme::apply(*qApp);
            QVERIFY(qobject_cast<Ardoise::Style*>(qApp->style()));
            QCOMPARE(qApp->palette().color(QPalette::Base), Ardoise::Theme::tokens().colors.inputBackground);
            QLineEdit input(QStringLiteral("Ardoise"));
            input.resize(220, 36);
            input.show();
            QCoreApplication::processEvents();
            const auto capture = input.grab().toImage();
            if (reference.isNull())
                reference = capture;
            else
                QCOMPARE(capture, reference);
        }
    }
};
QTEST_MAIN(ThemeTest)
#include "ThemeTest.moc"
