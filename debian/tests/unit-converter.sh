#!/bin/sh

set -eu

DEB_HOST_MULTIARCH=$(dpkg-architecture -qDEB_HOST_MULTIARCH 2>/dev/null)
EXAMPLES_FOLDER=/usr/lib/${DEB_HOST_MULTIARCH}/qt5/examples/lomiri-ui-toolkit/examples

# Copy the example
cp -r $EXAMPLES_FOLDER/unit-converter ./debian/tests/

xvfb-run qmltestrunner -input debian/tests
