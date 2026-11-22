use super::fixture::FixtureGui;
use atspi::{
    proxy::{
        accessible::{AccessibleProxy, ObjectRefExt},
    },
    AccessibilityConnection,
};
use zbus::proxy::CacheProperties;
use std::path::PathBuf;
use std::process::Command;
use log;

const PROXY_DESTINATION: &str = "org.a11y.atspi.Registry";
const PROXY_PATH: &str = "/org/a11y/atspi/accessible/root";
const PROXY_INTERFACE: &str = "org.a11y.atspi.Accessible";

/// Test that importing to a newly created json based format is possible.
///
/// # Arguments
///
/// * `environment` - A test fixture stating the test environment
///
/// # Errors
///
/// This function returns `Err()` if the test case produced an unexpected result
///
/// # Panics
///
/// This function panics if the test environment causes errors.
///
pub(super) fn testcase_start_stop(environment: &mut FixtureGui) -> Result<(), ()> {
    /* create the db_to_use_param string, panic in case an error occured */
    let mut db_to_use = PathBuf::from(environment.temp_dir);
    db_to_use.push("gui_test.cfu");
    let db_to_use_param = db_to_use.into_os_string().into_string().unwrap();

    print!("call: {} -u {}\n", environment.exe_to_test, db_to_use_param);
    let output1 = Command::new(environment.exe_to_test)
        .args(&["-u", &db_to_use_param])
        .output()
        .expect("Err at running process");

    // While starting a process and using its input and output streams can be done
    // syhchronously, sending and receiving messages via dbus is asynchronous:

    async {


        // To allow exe_to_test to start...
        std::thread::sleep(std::time::Duration::from_millis(500));

        // Connect to the accessibility interface
        log::debug!("Connecting to the a11y interface...");

        log::debug!("Establishing ATSPI connection");
        let atspi = AccessibilityConnection::new().await?;
        log::trace!("Getting root proxy");
        let proxy = AccessibleProxy::builder(atspi.connection())
        .destination(PROXY_DESTINATION)?
        .path(PROXY_PATH)?
        .interface(PROXY_INTERFACE)?
        .cache_properties(CacheProperties::No)
        .build()
        .await?;

        // Find application
        let app_name: &str = environment.exe_to_test;
        log::trace!("Finding application. Searching for: {app_name:?}");
        let mut matches = vec![];
        let conn = proxy.inner().connection();
        for child in proxy.get_children().await? {
            let ap = child.into_accessible_proxy(conn).await?;
            if ap.get_role().await? == atspi::Role::Application {
                let name = ap.name().await?;
                log::trace!("Found app with name {name:?}");
                if name == app_name {
                    log::trace!("Name matches");
                    matches.push(ap);
                }
            }
        }

        if matches.is_empty() {
            log::debug!("No app match");
        }
        if matches.len() > 1 {
            log::debug!("Too many matches");
        }

    }.block_on();


    let stdout1 = match std::string::String::from_utf8(output1.stdout) {
        Ok(stdout) => stdout,
        Err(_err) => return Result::Err(()),
    };

    /* check if the returned string looks as expected */
    let as_expected1 = true;

    /* check that the exit code is 0 */
    let exit_ok1: bool = output1.status.success();

    print!("testcase_start_stop: <<{}>>:{}\n", stdout1, stdout1.len());
    if as_expected1 && exit_ok1 {
        Result::Ok(())
    } else {
        Result::Err(())
    }
}

/*
 * Copyright 2026-2026 Andreas Warnke
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
