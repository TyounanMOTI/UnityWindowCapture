#include "window.h"
#include "IUnityInterface.h"
#include "IUnityGraphicsD3D11.h"
#include <cassert>
#include <dxgi.h>
#include <functional>
#include <memory>

using Microsoft::WRL::ComPtr;

namespace {
WINDOWINFO get_window_info(HWND hwnd)
{
  WINDOWINFO info;
  ZeroMemory(&info, sizeof(WINDOWINFO));
  auto status = GetWindowInfo(hwnd, &info);
  if (status == 0) {
    throw std::runtime_error("Failed to get window info.");
  }
  return info;
}
}

namespace window_capture {

window::window()
  : hwnd(nullptr)
{
}

window::window(HWND hwnd)
  : hwnd(hwnd)
{
}

void window::set_texture(ID3D11Resource* texture)
{
  unity_texture = texture;
}

int window::get_width()
{
  assert(hwnd != NULL);
  auto info = get_window_info(hwnd);
  return info.rcWindow.right - info.rcWindow.left;
}

int window::get_height()
{
  assert(hwnd != NULL);
  auto info = get_window_info(hwnd);
  return info.rcWindow.bottom - info.rcWindow.top;
}

UINT window::get_dpi()
{
  return GetDpiForWindow(hwnd);
}

extern IUnityInterfaces* g_unity;

void window::render()
{
  if (!gdi_texture) {
    create_texture();
  } else {
    D3D11_TEXTURE2D_DESC gdi_desc;
    gdi_texture->GetDesc(&gdi_desc);
    if (gdi_desc.Height != get_height()
        || gdi_desc.Width != get_width()) {
      create_texture();
    }
  }

  std::unique_ptr<HDC__, std::function<void(HDC)>>
    target_hdc(GetWindowDC(hwnd), [=](HDC x) { ReleaseDC(hwnd, x); });
  if (!target_hdc) {
    throw std::runtime_error("Failed to get target window HDC.");
  }

  ComPtr<IDXGISurface1> surface;
  auto hr = gdi_texture.As(&surface);
  if (FAILED(hr)) {
    throw std::runtime_error("Failed to get DXGI Surface.");
  }

  HDC gdi_hdc_ptr = nullptr;
  hr = surface->GetDC(TRUE, &gdi_hdc_ptr);
  if (FAILED(hr)) {
    throw std::runtime_error("Failed to get DC.");
  }
  std::unique_ptr<HDC__, std::function<void(HDC)>> gdi_hdc(gdi_hdc_ptr, [=](HDC) { surface->ReleaseDC(NULL); });

  auto device_caps = GetDeviceCaps(target_hdc.get(), RASTERCAPS);
  if (!(device_caps & RC_BITBLT)) {
    throw std::runtime_error("Capturing window device context does not support BitBlt.");
  }

  device_caps = GetDeviceCaps(gdi_hdc.get(), RASTERCAPS);
  if (!(device_caps & RC_BITBLT)) {
    throw std::runtime_error("Output texture device context does not support BitBlt.");
  }

  auto err = BitBlt(gdi_hdc.get(), 0, 0, get_width(), get_height(), target_hdc.get(), 0, 0, SRCCOPY);
  if (err == FALSE) {
    throw std::runtime_error("Failed to BitBlt.");
  }

  gdi_hdc.reset();

  D3D11_RESOURCE_DIMENSION gdi_dimension, unity_dimension;
  gdi_texture->GetType(&gdi_dimension);
  unity_texture->GetType(&unity_dimension);
  if (gdi_dimension != unity_dimension) {
    throw std::runtime_error("Texture dimension is different from Unity texture.");
  }

  D3D11_TEXTURE2D_DESC gdi_desc, unity_desc;
  gdi_texture->GetDesc(&gdi_desc);
  reinterpret_cast<ID3D11Texture2D*>(unity_texture)->GetDesc(&unity_desc);
  if (gdi_desc.Width != unity_desc.Width
      || gdi_desc.Height != unity_desc.Height) {
    throw std::runtime_error("Texture width/height is different from Unity texture.");
  }

  if (gdi_desc.Format != unity_desc.Format) {
    throw std::runtime_error("Texture format is different from Unity texture.");
  }

  auto device = g_unity->Get<IUnityGraphicsD3D11>()->GetDevice();
  ComPtr<ID3D11DeviceContext> context;
  device->GetImmediateContext(&context);
  context->CopyResource(unity_texture, gdi_texture.Get());
}

void window::create_texture()
{
  // ウィンドウサイズ変更に対応するためテクスチャを作り直し
  // キャプチャした画像を保管しておくGDI Compatibleなテクスチャを作成
  D3D11_TEXTURE2D_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.Width = get_width();
  desc.Height = get_height();
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  desc.BindFlags = D3D11_BIND_RENDER_TARGET;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.MiscFlags = D3D11_RESOURCE_MISC_GDI_COMPATIBLE;

  auto device = g_unity->Get<IUnityGraphicsD3D11>()->GetDevice();
  auto hr = device->CreateTexture2D(&desc, nullptr, gdi_texture.ReleaseAndGetAddressOf());
  if (FAILED(hr)) {
    std::runtime_error("Failed to create GDI compatible texture.");
  }
}

}
