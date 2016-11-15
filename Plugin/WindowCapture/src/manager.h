#pragma once

#include <windows.h>
#include <map>
#include <memory>
#include <vector>
#include "window.h"

namespace window_capture {

class manager
{
public:
  using id = int;
  static const id invalid_id = -1;

  id get_foreground_window_id();
  id find_id(const HWND hwnd) const;
  window& get_window(const id request_id);
  std::vector<id> get_ids() const;
  size_t get_num_windows() const;

private:
  std::map<id, window> windows;
  id head_id = 0;
};

extern std::unique_ptr<manager> g_manager;

}
