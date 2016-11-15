#ifndef COMMON_CGINC
#define COMMON_CGINC

#include "UnityCG.cginc"
#include "./Params.cginc"

struct appdata
{
    float4 vertex : POSITION;
    float2 uv     : TEXCOORD0;
};

struct v2f
{
    float4 vertex : SV_POSITION;
    float2 uv     : TEXCOORD0;
};

struct Input 
{
    float2 uv_MainTex;
};

inline float2 InvertUV(float2 uv)
{
#ifdef INVERT_X
    uv.x = 1.0 - uv.x;
#endif
#ifdef INVERT_Y
    uv.y = 1.0 - uv.y;
#endif
    return uv;
}

inline float2 RotateUV(float2 uv)
{
#ifdef ROTATE90
    float2 tmp = uv;
    uv.x = tmp.y;
    uv.y = 1.0 - tmp.x;
#elif ROTATE180
    uv.x = 1.0 - uv.x;
    uv.y = 1.0 - uv.y;
#elif ROTATE270
    float2 tmp = uv;
    uv.x = 1.0 - tmp.y;
    uv.y = tmp.x;
#endif
    return uv;
}

inline float2 ClipUV(float2 uv)
{
    uv.x = _ClipX + uv.x * _ClipWidth;
    uv.y = _ClipY + uv.y * _ClipHeight;
    return uv;
}

inline void ConvertToLinearIfNeeded(inout fixed3 rgb)
{
    if (!IsGammaSpace()) {
        rgb = GammaToLinearSpace(rgb);
    }
}

inline fixed4 GetScreenTexture(float2 uv)
{
    uv = InvertUV(uv);
#ifdef USE_CLIP
    uv = ClipUV(uv);
#endif
    uv = RotateUV(uv);
    fixed4 c = tex2D(_MainTex, uv);
    ConvertToLinearIfNeeded(c.rgb);
    return c;
}

inline void ResizeVertex(inout float4 v, half width, half height)
{
  v.x *= width;
  v.y *= height;
}

inline void BendVertex(inout float4 v, half radius, half width)
{
#if !defined(_BEND_OFF)
    half angle = width * v.x / radius;
    #ifdef _BEND_Y
    radius -= v.y;
    v.y += radius * (1 - cos(angle));
    #elif _BEND_Z
    radius -= v.z;
    v.z += radius * (1 - cos(angle));
    #endif
    v.x = radius * sin(angle) / width;
#endif
}

#endif
