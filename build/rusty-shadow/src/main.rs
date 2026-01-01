#![warn(missing_docs)]

//! This crate converts c99 code to rust.

mod help;
use crate::help::print_help;
use std::env;

fn main() {
    /* promising: */
    /* https://www.reddit.com/r/rust/comments/11r0ux3/rust_crate_for_reading_and_writing_configuration/ */

    print_help(env::args());
}

/*
 * Copyright 2025-2026 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
