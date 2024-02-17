#include "flutter_timezone_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>

namespace flutter_timezone {

// static
void FlutterTimezonePlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "flutter_timezone",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<FlutterTimezonePlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

FlutterTimezonePlugin::FlutterTimezonePlugin() {}

FlutterTimezonePlugin::~FlutterTimezonePlugin() {}

void FlutterTimezonePlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("getLocalTimezone") == 0) {

    TIME_ZONE_INFORMATION tzinfo = {0};
    GetTimeZoneInformation(&tzinfo);

    wchar_t *wc = tzinfo.StandardName;
    int cch = WideCharToMultiByte(CP_ACP, 0, wc, -1, 0, 0, NULL, NULL);
    char *psz = new char[cch];

    WideCharToMultiByte(CP_ACP, 0, wc, -1, psz, cch, NULL, NULL);

    result->Success(flutter::EncodableValue(psz));
    delete[] psz;
}
else {
      result->NotImplemented();
  }
}

}  // namespace flutter_timezone
