# Lomiri UI Toolkit


## Introduction

This project consists of a set of QML components to ease the creation of
beautiful applications in QML for Lomiri.

QML alone lacks built-in components for basic widgets like Button, Slider,
Scrollbar, etc, meaning a developer has to build them from scratch. This
toolkit aims to stop this duplication of work, supplying ready-made components
with a clear and consistent API.

Features:
- Localisation through `gettext`
- Components are fully themeable
- Scaling support to suit different displays

This project supports QML2 with Qt5 and Qt6. Debian unstable is currently well supported.


## Qt6 + CMake

### Build

```sh
cmake -S . -B build
cmake --build build -j $(nproc)
```

### Test

```sh
# Run all tests:
ctest --test-dir build
# List tests:
ctest --test-dir build -N
# Run specific test verbosely:
ctest --test-dir build -R <TEST_NAME> --verbose
```

#### API Test

The Lomiri api can be checked against the reference api file `components_qt6.api` with:
```sh
ctest --test-dir build -R apicheck
```

To update the reference api, run:
```sh
QML2_IMPORT_PATH=build/qml build/apicheck/apicheck --qml Lomiri.Components Lomiri.Components.ListItems Lomiri.Components.Popups Lomiri.Components.Pickers Lomiri.Components.Styles Lomiri.Components.Themes Lomiri.Layouts Lomiri.PerformanceMetrics Lomiri.Metrics Lomiri.Test > components_qt6.api
```

#### Components Gallery

Show an interactive gallery that demonstrates the Lomiri UI Toolkit components:

```sh
QML2_IMPORT_PATH=build/qml \
  build/lomiri-ui-toolkit-launcher/lomiri-ui-toolkit-launcher-qt6 \
  examples/lomiri-ui-toolkit-gallery-qt6/lomiri-ui-toolkit-gallery.qml
```

### Install

Install the project to the current system (may need to be run as root):
```sh
cmake --install build
```

## Qt5 + qmake

### Build

To build this repository, ensure you have Qt 5.15 installed and run:
```sh
mkdir build
cd build
qmake ..
make -j $(nproc)
```

### Test

Set necessary environment variables for testing:
```sh
export UITK_BUILD_ROOT=build
source export_qml_dir.sh
```

#### Check the API

Test the `apicheck` tool:
```sh
cd build/tests/api
make check
```

Test the actual Lomiri api:
```sh
./tests/qmlapicheck.sh
```

This produces the file `components.api.new` and compares it to `components.api`. The latter needs to be updated when changes to the Lomiri api are made in order to pass the tests.

#### Check project licenses

```sh
make license
```

#### View the Components Gallery

```sh
./gallery.sh
```

#### Verify Packaging Tidyness

```sh
./packaging-sorting.sh
```

#### Verify Python Coding Style

```sh
pyflakes .
```

### Install

To install these components, run
```
make install
```
You may need to be root for this command to succeed.

To use these components in QML without installing them, you need to set the
`QML2_IMPORT_PATH` environment variable. This is done by running `source export_qml_dir.sh`
from the root directory of this repository, but is lost when you close your
shell.


## Documentation

Once built, you may view the documentation by opening documentation/html/overview-lomiri-sdk.html in your favorite web browser.

See also the [online documentation](https://ubports.gitlab.io/docs/api-docs/index.html?p=lomiriuserinterfacetoolkit%2Flomiri-components-qmlmodule.html).
