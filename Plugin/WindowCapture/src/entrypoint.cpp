#include "IUnityInterface.h"
#include "IUnityGraphics.h"
#include "manager.h"

namespace window_capture {
IUnityInterfaces* g_unity;
}

using namespace window_capture;

extern "C"
{
  namespace {
  void UNITY_INTERFACE_API OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
  {
    switch (eventType) {
    case kUnityGfxDeviceEventShutdown:
      // 各windowが持っているテクスチャリソースを解放する
      g_manager.reset();
    }
  }
  }

  void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces)
  {
    g_unity = unityInterfaces;
    g_manager = std::make_unique<manager>();
    auto graphics = g_unity->Get<IUnityGraphics>();
    graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);
  }

  void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload()
  {
    auto graphics = g_unity->Get<IUnityGraphics>();
    graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
  }
}
