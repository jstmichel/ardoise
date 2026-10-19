#include <Ardoise/Dialog.h>
#include <Ardoise/MainWindow.h>
#include <Ardoise/Theme.h>
#include <Ardoise/TitleBar.h>
#include <QtTest>
class WindowTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase() { Ardoise::Theme::apply(*qApp); }
    void usesNativeWindowDecorations()
    {
        Ardoise::MainWindow window;
        window.setWindowTitle(QStringLiteral("Test"));
        window.setContentWidget(new QWidget);
        QVERIFY(!window.windowFlags().testFlag(Qt::FramelessWindowHint));
        QVERIFY(window.windowFlags().testFlag(Qt::WindowTitleHint));
        QVERIFY(window.windowFlags().testFlag(Qt::WindowMinimizeButtonHint));
        QVERIFY(window.windowFlags().testFlag(Qt::WindowMaximizeButtonHint));
        QVERIFY(window.windowFlags().testFlag(Qt::WindowCloseButtonHint));
        QVERIFY(!window.findChild<Ardoise::TitleBar*>());
        QCOMPARE(window.windowTitle(), QStringLiteral("Test"));
        QVERIFY(window.contentWidget());
        window.resize(700, 500);
        window.show();
        QCoreApplication::processEvents();
        window.showMaximized();
        QTRY_VERIFY(window.isMaximized());
        window.showNormal();
        QTRY_VERIFY(!window.isMaximized());
        QVERIFY(window.close());
        QVERIFY(!window.isVisible());
    }
    void nativeCloseRejectsDialog()
    {
        Ardoise::Dialog dialog;
        dialog.setContentWidget(new QWidget);
        QVERIFY(!dialog.windowFlags().testFlag(Qt::FramelessWindowHint));
        QVERIFY(dialog.windowFlags().testFlag(Qt::WindowTitleHint));
        QVERIFY(dialog.windowFlags().testFlag(Qt::WindowCloseButtonHint));
        QVERIFY(!dialog.findChild<Ardoise::TitleBar*>());
        dialog.show();
        QSignalSpy spy(&dialog, &QDialog::rejected);
        QVERIFY(dialog.close());
        QCOMPARE(spy.count(), 1);
        QVERIFY(!dialog.isVisible());
    }
    void escapeRejectsDialog()
    {
        Ardoise::Dialog dialog;
        dialog.setContentWidget(new QWidget);
        dialog.show();
        QSignalSpy spy(&dialog, &QDialog::rejected);
        QTest::keyClick(&dialog, Qt::Key_Escape);
        QCOMPARE(spy.count(), 1);
    }
};
QTEST_MAIN(WindowTest)
#include "WindowTest.moc"
