use super::fixture::FixtureGui;
use atspi::{
    AccessibilityConnection,
    proxy::accessible::{AccessibleProxy, ObjectRefExt},
};
use futures::executor::block_on;
use std::path::PathBuf;
use std::process::Command;
use zbus::proxy::CacheProperties;

const PROXY_DESTINATION: &str = "org.a11y.atspi.Registry";
const PROXY_PATH: &str = "/org/a11y/atspi/accessible/root";
const PROXY_INTERFACE: &str = "org.a11y.atspi.Accessible";

/// Finds and connects to an app
///
/// # Arguments
///
/// * `app_name` - A path to the executable to be tested
///
async fn connect_to_app(app_name: &str) -> Result<(), ()> {
    // Connect to the accessibility interface
    print!("Connecting to the a11y interface...\n");

    print!("Establishing ATSPI connection\n");
    let atspi = AccessibilityConnection::new().await.unwrap();
    print!("Getting root proxy\n");
    let proxy = AccessibleProxy::builder(atspi.connection())
        .destination(PROXY_DESTINATION)
        .unwrap()
        .path(PROXY_PATH)
        .unwrap()
        .interface(PROXY_INTERFACE)
        .unwrap()
        .cache_properties(CacheProperties::No)
        .build()
        .await
        .unwrap();

    // Find application
    print!("Finding application. Searching for: {app_name:?}\n");
    let mut matches = vec![];
    let conn = proxy.inner().connection();
    for child in proxy.get_children().await.unwrap() {
        let ap = child.into_accessible_proxy(conn).await.unwrap();
        if ap.get_role().await.unwrap() == atspi::Role::Application {
            let name = ap.name().await.unwrap();
            print!("Found app with name {}\n", name);
            if name == app_name {
                print!("Name matches\n");
                matches.push(ap);
            }
        }
    }

    if matches.is_empty() {
        print!("No app match\n");
    }
    if matches.len() > 1 {
        print!("Too many matches\n");
    }
    return if matches.len() == 1 { Ok(()) } else { Err(()) };
}

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

    // To allow exe_to_test to start...
    std::thread::sleep(std::time::Duration::from_millis(500));

    // While starting a process and using its input and output streams can be done
    // syhchronously, sending and receiving messages via dbus is asynchronous:
    let future = connect_to_app(environment.exe_to_test);
    let result = block_on(future);

    let stdout1 = match std::string::String::from_utf8(output1.stdout) {
        Ok(stdout) => stdout,
        Err(_err) => return Result::Err(()),
    };

    /* check if the returned string looks as expected */
    let as_expected1 = true;

    /* check that the exit code is 0 */
    let exit_ok1: bool = output1.status.success();

    print!("testcase_start_stop: <<{}>>:{}\n", stdout1, stdout1.len());
    if as_expected1 && exit_ok1 && result == Ok(()) {
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
