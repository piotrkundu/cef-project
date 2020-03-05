// Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "examples/shared/app_factory.h"

#include "include/wrapper/cef_message_router.h"

namespace message_router {

    class MyV8Handler : public CefV8Handler {
    public:
        MyV8Handler() {}

        virtual bool Execute(const CefString& name,
                             CefRefPtr<CefV8Value> object,
                             const CefV8ValueList& arguments,
                             CefRefPtr<CefV8Value>& retval,
                             CefString& exception) OVERRIDE {
            if (name == "myfunc") {
                // Return my string value.
                retval = CefV8Value::CreateString("My Value!");
                return true;
            }
            if (name == "register") {
                if (arguments.size() == 1 && arguments[0]->IsFunction()) {
                    callback_func_ = arguments[0];
                    callback_context_ = CefV8Context::GetCurrentContext();
                    return true;
                }
            }
            // Function does not exist.
            return false;
        }
        CefRefPtr<CefV8Handler> get(){
            return static_cast<CefRefPtr<CefV8Handler>>(this);
        }
        bool runCallback(){
            CefV8ValueList args;
            args.push_back(CefV8Value::CreateString("Piotrs_Callback"));
            CefRefPtr<CefV8Value> retval2;
            if ((retval2 = callback_func_->ExecuteFunctionWithContext(callback_context_, NULL, args))) {
                if (callback_func_.get()->GetException().get()) {
                    // Execution threw an exception.
                    return false;
                }
            }
            return true;
        }
    private:
        CefRefPtr<CefV8Value> callback_func_;
        CefRefPtr<CefV8Context> callback_context_;
        // Provide the reference counting implementation for this class.
    IMPLEMENT_REFCOUNTING(MyV8Handler);
    };
    /*
// Implementation of CefApp for the renderer process.
class RendererApp : public CefApp, public CefRenderProcessHandler {
 public:
  RendererApp() {}

  // CefApp methods:
  CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE {
    return this;
  }
    void OnBrowserCreated(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefDictionaryValue> extra_info)OVERRIDE {
        auto frame = browser->GetMainFrame();
        CefString runThisFromCef("window.runThisFromCef('{\"name\":\"Player Name\",\"xuid\":\"xuid-'+ Math.round(Math.random() * 10e10) + '\",\"url\":\"avatar.jpg\",\"tag\":\"Hello World\"}');");
        frame->ExecuteJavaScript(runThisFromCef, frame->GetURL(), 0);
    }
  // CefRenderProcessHandler methods:
  void OnWebKitInitialized() OVERRIDE {
    // Create the renderer-side router for query handling.
    CefMessageRouterConfig config;
    message_router_ = CefMessageRouterRendererSide::Create(config);
  }

  void OnContextCreated(CefRefPtr<CefBrowser> browser,
                        CefRefPtr<CefFrame> frame,
                        CefRefPtr<CefV8Context> context) OVERRIDE {
      CefRefPtr<CefV8Value> windowObj = context->GetGlobal();

      //1. Execute JavaScript
      CefString runThisFromCef("runThisFromCef2('{\"name\":\"Player Name\",\"xuid\":\"xuid-'+ Math.round(Math.random() * 10e10) + '\",\"url\":\"avatar.jpg\",\"tag\":\"Hello World\"}');");
      frame->ExecuteJavaScript(runThisFromCef, frame->GetURL(), 0);
      //Math.round(Math.random() * 10e10)
//      frame->ExecuteJavaScript("modelUpdate('{json:{modelUpdate=text}}');", frame->GetURL(), 0);
// window.runThisFromCef('{"name":"Player Name","xuid":"xuid-'+ Math.round(Math.random() * 10e10) + '","url":"avatar.jpg","tag":"Hello World"}');
// window.runThisFromCef('{'name':'Player Name','xuid':'xuid-12314345','url':'avatar.jpg','tag':'Hello World'}')
// window.runThisFromCef('{"name":"Player Name","xuid":"xuid-12314345","url":"avatar.jpg","tag":"Hello World"}')
// window.runThisFromCef('{"name":"Player Name","xuid":"xuid-12314345","url":"avatar.jpg","tag":"Hello World"}')
      //2. Add JS value
      // Create a new V8 string value. See the "Basic JS Types" section below.
      CefRefPtr<CefV8Value> str = CefV8Value::CreateString("{json:{window.model=text}}");
      // Add the string to the window object as "window.model". See the "JS Objects" section below.
      windowObj->SetValue("model2PIOTR_ ", str, V8_PROPERTY_ATTRIBUTE_NONE);
      LOG(INFO) << "model2 created"<< std::endl;
      LOG(ERROR) << "ERROR: model2 created"<< std::endl;
      // Add the "myfunc" and register function to the "window" object.
      windowObj->SetValue("myfunc", CefV8Value::CreateFunction("myfunc", handler_.get()), V8_PROPERTY_ATTRIBUTE_NONE);
      windowObj->SetValue("register",CefV8Value::CreateFunction("register", handler_.get()),V8_PROPERTY_ATTRIBUTE_NONE);
      fprintf(stderr,"%s", "----PIOTR WAS HERE----\n");
    message_router_->OnContextCreated(browser, frame, context);
  }

  void OnContextReleased(CefRefPtr<CefBrowser> browser,
                         CefRefPtr<CefFrame> frame,
                         CefRefPtr<CefV8Context> context) OVERRIDE {
    message_router_->OnContextReleased(browser, frame, context);
  }

  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefProcessId source_process,
                                CefRefPtr<CefProcessMessage> message) OVERRIDE {
      handler_.runCallback();
    return message_router_->OnProcessMessageReceived(browser, frame,
                                                     source_process, message);
  }
  void OnUncaughtException(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefV8Context> context,
                                     CefRefPtr<CefV8Exception> exception,
                                     CefRefPtr<CefV8StackTrace> stackTrace)OVERRIDE {
      //browser->ReloadIgnoreCache();
      auto str = exception->GetMessageW().ToString();
      LOG(ERROR) << "ERROR: OnUncaughtException created"<< std::endl;
      printf("PIOTR OnUncaughtException %s", str.c_str());
  }

 private:
  // Handles the renderer side of query routing.
  CefRefPtr<CefMessageRouterRendererSide> message_router_;
  MyV8Handler handler_; // CefRefPtr<CefV8Handler>
IMPLEMENT_REFCOUNTING(RendererApp);
  DISALLOW_COPY_AND_ASSIGN(RendererApp);
};
*/
}  // namespace message_router

namespace shared {

CefRefPtr<CefApp> CreateRendererProcessApp() {
    int i = 10;
    int k = 0;
    k = i/k;
    ++k;++i;
    return nullptr;
  //return new message_router::RendererApp();
}

}  // namespace shared
