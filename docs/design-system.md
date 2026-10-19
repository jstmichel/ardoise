# Ardoise design system

## Philosophy and platform policy

Ardoise applications share one custom dark visual system on Qt 6 Widgets. Qt supplies
behavior, accessibility, input, layout, and system integration; Ardoise supplies the
visual identity. Linux, Windows, and macOS use the same painters and assets.

Do not introduce Kirigami, AppKit/Cocoa UI, WinUI, QML, or platform-specific visual
widgets. Native file dialogs and Qt window-system operations remain appropriate.
The system UI font is retained; small font/rasterization differences are expected.

## Applying the theme

Link `Ardoise::Widgets`, then call `Ardoise::Theme::apply(application)` immediately
after constructing `QApplication`, before constructing widgets. This explicitly
installs `Ardoise::Style`, its palette, typography, and component translations. There
is no static application mutation. `Style` owns an explicit Fusion base;
Breeze, Windows, and macOS styles cannot change control rendering.

`Theme::tokens()` returns the common immutable `ThemeTokens`. `Theme::palette()`,
`font(TextRole)`, and `applyTypography()` are available independently. Only the
dark theme is implemented. Colors are grouped for a future theme, without a
runtime theme registry or loader.

## Tokens and color rules

`include/Ardoise/ThemeTokens.h` is authoritative. All dimensions are logical pixels;
Qt handles device scaling. Use the spacing scale rather than screen-specific
copies of standard metrics.

| Group | Values / purpose |
| --- | --- |
| Surfaces | Primary `#0f1722`, secondary `#141c29`, elevated `#1b2332`; graphite-blue rather than neutral black |
| Panels | `#131b28`, hover `#1d2638`, selected `#5737c2` |
| Inputs | `#151e2b`, hover `#1a2434`, focused `#182131` |
| Borders | Subtle `#2b3449`, normal `#3a425a`, hover `#58617c`, focus `#9569fa`; muted violet-gray, no glow |
| Accent | `#7041e8`, hover `#8253f4`, pressed `#5d32c7`, disabled/muted `#342553` |
| Text | Primary `#f0f2fa`, secondary `#bac5de`, muted `#98a7c2`, disabled `#69748b` |
| Selection | Violet `#5737c2` with white text |
| Semantic | Danger `#ec7188`, warning `#e5b76e`, success `#77c9a4`; use only for the corresponding meaning |
| Separator | `#2a3448` |
| Spacing | 2, 4, 6, 8, 12, 16, 20, 24 |
| Radius | Small 4, medium 6, large 10 |
| Borders | Normal 1, focused 2 |
| Controls | Small 28, fields and compact icon buttons 32, standalone buttons 44 minimum heights |
| Icons | 16, 20, 28; monochrome SVGs tinted by the theme |
| Columns | Sidebar default 320/minimum 240; inspector default 390/minimum 320 |
| Chrome | Title bar 48 minimum, window buttons 48 wide, resize edge 6 |
| Lists | Tree rows 26, indentation 20, thumbnails 112 × 84, gap 8 |
| Scroll/split | Scrollbar 10, minimum thumb 28, splitter handle 6 |
| Sections | Header 38, history heading 32 |
| Prompts | Three visible lines in both editors; height calculated from font, padding and counter |
| Details | Preferred 180, maximum 240 |

Typography uses Qt's system UI family with a minimum body size of 10.5 points.
`BodyStrong` is bold, `Caption` is one point smaller, `SectionTitle` is one point
larger, and `PageTitle` is five points larger. Font metrics can increase control
sizes; do not override accessibility with fixed text sizes in applications.

Normal borders provide structure. Hover increases contrast, focus adds a clear
accent outline, selection uses a violet surface, and disabled controls mute both
foreground and accent. Buttons also have a pressed state; checkboxes preserve
checked and partially checked states. There are no per-screen stylesheets.

## Component inventory

Public names use the `Ardoise` namespace. Standard Qt controls are intentionally
not subclassed solely to rename them.

