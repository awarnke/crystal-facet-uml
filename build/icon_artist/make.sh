#!/bin/sh
find . -name '*.rs' | xargs rustfmt
cargo build ; echo "ERROR: $?"
echo "consider running rustup update"
echo "view source documentation using cargo doc && chromium-browser target/doc/icon_artist/index.html"
echo "run program by e.g. cargo run -- -h"

# Copyright 2023-2024 Andreas Warnke
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

