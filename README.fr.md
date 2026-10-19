# Ardoise

[![CI](https://github.com/jstmichel/ardoise/actions/workflows/ci.yml/badge.svg)](https://github.com/jstmichel/ardoise/actions/workflows/ci.yml)
[![Release](https://img.shields.io/github/v/release/jstmichel/ardoise)](https://github.com/jstmichel/ardoise/releases/latest)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue)](LICENSE)

*English: [README.md](README.md)*

Ardoise est un thème sombre et un ensemble de composants réutilisables pour les
applications de bureau **Qt 6 Widgets**, en C++20 avec CMake. Linux, macOS et Windows
partagent les mêmes dessins, jetons et icônes ; fenêtres et dialogues gardent leur barre
de titre native.

![La gallery d'Ardoise](docs/screenshots/gallery.png)

- Un seul appel, `Ardoise::Theme::apply()`, habille tous les contrôles Qt standard, sans
  feuille de style.
- Les composants ajoutent un comportement commun aux classes Qt et conservent leur
  clavier, leur accessibilité et leurs signaux.
- 40 icônes SVG monochromes, teintées par le thème et nettes à toute échelle.
- Des jetons de design (couleurs, espacements, rayons, typographie) utilisables dans vos
  propres widgets.
- Les textes des composants en anglais et en français.
- Une bibliothèque statique avec un paquet CMake déplaçable ; aucune dépendance
  d'exécution au-delà de Qt Widgets et Qt SVG.

## État

Version 0.1.0, la première. L'API peut changer entre versions mineures avant 1.0. Linux
est validé par des tests automatiques hors écran ; la CI compile et teste aussi sur macOS
et Windows. Notes de version : [docs/release-notes/0.1.0.md](docs/release-notes/0.1.0.md).

## Démarrage rapide

Ajoutez Ardoise à un projet CMake avec `FetchContent` :

```cmake
include(FetchContent)
FetchContent_Declare(Ardoise
    GIT_REPOSITORY https://github.com/jstmichel/ardoise.git
    GIT_TAG v0.1.0)
FetchContent_MakeAvailable(Ardoise)

target_link_libraries(my-app PRIVATE Ardoise::Widgets)
```

Appliquez le thème juste après avoir créé l'application, avant tout widget :

```cpp
#include <Ardoise/MainWindow.h>
#include <Ardoise/PrimaryButton.h>
#include <Ardoise/Theme.h>
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication application(argc, argv);
    Ardoise::Theme::apply(application);

    Ardoise::MainWindow window;
    window.setWindowTitle(QStringLiteral("My application"));
    window.setContentWidget(new Ardoise::PrimaryButton(QStringLiteral("Start")));
    window.show();
    return application.exec();
}
```

`Ardoise::Widgets` apporte à votre cible les en-têtes publics, C++20 et Qt Widgets.

## Utiliser dans un autre projet

Ardoise demande CMake 3.21+, un compilateur C++20 et Qt 6.2+ avec Widgets, SVG et
LinguistTools. Trois façons de l'intégrer, qui fournissent toutes la cible
`Ardoise::Widgets`.

### FetchContent (recommandé)

Comme dans le démarrage rapide. Épinglez `GIT_TAG` sur un tag de version. Pour travailler
sur Ardoise et votre application en même temps, indiquez à CMake une copie locale au lieu
du téléchargement :

```sh
cmake -S . -B build -DFETCHCONTENT_SOURCE_DIR_ARDOISE=/chemin/vers/ardoise
```

### Copie des sources

Avec Ardoise en sous-module Git ou dans un dossier voisin :

```cmake
add_subdirectory(external/ardoise)
target_link_libraries(my-app PRIVATE Ardoise::Widgets)
```

Intégrée ainsi, Ardoise ne compile que la bibliothèque : ni exemples, ni tests, ni règles
d'installation.

### Paquet installé

Sous Linux et macOS, une seule commande télécharge la dernière version, la vérifie avec
le `SHA256SUMS` de la release, la compile et l'installe dans `~/.local` :

```sh
curl -fsSL https://github.com/jstmichel/ardoise/releases/latest/download/get-ardoise.sh | sh
```

Ajoutez `-s -- --version v0.1.0` après `sh` pour une version précise, `--prefix <dossier>`
pour un autre dossier, `--qt <dossier>` quand CMake ne trouve pas Qt, ou `--help`. Sous
Windows, ou depuis une copie des sources, installez avec CMake :

```sh
cmake -S ardoise -B ardoise/build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
cmake --build ardoise/build
cmake --install ardoise/build --prefix "$HOME/.local"
```

Trouvez-la ensuite comme tout paquet CMake :

```cmake
find_package(Ardoise 0.1 CONFIG REQUIRED)
target_link_libraries(my-app PRIVATE Ardoise::Widgets)
```

CMake trouve `~/.local` quand `~/.local/bin` est dans le `PATH` ; sinon, configurez votre
application avec `-DCMAKE_PREFIX_PATH="$HOME/.local"`. Le préfixe peut être déplacé ; Qt
doit rester trouvable. Les paquets sont compatibles au sein d'une même version mineure.
Réinstallez après une mise à jour de Qt et, sous Windows, utilisez le même type de build
que votre application.

Avec CMake 3.24 ou plus récent, `FetchContent` peut préférer un paquet installé et ne
télécharger la version que s'il n'en trouve pas :

```cmake
FetchContent_Declare(Ardoise
    GIT_REPOSITORY https://github.com/jstmichel/ardoise.git
    GIT_TAG v0.1.0
    FIND_PACKAGE_ARGS 0.1 CONFIG)
FetchContent_MakeAvailable(Ardoise)
```

Ardoise est une bibliothèque statique : elle n'embarque pas Qt, et votre application
déploie Qt comme d'habitude (`windeployqt`, `macdeployqt`, paquets de la distribution).

## Composants

Chaque image vient de la gallery (`examples/gallery`). Les contrôles Qt standard n'ont
pas besoin de sous-classe : le thème les habille.

### Boutons

![Boutons](docs/screenshots/buttons.png)

```cpp
auto* save = new Ardoise::PrimaryButton(
    Ardoise::IconProvider::icon(Ardoise::Icon::Save), tr("Save"));
auto* cancel = new QPushButton(tr("Cancel"));        // action secondaire, habillée

auto* settings = new Ardoise::IconButton(Ardoise::Icon::Settings, tr("Settings"));
auto* remove = new Ardoise::IconButton(Ardoise::Icon::Close, tr("Remove"));
remove->setTone(Ardoise::ButtonTone::Destructive);   // rouge au survol
```

`PrimaryButton` est l'action principale d'un écran. `IconButton` est une action compacte
et accessible, sans texte : son libellé devient l'infobulle et le nom accessible. Donnez la
propriété `ardoiseFlat` à `true` aux boutons de barre d'outils pour les afficher sans
bordure.

### Champs

![Champs](docs/screenshots/inputs.png)

```cpp
auto* search = new Ardoise::SearchField;             // icône de recherche et bouton d'effacement
search->setPlaceholderText(tr("Search…"));
connect(search, &QLineEdit::textChanged, this, &MyView::filter);
```

`QLineEdit`, `QComboBox`, `QSpinBox`, `QDoubleSpinBox`, `QSlider` et `QCheckBox` sont
habillés par le thème, avec les mêmes hauteurs, bordures et contour de focus.

### Texte

![Texte](docs/screenshots/text.png)

```cpp
auto* prompt = new Ardoise::PlainTextEdit;
prompt->setCounterVisible(true);
prompt->setReferenceLength(2000);                    // indicatif : affiche « 50 / 2000 »
prompt->setFixedHeight(prompt->heightForLines(3));
```

Le compteur est une indication et ne tronque jamais le texte.

### Arbre et section repliable

![Arbre et section repliable](docs/screenshots/tree.png)

```cpp
auto* tree = new Ardoise::TreeView;
tree->setModel(model);                               // tout QAbstractItemModel

auto* section = new Ardoise::CollapsibleSection(tr("Layers"), tree);
connect(section, &Ardoise::CollapsibleSection::expandedChanged, this, &MyView::save);
```

`CollapsibleSection` a un en-tête pleine largeur utilisable au clavier, un widget
accessoire facultatif (`setAccessoryWidget`) et un séparateur que l'on peut masquer sur la
dernière section.

### Vignettes

![Vignettes](docs/screenshots/thumbnails.png)

```cpp
auto* strip = new Ardoise::ThumbnailView;            // une rangée horizontale, 112 × 112
strip->setModel(images);                             // images dans Qt::DecorationRole
```

`ThumbnailView` utilise `ThumbnailDelegate`, qui conserve les proportions, arrondit les
coins et dessine la bordure de sélection.

### Icônes

![Icônes](docs/screenshots/icons.png)

```cpp
QIcon icon = Ardoise::IconProvider::icon(Ardoise::Icon::Upload);
QIcon muted = Ardoise::IconProvider::icon(Ardoise::Icon::Info, Ardoise::IconTone::Muted);
```

Les tons sont `Default`, `Muted`, `Accent` et `Danger`. La liste complète est l'énumération
`Ardoise::Icon` de [IconProvider.h](include/Ardoise/IconProvider.h).

### Fenêtres, dialogues et panneaux

```cpp
Ardoise::Dialog dialog(this);                        // barre de titre native, Échap annule
dialog.setWindowTitle(tr("Export"));
dialog.setContentWidget(form);

auto* panel = new Ardoise::Panel;                    // surface pleine avec bordure
panel->setCornerRadius(Ardoise::Theme::tokens().metrics.radiusLarge);
auto* layout = new QVBoxLayout(panel);
```

`MainWindow` et `Dialog` gardent les décorations natives ; placez vos menus dans le
contenu et utilisez `setWindowTitle` et `setWindowIcon`.

### Typographie et couleurs

![Typographie](docs/screenshots/typography.png)

![Couleurs](docs/screenshots/colors.png)

```cpp
Ardoise::Theme::applyTypography(title, Ardoise::TextRole::PageTitle);

const auto& tokens = Ardoise::Theme::tokens();
painter.setPen(tokens.colors.textMuted);
layout->setSpacing(tokens.metrics.spacing12);
```

Utilisez les jetons dans vos propres widgets plutôt que des valeurs codées en dur. Le
contrat visuel complet est dans [docs/design-system.md](docs/design-system.md) (en
anglais).

### Traductions

`Theme::apply()` installe la traduction anglaise ou française des textes des composants,
selon la langue du système. Appelez `Theme::installTranslations(application, locale)` pour
suivre une langue choisie dans votre application.

## Compiler et tester

```sh
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

Les tests s'exécutent hors écran. Ouvrez la gallery avec
`./build/examples/gallery/ardoise-gallery`, et refaites les captures de cette page avec
`scripts/screenshots.sh`.

Voir [CONTRIBUTING.md](CONTRIBUTING.md) (en anglais) pour la préparation des plateformes et
les options de compilation, et [docs/architecture.md](docs/architecture.md) pour les
limites de la bibliothèque.

## Licence

[MIT](LICENSE).
