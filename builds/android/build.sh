#!/bin/bash
#
#   Copyright 2012 - 2013 Kulykov Oleh
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#

cd `dirname $0`
PWD=$(pwd)

# clean up prev build result
rm -rf libs
rm -rf obj

perl -pe 's/(RECORE_BUILD_NUMBER )(\d+)/$1.($2+1)/eg' -i ../../include/recore/RECoreVersion.h

# Update NDK_MODULE_PATH so we can find our imported modules
export NDK_MODULE_PATH="$PWD"

# Build everything
$NDK/ndk-build "$@"


# clean up prev build result
rm -rf ../../lib_bin/android
mkdir ../../lib_bin/android

cd obj
find . -type f -iregex '.*\.a$' | xargs -I @ cp @ ../../../lib_bin/android
cd ..

cd libs
find . -type f -iregex '.*\.so$' | xargs -I @ cp @ ../../../lib_bin/android
cd ..

