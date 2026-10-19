#include <Ardoise/CollapsibleSection.h>
#include <Ardoise/IconButton.h>
#include <Ardoise/IconProvider.h>
#include <Ardoise/MainWindow.h>
#include <Ardoise/Panel.h>
#include <Ardoise/PlainTextEdit.h>
#include <Ardoise/PrimaryButton.h>
#include <Ardoise/SearchField.h>
#include <Ardoise/Theme.h>
#include <Ardoise/ThumbnailView.h>
#include <Ardoise/TreeView.h>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDir>
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QSlider>
#include <QSpinBox>
#include <QStandardItemModel>
#include <QTimer>
#include <QVBoxLayout>
#include <utility>

// Each group is a panel whose object name is also the file name of its screenshot.
static Ardoise::Panel* addGroup(QVBoxLayout* page, const QString& name, const QString& title, QWidget* content)
{
    const auto& m = Ardoise::Theme::tokens().metrics;
    auto* panel = new Ardoise::Panel;
    panel->setObjectName(name);
    auto* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(m.spacing16, m.spacing16, m.spacing16, m.spacing16);
    layout->setSpacing(m.spacing12);
    auto* heading = new QLabel(title);
    Ardoise::Theme::applyTypography(heading, Ardoise::TextRole::SectionTitle);
    layout->addWidget(heading);
    layout->addWidget(content);
    page->addWidget(panel);
    return panel;
}

static QWidget* buttons()
{
    const auto& m = Ardoise::Theme::tokens().metrics;
    auto* widget = new QWidget;
    auto* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(m.spacing12);
    layout->addWidget(new Ardoise::PrimaryButton(
        Ardoise::IconProvider::icon(Ardoise::Icon::Sparkles), QStringLiteral("Primary action")));
    layout->addWidget(new QPushButton(QStringLiteral("Secondary action")));
    auto* disabled = new Ardoise::PrimaryButton(QStringLiteral("Disabled"));
    disabled->setEnabled(false);
    layout->addWidget(disabled);
    layout->addWidget(new Ardoise::IconButton(Ardoise::Icon::Settings, QStringLiteral("Settings")));
    auto* remove = new Ardoise::IconButton(Ardoise::Icon::Close, QStringLiteral("Remove"));
    remove->setTone(Ardoise::ButtonTone::Destructive);
    layout->addWidget(remove);
    layout->addStretch();
    return widget;
}

static QWidget* inputs(Ardoise::SearchField* search)
{
    auto* widget = new QWidget;
    auto* form = new QFormLayout(widget);
    form->setContentsMargins(0, 0, 0, 0);
    search->setPlaceholderText(QStringLiteral("Search…"));
    form->addRow(QStringLiteral("Search"), search);
    auto* combo = new QComboBox;
    combo->addItems({ QStringLiteral("First choice"), QStringLiteral("Second choice") });
    form->addRow(QStringLiteral("Combo box"), combo);
    auto* spin = new QSpinBox;
    spin->setValue(30);
    form->addRow(QStringLiteral("Spin box"), spin);
    auto* slider = new QSlider(Qt::Horizontal);
    slider->setValue(65);
    form->addRow(QStringLiteral("Slider"), slider);
    auto* check = new QCheckBox(QStringLiteral("Enabled option"));
    check->setChecked(true);
    form->addRow(check);
    return widget;
}

static QWidget* text()
{
    auto* edit = new Ardoise::PlainTextEdit;
    edit->setCounterVisible(true);
    edit->setReferenceLength(2000);
    edit->setPlainText(QStringLiteral("Plain text, keyboard navigation and visible focus."));
    edit->setFixedHeight(edit->heightForLines(3));
    return edit;
}

static QWidget* tree()
{
    auto* view = new Ardoise::TreeView;
    auto* model = new QStandardItemModel(view);
    auto* parent = new QStandardItem(QStringLiteral("A reusable tree"));
    for (int i = 0; i < 20; ++i)
        parent->appendRow(new QStandardItem(QStringLiteral("Item %1").arg(i + 1)));
    model->appendRow(parent);
    view->setModel(model);
    view->expandAll();
    view->setCurrentIndex(model->index(0, 0, parent->index()));
    view->setMinimumHeight(150);
    return new Ardoise::CollapsibleSection(QStringLiteral("Expandable section"), view, true);
}

static QWidget* thumbnails()
{
    auto* view = new Ardoise::ThumbnailView;
    auto* model = new QStandardItemModel(view);
    for (auto icon : { Ardoise::Icon::Landscape, Ardoise::Icon::Camera, Ardoise::Icon::Quality,
             Ardoise::Icon::Person })
    {
        auto* item = new QStandardItem;
        item->setIcon(Ardoise::IconProvider::icon(icon));
        item->setData(QStringLiteral("Sample icon"), Qt::AccessibleTextRole);
        model->appendRow(item);
    }
    view->setModel(model);
    view->setCurrentIndex(model->index(0, 0));
    return view;
}

