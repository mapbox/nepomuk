#!/usr/bin/env bash

set -o errexit
set -o pipefail
set -o nounset

# Runs the Clang Formatter in parallel on the code base.
# Return codes:
#  - 1 there are files to be formatted
#  - 0 everything looks fine

# Get CPU count
OS=$(uname)
CPUCOUNT=1
if [[ $OS = "Linux" ]] ; then
    CPUCOUNT=$(nproc)
elif [[ ${OS} = "Darwin" ]] ; then
    CPUCOUNT=$(sysctl -n hw.physicalcpu)
fi

# Discover clang-format
if type clang-format-3.8 2> /dev/null ; then
    CLANG_FORMAT=clang-format-3.8
elif type clang-format 2> /dev/null ; then
    # Clang format found, but need to check version
    CLANG_FORMAT=clang-format
    CLANG_VERSION=$(clang-format --version)
    if [[ ${CLANG_VERSION} != *3.8* ]] ; then
        echo "clang-format is not 3.8 (returned ${CLANG_VERSION})"
        exit 1
    fi
else
    echo "No appropriate clang-format found (expected clang-format-3.8, or clang-format)"
    exit 1
fi

find src include test -type f -name '*.hpp' -o -name '*.cpp' -o -name '*.cc' -o -name '*.h'\
  | xargs -I{} -P ${CPUCOUNT} ${CLANG_FORMAT} -i -style=file {}


unformatted=$(git ls-files --modified)

if [[ $unformatted ]]; then
    echo "The following files do not adhere to the .clang-format style file:"
    echo $unformatted
    exit 1
else
    exit 0
fi
