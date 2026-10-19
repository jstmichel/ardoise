# Architecture

Generic editing icons live in IconProvider and the bundled SVG resources:
ZoomOut, ZoomIn, ActualSize, Crop, Upscale, Downscale, Paperclip (attachments), Sparkles (AI assistance), IdentityEdit (editing with an identity) Sharpen (sharpening an image) Send (sending a message), Upload (uploading files) and Image (a picture). Consumers compose IconButtons
into their own toolbars and own image/crop/export workflows; no image editing
logic is added to the framework.

Applications → `Ardoise::Widgets` → Qt 6 Widgets/Gui/Core and Qt SVG.
The framework is a standalone C++20 static library with no Canvas, Service,
SDK, persistence or generation dependencies. Public APIs use namespace `Ardoise`.

The 0.1 API supplies explicit theme installation, tokens, a Fusion-based proxy
style, SVG icons, common controls, panels, native window/dialog shells and a gallery.
See [design-system.md](design-system.md) for the visual contract and complete
component inventory. `Ardoise::version()` returns the compiled project version.

Applications own theme activation, window icons, shortcuts, settings, files,
service connections, models and workflows. The library owns reusable presentation
and preserves Qt behavior. Do not add a plugin system, theme loader, generic event
bus or application state manager without an actual consuming requirement.

Public headers live in `include/Ardoise`; implementation and private headers live
in `src`. Sources are listed explicitly. MOC/RCC and compiler options are scoped
to the library target. Theme application is explicit, never static initialization.

`Ardoise::Widgets` is a source alias or installed imported target. It propagates
C++20 and public Qt Widgets requirements; static linkage also brings Qt SVG.
Standalone builds enable examples, tests and installation; embedded builds omit
examples/install by default and never register standalone tests. Resources and
translations are embedded, including in relocated installed consumers.

Installed-package compatibility is limited to the same minor version before 1.0.
No stable ABI or cross-toolchain binary compatibility is promised. The package
does not bundle Qt or deploy applications; rebuild per compiler/architecture/OS.
