#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
STUB_DIR="${ROOT_DIR}/tests/compilation_stubs"
SOURCE_FILE="${ROOT_DIR}/flipper/flipper-evil-portal/helpers/evil_portal_storage.c"

gcc -std=c11 -Wall -Wextra -Werror \
    -I"${STUB_DIR}" \
    -I"${ROOT_DIR}/flipper/flipper-evil-portal" \
    -fsyntax-only "${SOURCE_FILE}"
