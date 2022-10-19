#! /bin/bash

SCRIPT_DIR="$( cd -- "$( dirname -- "$0" )" >/dev/null 2>&1 && pwd )"

cd "$SCRIPT_DIR" || exit 1
mkdir build

cd "build" || exit 1

cmake ../  -DRIC_GENERATED_E2AP_BINDING_DIR="$SCRIPT_DIR/e2_bindings/E2AP-v01.01" \
            -DRIC_GENERATED_E2SM_KPM_BINDING_DIR="$SCRIPT_DIR/e2_bindings/E2SM-KPM" \
            -DRIC_GENERATED_E2SM_NI_BINDING_DIR="$SCRIPT_DIR/e2_bindings/E2SM-NI" \
            -DRIC_GENERATED_E2SM_GNB_NRT_BINDING_DIR="$SCRIPT_DIR/e2_bindings/E2SM-GNB-NRT"
make
make install
ldconfig