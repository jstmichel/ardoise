#include <Ardoise/CollapsibleSection.h>
#include <Ardoise/IconProvider.h>
#include <Ardoise/PlainTextEdit.h>
#include <Ardoise/Panel.h>
#include <Ardoise/Theme.h>
#include <Ardoise/ThumbnailView.h>
#include <Ardoise/TreeView.h>
#include <QLabel>
#include <QLineEdit>
#include <QScrollBar>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QStyleOptionSlider>
#include <QStyleFactory>
#include <QStandardItemModel>
#include <QToolButton>
#include <QtTest>
class TestScrollBar : public QScrollBar
{
public:
    using QScrollBar::QScrollBar;
    using QScrollBar::initStyleOption;
};
class WidgetsTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase() { Ardoise::Theme::apply(*qApp); }
    void scrollAreaPreservesParentSurface()
    {
        Ardoise::Panel panel;
        panel.resize(320, 240);
        auto* layout = new QVBoxLayout(&panel);
        auto* area = new QScrollArea;
        area->setFrameShape(QFrame::NoFrame);
        area->setWidgetResizable(true);
        layout->addWidget(area);
        area->setWidget(new QWidget);
        panel.show();
        QCoreApplication::processEvents();
        const auto verifySurface = [&]
        {
            QVERIFY(!area->autoFillBackground());
            QVERIFY(!area->viewport()->autoFillBackground());
            QVERIFY(!area->widget()->autoFillBackground());
            const QPoint point = area->widget()->mapTo(&panel, QPoint(20, 100));
            const auto image = panel.grab().toImage();
            const auto dpr = image.devicePixelRatio();
            QCOMPARE(image.pixelColor(point * dpr), image.pixelColor(QPoint(4, point.y()) * dpr));
            for (int y : { 20, panel.height() / 2, panel.height() - 20 })
                QCOMPARE(image.pixelColor(QPoint(panel.width() / 2, y) * dpr),
                    Ardoise::Theme::tokens().colors.panelBackground);
        };
        verifySurface();
        area->setWidget(new QWidget);
        QCoreApplication::processEvents();
        verifySurface();
    }
    void scrollbarTrackClicksAndWideThumbHitArea()
    {
        for (auto orientation : {Qt::Vertical, Qt::Horizontal})
            for (bool inverted : {false, true})
            {
                TestScrollBar bar(orientation);
                bar.setRange(0, 100);
                bar.setPageStep(20);
                bar.setInvertedAppearance(inverted);
                bar.setValue(50);
                bar.resize(orientation == Qt::Vertical ? QSize(10, 200) : QSize(200, 10));
                bar.show();
                QCoreApplication::processEvents();
                QStyleOptionSlider option;
                bar.initStyleOption(&option);
                const auto thumb = bar.style()->subControlRect(QStyle::CC_ScrollBar, &option,
                    QStyle::SC_ScrollBarSlider, &bar);
                // The edge of the lane is outside the painted thumb but remains draggable.
                QPoint edge = thumb.center();
                if (orientation == Qt::Vertical) edge.setX(thumb.left());
                else edge.setY(thumb.top());
                QCOMPARE(bar.style()->hitTestComplexControl(QStyle::CC_ScrollBar, &option, edge, &bar),
                    QStyle::SC_ScrollBarSlider);
                const QPoint pagePoint = orientation == Qt::Vertical ? QPoint(5, 190) : QPoint(190, 5);
                QTest::mouseClick(&bar, Qt::LeftButton, Qt::NoModifier, pagePoint);
                QCOMPARE(bar.value(), inverted ? 30 : 70);
                bar.setValue(50);
                QTest::mousePress(&bar, Qt::LeftButton, Qt::NoModifier, edge);
                const QPoint destination = edge + (orientation == Qt::Vertical ? QPoint(0, 30) : QPoint(30, 0));
                QTest::mouseMove(&bar, destination);
                QTest::mouseRelease(&bar, Qt::LeftButton, Qt::NoModifier, destination);
                QVERIFY(inverted ? bar.value() < 50 : bar.value() > 50);
                // Preserve Qt's keyboard semantics, including inverted appearance.
                QScrollBar reference(orientation);
                auto* fusion = QStyleFactory::create(QStringLiteral("Fusion"));
                fusion->setParent(&reference);
                reference.setStyle(fusion);
                reference.setRange(0, 100);
                reference.setInvertedAppearance(inverted);
                reference.setValue(50);
                bar.setValue(50);
                const auto key = orientation == Qt::Vertical ? Qt::Key_Down : Qt::Key_Right;
                QTest::keyClick(&bar, key);
                QTest::keyClick(&reference, key);
                QCOMPARE(bar.value(), reference.value());
            }
    }
    void collapseKeepsKeyboardFocus()
    {
        auto* content = new QLineEdit;
        Ardoise::CollapsibleSection section(QStringLiteral("Section"), content, true);
        section.resize(320, 100);
        section.show();
        section.activateWindow();
        content->setFocus();
        QTest::qWait(20);
        QSignalSpy spy(&section, &Ardoise::CollapsibleSection::expandedChanged);
        section.setExpanded(false);
        QVERIFY(content->isHidden());
        QCOMPARE(spy.count(), 1);
        auto* header = section.findChild<QToolButton*>();
        QVERIFY(header);
        QVERIFY(header->hasFocus());
        QTest::keyClick(header, Qt::Key_Space);
        QVERIFY(section.isExpanded());
        QVERIFY(!content->isHidden());
    }
    void counterPreservesLongTextAndUndo()
    {
        Ardoise::PlainTextEdit editor;
        editor.setCounterVisible(true);
        editor.setReferenceLength(2000);
        editor.resize(300, 140);
        editor.show();
        editor.setPlainText(QString(2100, QLatin1Char('a')));
        QCOMPARE(editor.toPlainText().size(), 2100);
        auto* counter = editor.findChild<QLabel*>();
        QVERIFY(counter);
        QCOMPARE(counter->text(), QStringLiteral("2100 / 2000"));
        QVERIFY(editor.viewport()->geometry().bottom() < counter->geometry().top());
        QCoreApplication::processEvents();
        QVERIFY(editor.verticalScrollBar()->isVisible());
        const auto scrollbarRect = QRect(editor.verticalScrollBar()->mapTo(&editor, QPoint()), editor.verticalScrollBar()->size());
        QVERIFY(!counter->geometry().intersects(scrollbarRect));
        editor.selectAll();
        QTest::keyClicks(&editor, "replacement");
        editor.undo();
        QCOMPARE(editor.toPlainText().size(), 2100);
    }
    void counterSharesTextFieldBackground()
    {
        Ardoise::PlainTextEdit editor;
        editor.setCounterVisible(true);
        editor.setReferenceLength(2000);
        editor.resize(300, 140);
        editor.show();
        QCoreApplication::processEvents();
        auto* counter = editor.findChild<QLabel*>();
        QVERIFY(counter);
        const auto verifySurface = [&]
        {
            const auto image = editor.grab().toImage();
            const qreal dpr = image.devicePixelRatio();
            const QPoint textPoint = editor.viewport()->mapTo(&editor, QPoint(20, 20));
            const QPoint countPoint(counter->geometry().left() + 20, counter->geometry().center().y());
            const QPoint footerPoint(3, counter->geometry().center().y());
            QCOMPARE(image.pixelColor(textPoint * dpr), Ardoise::Theme::tokens().colors.inputBackground);
            QCOMPARE(image.pixelColor(countPoint * dpr), image.pixelColor(textPoint * dpr));
            QCOMPARE(image.pixelColor(footerPoint * dpr), image.pixelColor(textPoint * dpr));
        };
        verifySurface();
        editor.setFocus();
        QTest::mouseMove(&editor, QPoint(20, 20));
        QCoreApplication::processEvents();
        verifySurface();
    }
    void iconResourcesAndScale()
    {
        for (int i = 0; i <= static_cast<int>(Ardoise::Icon::Image); ++i)
        {
            auto icon = Ardoise::IconProvider::icon(static_cast<Ardoise::Icon>(i));
            auto p = icon.pixmap(QSize(24, 24), 2.0);
            QVERIFY(!p.isNull());
            QCOMPARE(p.deviceIndependentSize(), QSizeF(24, 24));
            bool hasInk = false;
            const auto image = p.toImage();
            for (int y = 0; y < image.height() && !hasInk; ++y)
                for (int x = 0; x < image.width(); ++x)
                    if (qAlpha(image.pixel(x, y)))
                    {
                        hasInk = true;
                        break;
                    }
            QVERIFY(hasInk);
        }
    }
    void treeSelectionStaysOnItemText()
    {
        Ardoise::TreeView view;
        QStandardItemModel model;
        auto* parent = new QStandardItem(QStringLiteral("Parent"));
        auto* child = new QStandardItem(QStringLiteral("Selected value"));
        child->appendRow(new QStandardItem(QStringLiteral("Nested value")));
        parent->appendRow(child);
        model.appendRow(parent);
        view.setModel(&model);
        view.expandAll();
        view.resize(320, 150);
        view.show();
        view.setCurrentIndex(child->index());
        view.clearFocus();
        QCoreApplication::processEvents();
        const auto row = view.visualRect(child->index());
        const auto capture = view.viewport()->grab().toImage();
        const auto selection = Ardoise::Theme::tokens().colors.selectionBackground;
        const int y = row.center().y();
        const auto pixel = [&](int x) { return capture.pixelColor(QPoint(x, y) * capture.devicePixelRatio()); };
        for (int x = 0; x < row.left(); ++x)
            QVERIFY2(pixel(x) != selection, "Selection must not paint indentation or branch guides");
        QVERIFY2(pixel(row.right() - 4) != selection, "Selection must not fill trailing row space");
        bool highlighted = false;
        for (int x = row.left(); x < row.right(); ++x)
            highlighted |= pixel(x) == selection;
        QVERIFY(highlighted);
        view.setFocus();
        QCoreApplication::processEvents();
        const auto focused = view.viewport()->grab().toImage();
        for (int y = row.top(); y <= row.bottom(); ++y)
            QVERIFY2(focused.pixelColor(QPoint(row.right() - 4, y) * focused.devicePixelRatio())
                    != Ardoise::Theme::tokens().colors.borderAccent,
                "Keyboard focus must remain on the item text too");
    }
    void thumbnailRowStaysCompact()
    {
        QStandardItemModel model;
        for (int i = 0; i < 10; ++i)
        {
            auto* item = new QStandardItem;
            QPixmap image(100, 200);
            image.fill(Qt::red);
            item->setIcon(QIcon(image));
            model.appendRow(item);
        }
        Ardoise::ThumbnailView view;
        view.setModel(&model);
        view.resize(400, 400);
        view.show();
        QCoreApplication::processEvents();
        QVERIFY(!view.autoFillBackground());
        QVERIFY(!view.viewport()->autoFillBackground());
        const auto firstRect = view.visualRect(model.index(0, 0));
        QCOMPARE(firstRect.width(), firstRect.height());
        const auto capture = view.viewport()->grab().toImage();
        QRect imageBounds;
        const qreal dpr = capture.devicePixelRatio();
        for (int y = firstRect.top(); y <= firstRect.bottom(); ++y)
            for (int x = firstRect.left(); x <= firstRect.right(); ++x)
                if (capture.pixelColor(QPoint(x, y) * dpr) == QColor(Qt::red))
                    imageBounds = imageBounds.united(QRect(x, y, 1, 1));
        QVERIFY(!imageBounds.isEmpty());
        QVERIFY(qAbs(imageBounds.height() - 2 * imageBounds.width()) <= 2);
        QVERIFY(view.height() < 150);
        QVERIFY(view.horizontalScrollBar()->maximum() > 0);
        QCOMPARE(view.verticalScrollBar()->maximum(), 0);
        view.setCurrentIndex(model.index(0, 0));
        view.setFocus();
        QTest::keyClick(&view, Qt::Key_Right);
        QCOMPARE(view.currentIndex().row(), 1);
    }
};
QTEST_MAIN(WidgetsTest)
#include "WidgetsTest.moc"
