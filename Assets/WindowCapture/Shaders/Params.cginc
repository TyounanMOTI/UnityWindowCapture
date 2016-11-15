#ifndef _PARAMS_CGINC
#define _PARAMS_CGINC

sampler2D _MainTex;
fixed4 _Color;

half4 _ClipPositionScale;
#define _ClipX _ClipPositionScale.x
#define _ClipY _ClipPositionScale.y
#define _ClipWidth _ClipPositionScale.z
#define _ClipHeight _ClipPositionScale.w

#ifndef SURFACE_SHADER
float4 _MainTex_ST;
#endif

#endif