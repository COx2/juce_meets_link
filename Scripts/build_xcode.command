#!/bin/sh

set -e -u

# 参考: https://stackoverflow.com/questions/3572030/bash-script-absolute-path-with-os-x
realpath() {
  [[ $1 = /* ]] && echo "$1" || echo "$PWD/${1#./}"
}

SCRIPT_NAME="$(basename "$(realpath "${BASH_SOURCE:-$0}")")"
SCRIPT_DIRECTORY="$(dirname "$(realpath "${BASH_SOURCE:-$0}")")"

cd "${SCRIPT_DIRECTORY}/.."

cmake -B Build -G Xcode .

