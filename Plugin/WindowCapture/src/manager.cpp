#include "manager.h"
#include "IUnityInterface.h"
#include "IUnityGraphics.h"
#include <algorithm>

namespace window_capture {

std::unique_ptr<manager> g_manager;

manager::id manager::get_foreground_window_id()
{
  auto hwnd = GetForegroundWindow();
  if (hwnd == NULL) {
    return invalid_id;
  }

  auto foreground_window_id = find_id(hwnd);

  // 新しいウィンドウの場合
  if (foreground_window_id == invalid_id) {
    window new_window{ hwnd };
    windows[head_id] = new_window;
    foreground_window_id = head_id;
    ++head_id;
  }

  return foreground_window_id;
}

manager::id manager::find_id(const HWND hwnd) const
{
  auto found = std::find_if(windows.begin(), windows.end(), [=](const std::pair<id, window>& x)
  {
    return x.second.get_hwnd() == hwnd;
  });
  if (found == windows.end()) {
    return invalid_id;
  } else {
    return (*found).first;
  }
}

window& manager::get_window(const id request_id)
{
  return windows.at(request_id);
}

std::vector<manager::id> manager::get_ids() const
{
  std::vector<id> ids(windows.size());
  auto window = windows.begin();
  std::generate(ids.begin(), ids.end(), [&]()
  {
    auto retval = (*window).first;
    ++window;
    return retval;
  });
  return ids;
}

size_t manager::get_num_windows() const
{
  return windows.size();
}

}

using namespace window_capture;

extern "C"
{
  UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API GetForegroundWindowID()
  {
    if (!g_manager) {
      return manager::invalid_id;
    }
    return g_manager->get_foreground_window_id();
  }

  UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API GetWindowWidth(int id)
  {
    if (!g_manager) {
      return -1;
    }
    try {
      return g_manager->get_window(id).get_width();
    } catch (std::out_of_range&) {
      return -1;
    } catch (std::runtime_error&) {
      return -1;
    }
  }

  UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API GetWindowHeight(int id)
  {
    if (!g_manager) {
      return -1;
    }
    try {
      return g_manager->get_window(id).get_height();
    } catch (std::out_of_range&) {
      return -1;
    } catch (std::runtime_error&) {
      return -1;
    }
  }

  UNITY_INTERFACE_EXPORT UINT UNITY_INTERFACE_API GetWindowDPI(int id)
  {
    if (!g_manager) {
      return 0;
    }
    try {
      return g_manager->get_window(id).get_dpi();
    } catch (std::out_of_range&) {
      return 0;
    }
  }

  UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API GetNumWindows()
  {
    if (!g_manager) {
      return 0;
    }
    return static_cast<int>(g_manager->get_num_windows());
  }

  UNITY_INTERFACE_EXPORT int UNITY_INTERFACE_API GetWindowIDs(int* out_ids, int reserved_length)
  {
    if (!g_manager) {
      return 0;
    }
    auto num_windows = g_manager->get_num_windows();
    if (num_windows > reserved_length) {
      return static_cast<int>(num_windows);
    }

    auto& ids = g_manager->get_ids();
    std::copy(ids.begin(), ids.end(), out_ids);

    return static_cast<int>(num_windows);
  }

  void UNITY_INTERFACE_API OnRenderEvent(int id)
  {
    if (!g_manager) {
      return;
    }
    try {
      g_manager->get_window(id).render();
    } catch (std::out_of_range&) {
    } catch (std::runtime_error&) {
    }
  }

  UNITY_INTERFACE_EXPORT UnityRenderingEvent UNITY_INTERFACE_API GetRenderEventFunc()
  {
    return OnRenderEvent;
  }

  UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API UpdateWindowByID(int id)
  {
    if (!g_manager) {
      return;
    }
    try {
      g_manager->get_window(id).update();
    } catch (std::out_of_range&) {
    }
  }

  UNITY_INTERFACE_EXPORT void* UNITY_INTERFACE_API GetTexture(int id)
  {
    if (!g_manager) {
      return nullptr;
    }
    try {
      return g_manager->get_window(id).get_texture();
    } catch (std::out_of_range&) {
      return nullptr;
    }
  }

}