static QWidget* icons()
{
    using Ardoise::Icon;
    const auto& m = Ardoise::Theme::tokens().metrics;
    auto* widget = new QWidget;
    auto* grid = new QGridLayout(widget);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(m.spacing8);
    const Icon all[] = { Icon::Search, Icon::Clear, Icon::Plus, Icon::MoreVertical, Icon::ChevronLeft,
        Icon::ChevronRight, Icon::ChevronDown, Icon::ChevronUp, Icon::Minimize, Icon::Maximize, Icon::Restore,
        Icon::Close, Icon::New, Icon::Open, Icon::Save, Icon::Folder, Icon::File, Icon::Wand, Icon::Dice,
        Icon::Fullscreen, Icon::Settings, Icon::Info, Icon::Quality, Icon::Light, Icon::Camera,
        Icon::Landscape, Icon::Person, Icon::ZoomOut, Icon::ZoomIn, Icon::ActualSize, Icon::Crop,
        Icon::Upscale, Icon::Downscale, Icon::Paperclip, Icon::Sparkles, Icon::IdentityEdit, Icon::Sharpen,
        Icon::Send, Icon::Upload, Icon::Image };
    const int columns = 10;
    int index = 0;
    for (auto icon : all)
    {
        auto* label = new QLabel;
        label->setPixmap(Ardoise::IconProvider::icon(icon).pixmap(m.iconSize, m.iconSize));
        label->setFixedSize(m.controlHeight, m.controlHeight);
        label->setAlignment(Qt::AlignCenter);
        grid->addWidget(label, index / columns, index % columns);
        ++index;
    }
    return widget;
}

static QWidget* typography()
{
    auto* widget = new QWidget;
    auto* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    const std::pair<Ardoise::TextRole, QString> roles[] = { { Ardoise::TextRole::PageTitle, "Page title" },
        { Ardoise::TextRole::SectionTitle, "Section title" }, { Ardoise::TextRole::BodyStrong, "Body strong" },
        { Ardoise::TextRole::Body, "Body" }, { Ardoise::TextRole::Caption, "Caption" } };
    for (const auto& [role, name] : roles)
    {
        auto* sample = new QLabel(name);
        Ardoise::Theme::applyTypography(sample, role);
        layout->addWidget(sample);
    }
    layout->addStretch();
    return widget;
}

static QWidget* colors()
{
    const auto& tokens = Ardoise::Theme::tokens();
    auto* widget = new QWidget;
    auto* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    for (const auto& color :
        { tokens.colors.backgroundPrimary, tokens.colors.backgroundElevated, tokens.colors.borderNormal,
            tokens.colors.accent, tokens.colors.danger, tokens.colors.warning, tokens.colors.success })
    {
        auto* swatch = new QLabel(color.name());
        swatch->setAlignment(Qt::AlignCenter);
        swatch->setMinimumHeight(tokens.metrics.controlHeightLarge);
        auto palette = swatch->palette();
        palette.setColor(QPalette::Window, color);
        swatch->setPalette(palette);
        swatch->setAutoFillBackground(true);
        layout->addWidget(swatch);
    }
    return widget;
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Ardoise::Theme::apply(app);
    const auto& m = Ardoise::Theme::tokens().metrics;
    Ardoise::MainWindow window;
    window.setWindowTitle(QStringLiteral("Ardoise Gallery"));
    window.resize(880, 920);
    auto* content = new QWidget;
    auto* page = new QVBoxLayout(content);
    page->setSpacing(m.spacing16);
    auto* heading = new QLabel(QStringLiteral("Ardoise"));
    Ardoise::Theme::applyTypography(heading, Ardoise::TextRole::PageTitle);
    page->addWidget(heading);
    auto* search = new Ardoise::SearchField;
    addGroup(page, QStringLiteral("buttons"), QStringLiteral("Buttons"), buttons());
    addGroup(page, QStringLiteral("inputs"), QStringLiteral("Inputs"), inputs(search));
    addGroup(page, QStringLiteral("text"), QStringLiteral("Plain text"), text());
    addGroup(page, QStringLiteral("tree"), QStringLiteral("Tree and collapsible section"), tree());
    addGroup(page, QStringLiteral("thumbnails"), QStringLiteral("Thumbnails"), thumbnails());
    addGroup(page, QStringLiteral("icons"), QStringLiteral("Icons"), icons());
    addGroup(page, QStringLiteral("typography"), QStringLiteral("Typography"), typography());
    addGroup(page, QStringLiteral("colors"), QStringLiteral("Colors"), colors());
    page->addStretch();
    auto* scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setWidget(content);
    window.setContentWidget(scroll);
    window.show();
    search->setFocus();

    // ARDOISE_GALLERY_CAPTURE names a directory: the gallery saves gallery.png and one image
    // per group there, then quits. scripts/screenshots.sh uses it for the README.
    const auto capture = qEnvironmentVariable("ARDOISE_GALLERY_CAPTURE");
    if (!capture.isEmpty())
        QTimer::singleShot(100, &app,
            [&]
            {
                const QDir directory(capture);
                window.grab().save(directory.filePath(QStringLiteral("gallery.png")));
                for (auto* panel : content->findChildren<Ardoise::Panel*>())
                    panel->grab().save(directory.filePath(panel->objectName() + QStringLiteral(".png")));
                app.quit();
            });
    return app.exec();
}