| Component / Qt base | Use and implementation | Main tokens |
| --- | --- | --- |
| `MainWindow` / `QMainWindow` | Native decorated window; `setContentWidget`. Use Qt window title/icon APIs and put menus in application content. Owns the content. | Outer spacing, background |
| `Dialog` / `QDialog` | Native decorated modal form; `setContentWidget`; retains accept/reject/Escape behavior. | Content spacing |
| `TitleBar` / `QWidget` | Standalone legacy custom title widget; not composed by MainWindow/Dialog. Native decorations are the default. | Title height, icons, spacing |
| `IconButton` / `QToolButton` | Accessible icon-only action; `setIcon(Icon)`, `setTone`; shared style paints it. | Control height, icons, danger, focus |
| `Panel` / `QFrame` | Large structural region with a solid shared `panelBackground` surface and border; optional `setCornerRadius`. Add a normal Qt layout. | Panel surfaces, normal border, radius |
| `PrimaryButton` / `QPushButton` | Main action; shared style uses `ardoisePrimary` property. Same height as secondary buttons. | Accent states, button height, focus |
| `QPushButton` / `QToolButton` | Standalone action; themed by `Style` with a shared 44 px minimum height. `IconButton` stays compact at the 32 px field height for actions beside inputs; consumers can explicitly size paired buttons to their companion control. | Elevated surface, border, button height |
| `SearchField` / `QLineEdit` | Search icon, clear action, shared sizing; Qt editing/signals unchanged. | Input states, icons, spacing |
| `QLineEdit` | General text field; shared style. | Input states, radius, control height |
| `PlainTextEdit` / `QPlainTextEdit` | Text editor with optional advisory counter (`setCounterVisible`, `setReferenceLength`); `heightForLines()` includes text padding and the counter; never truncates text. | Input, muted caption, spacing |
| `QComboBox` | Native Qt model/selection behavior, custom shared field and arrow rendering. | Input, border, spacing, height |
| `QSpinBox`, `QDoubleSpinBox` | Qt stepping/range/locale with shared field, arrows and geometry. | Input, border, height |
| `QSlider` | Qt value/input behavior; shared groove, accent fill, handle painter. | Accent, subtle border, focus, icon size |
| `QCheckBox` | Qt accessible check semantics; shared indicator including tri-state. | Accent, disabled, border, radius |
| `CollapsibleSection` / `QWidget` | Full-width keyboard-operable header, content, optional accessory; expanded state and `expandedChanged`. Uses only its content height, leaving spare height to the parent layout. Custom separator; header uses style. | Section height, spacing, separator |
| `TreeView` / `QTreeView` | Common rows, connectors, text-only selection/focus, scrolling; arbitrary external model. Branches and trailing row space remain unhighlighted. Uses style, no domain data. | Tree metrics, selection, focus |
| `ThumbnailView` / `QListView` | Horizontal non-wrapping single-row strip with 112 × 112 square thumbnails, a transparent viewport and keyboard selection. | Thumbnail size/gaps, scrollbar |
| `ThumbnailDelegate` / `QStyledItemDelegate` | Aspect-preserving `DecorationRole` image/icon with rounded clipping and selection border. | Thumbnail size, accent, radius |
| `QFrame` HLine/VLine | Shared separator rendering through the style. | Separator |
| `QScrollArea`, `QScrollBar`, `QSplitter` | Standard behavior and shared scrollbar/splitter painting. | Surfaces, scrollbar/split metrics |
| `QMenu` | Shared Fusion geometry and Ardoise palette; application owns actions/shortcuts. | Elevated surface, selection, text |
| `IconProvider` | Original bundled monochrome SVGs via a DPR-aware QIconEngine; Default/Muted/Accent/Danger tones. | Icon colors and states |

Set `ardoiseFlat` on toolbar icon buttons for borderless resting/hover/checked
surfaces. Checked buttons use the muted accent; keyboard focus retains a visible
focus outline. Compose compact toolbars using Panel and token spacing.

Theme implementation lives in `src/theme`; generic widgets and window
shells live in `src/widgets`. SVG assets are under `resources/icons`. Resources
and English/French chrome translations are linked into the static library and
explicitly initialized, including for installed consumers.

Scrollbars reserve a 10-pixel lane with a transparent track and a centered
6-pixel rounded thumb. The full lane remains draggable; track clicks page normally
without arrow buttons. Hover/drag brighten the thumb. Geometry supports both
orientations and reversed direction.

TreeView uses a transparent viewport so the parent panel surface remains visible;
selection and hover backgrounds are still drawn by the shared style.

The theme disables automatic background filling on standard `QScrollArea`
containers, their viewports and attached content, preserving the parent surface
without application-specific styling. Input editors retain their field backgrounds.

CollapsibleSection draws its bottom separator by default. Use
`setSeparatorVisible(false)` for a final section that has no section beneath it.

## Window behavior and accessibility

MainWindow and Dialog use standard Qt window flags and native platform title bars,
buttons, decoration menus, movement, resizing and snapping. The Ardoise theme styles
only the content. Native decoration appearance follows the operating system and
compositor; no custom resize/drag event filters are installed. Applications use
setWindowTitle/setWindowIcon and place menus in their content. The former titleBar()
and MainWindow::setApplicationMenu() APIs are removed. TitleBar remains a standalone
legacy widget; the window/dialog shells no longer instantiate it.
Compositor snapping, multi-monitor scaling and macOS fullscreen behavior require
checks on the target desktops.

No custom accessibility roles replace Qt's controls. Icon actions have accessible
names, tooltips and keyboard focus. Tab, text selection, undo/redo, tree navigation,
checkboxes, and expander activation retain Qt behavior. Collapsing a section
returns focus to its header if focus was inside the hidden content.
Applications own saved geometry, shortcuts, window icons and screen structure.

## Styling and extension rules

Do use tokens, Ardoise icons and existing controls. Prefer the shared style for a
new standard control state. Introduce a component only when it adds shared
behavior, metrics or painting. Keep public headers self-contained and list every
source explicitly in CMake.

Do not hard-code colors/radii in screens, add arbitrary Canvas QSS, inherit a
platform visual style, duplicate generic controls, or move application models,
SQLite, file workflows, generation settings or backend APIs into this library.
Do not install the theme automatically merely by linking the library.

When adding a component: identify its generic API, preserve Qt input semantics,
use existing tokens, add it to the small gallery, test meaningful interaction and
states, and update this inventory. Test source and installed-package consumers.

## Gallery and validation

`ardoise-gallery` demonstrates controls, panels, icons, typography, colors, tree/list
selection and disabled states. Interact with it to check hover/pressed/focus.
`ARDOISE_GALLERY_CAPTURE=/path/to/directory` saves `gallery.png` and one image per
group there, then exits; `scripts/screenshots.sh` uses it for the README images.

Automated tests cover theme independence from the initial Qt style, text editing,
expansion/focus, icon resources/DPR, thumbnail keyboard navigation, native decoration flags,
modal close/Escape rejection, source integration and installed-package relocation.
Linux offscreen rendering is checked. Native Windows/macOS and real compositor
movement/resizing still need on-device validation; offscreen tests cannot establish
those guarantees.
