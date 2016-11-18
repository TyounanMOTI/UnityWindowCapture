#pragma once
#include <Windows.h>
#include <string>
#include <d3d11.h>
#include <wrl/client.h>

namespace window_capture {

class window
{
public:
  window();
  explicit window(HWND hwnd);

  HWND get_hwnd() const { return hwnd; }
  void set_texture(ID3D11Resource* texture);
  int get_width();
  int get_height();
  UINT get_dpi();
  void render();
  void create_texture();

private:
  HWND hwnd;
  ID3D11Resource* unity_texture = nullptr;
  Microsoft::WRL::ComPtr<ID3D11Texture2D> gdi_texture;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> gdi_srv;
};

}
