// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "examples/message_router/client_impl.h"
#include "examples/shared/app_factory.h"
#include "examples/shared/browser_util.h"
#include "examples/shared/resource_util.h"

namespace message_router {

namespace {

std::string GetStartupURL() {
//    return std::string("file:///C:/Users/v-pikund/_git/minecraft-ui/dist/hbui/index.html");
    return shared::kTestOrigin + std::string("index.html");// overlay-ui
//    return shared::kTestOrigin + std::string("hbui/index.html"); //minecraft-ui
//    return std::string("hbui/index.html");
//    return shared::kTestOrigin + std::string("message_router.html");
//
}

}  // namespace

// Implementation of CefApp for the browser process.
class BrowserApp : public CefApp, public CefBrowserProcessHandler {
 public:
  BrowserApp() {}

  // CefApp methods:
  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE {
    return this;
  }

  void OnBeforeCommandLineProcessing(
      const CefString& process_type,
      CefRefPtr<CefCommandLine> command_line) OVERRIDE {
    // Command-line flags can be modified in this callback.
    // |process_type| is empty for the browser process.
    if (process_type.empty()) {
#if defined(OS_MACOSX)
      // Disable the macOS keychain prompt. Cookies will not be encrypted.
      command_line->AppendSwitch("use-mock-keychain");
#endif
        command_line->AppendSwitch("force-devtools-available");
        command_line->AppendSwitch("auto-open-devtools-for-tabs");
        command_line->AppendSwitch("disable-developer-tools=false");

    }
  }

  // CefBrowserProcessHandler methods:
  void OnContextInitialized() OVERRIDE {
    // Create the browser window.
    const CefString& startup_url = GetStartupURL();
      CefBrowserSettings settings;
      settings.windowless_frame_rate = 120;
      fprintf(stderr,"%s", "PIOTR WAS HERE\n");
      LOG(INFO) << "BrowserApp::OnContextInitialized called"<< std::endl;
      LOG(ERROR) << "ERROR::OnContextInitialized called"<< std::endl;

      shared::CreateBrowser(new Client(startup_url), startup_url, settings);
  }

 private:
  IMPLEMENT_REFCOUNTING(BrowserApp);
  DISALLOW_COPY_AND_ASSIGN(BrowserApp);
};

}  // namespace message_router

namespace shared {

CefRefPtr<CefApp> CreateBrowserProcessApp() {
  return new message_router::BrowserApp();
}

}  // namespace shared
