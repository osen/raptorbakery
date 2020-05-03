#include "mathutil.h"

#include <math.h>

struct ReVec2 ReVec2Xy(float x, float y)
{
  struct ReVec2 rtn = {0};

  rtn.x = x;
  rtn.y = y;

  return rtn;
}

struct ReVec3 ReVec3Xyz(float x, float y, float z)
{
  struct ReVec3 rtn = {0};

  rtn.x = x;
  rtn.y = y;
  rtn.z = z;

  return rtn;
}

struct ReVec4 ReVec4Xyzw(float x, float y, float z, float w)
{
  struct ReVec4 rtn = {0};

  rtn.x = x;
  rtn.y = y;
  rtn.z = z;
  rtn.w = w;

  return rtn;
}

struct ReVec4 ReVec4Rgba(float r, float g, float b, float a)
{
  return ReVec4Xyzw(r, g, b, a);
}

struct ReMat4 ReMat4Identity()
{
  struct ReMat4 rtn = {0};

  rtn.m[0][0] = 1;
  rtn.m[1][1] = 1;
  rtn.m[2][2] = 1;
  rtn.m[3][3] = 1;

  return rtn;
}

struct ReMat4 ReMat4Perspective(float fov, float aspect, float near, float far)
{
  struct ReMat4 rtn = {0};

 fov = fov * M_PI / 180.0;
 float const a = 1.f / tan(fov / 2.f);

  rtn.m[0][0] = a / aspect;
  rtn.m[0][1] = 0.f;
  rtn.m[0][2] = 0.f;
  rtn.m[0][3] = 0.f;

  rtn.m[1][0] = 0.f;
  rtn.m[1][1] = a;
  rtn.m[1][2] = 0.f;
  rtn.m[1][3] = 0.f;

  rtn.m[2][0] = 0.f;
  rtn.m[2][1] = 0.f;
  rtn.m[2][2] = -((far + near) / (far - near));
  rtn.m[2][3] = -1.f;

  rtn.m[3][0] = 0.f;
  rtn.m[3][1] = 0.f;
  rtn.m[3][2] = -((2.f * far * near) / (far - near));
  rtn.m[3][3] = 0.f;

  return rtn;
}

struct ReMat4 ReMat4Mul(struct ReMat4 a, struct ReMat4 b)
{
  struct ReMat4 rtn = {0};
  int k = 0;
  int r = 0;
  int c = 0;

  for(c = 0; c < 4; ++c)
  {
    for(r = 0; r < 4; ++r)
    {
      rtn.m[c][r] = 0.f;

      for(k = 0; k < 4; ++k)
      {
        rtn.m[c][r] += a.m[k][r] * b.m[c][k];
      }
    }
  }

  return rtn;
}

struct ReMat4 ReMat4RotateY(struct ReMat4 ctx, float angle)
{
  struct ReMat4 rtn = {0};
  float s = 0;
  float c = 0;

  angle = angle * M_PI / 180.0;

  s = sinf(angle);
  c = cosf(angle);

  rtn.m[0][0] = c;
  rtn.m[0][1] = 0.f;
  rtn.m[0][2] = s;
  rtn.m[0][3] = 0.f;

  rtn.m[1][0] = 0.f;
  rtn.m[1][1] = 1.f;
  rtn.m[1][2] = 0.f;
  rtn.m[1][3] = 0.f;

  rtn.m[2][0] = -s;
  rtn.m[2][1] = 0.f;
  rtn.m[2][2] = c;
  rtn.m[2][3] = 0.f;

  rtn.m[3][0] = 0.f;
  rtn.m[3][1] = 0.f;
  rtn.m[3][2] = 0.f;
  rtn.m[3][3] = 1.f;

  return ReMat4Mul(ctx, rtn);
}

struct ReMat4 ReMat4RotateZ(struct ReMat4 ctx, float angle)
{
  struct ReMat4 rtn = {0};
  float s = 0;
  float c = 0;

  angle = angle * M_PI / 180.0;

  s = sinf(angle);
  c = cosf(angle);

  rtn.m[0][0] = c;
  rtn.m[0][1] = s;
  rtn.m[0][2] = 0.f;
  rtn.m[0][3] = 0.f;

  rtn.m[1][0] = -s;
  rtn.m[1][1] = c;
  rtn.m[1][2] = 0.f;
  rtn.m[1][3] = 0.f;

  rtn.m[2][0] = 0.f;
  rtn.m[2][1] = 0.f;
  rtn.m[2][2] = 1.f;
  rtn.m[2][3] = 0.f;

  rtn.m[3][0] = 0.f;
  rtn.m[3][1] = 0.f;
  rtn.m[3][2] = 0.f;
  rtn.m[3][3] = 1.f;

  return ReMat4Mul(ctx, rtn);
}


struct ReMat4 ReMat4RotateX(struct ReMat4 ctx, float angle)
{
  struct ReMat4 rtn = {0};
  float s = 0;
  float c = 0;

  angle = angle * M_PI / 180.0;

  s = sinf(angle);
  c = cosf(angle);

  rtn.m[0][0] = 1.f;
  rtn.m[0][1] = 0.f;
  rtn.m[0][2] = 0.f;
  rtn.m[0][3] = 0.f;

  rtn.m[1][0] = 0.f;
  rtn.m[1][1] = c;
  rtn.m[1][2] = s;
  rtn.m[1][3] = 0.f;

  rtn.m[2][0] = 0.f;
  rtn.m[2][1] = -s;
  rtn.m[2][2] = c;
  rtn.m[2][3] = 0.f;

  rtn.m[3][0] = 0.f;
  rtn.m[3][1] = 0.f;
  rtn.m[3][2] = 0.f;
  rtn.m[3][3] = 1.f;

  return ReMat4Mul(ctx, rtn);
}

struct ReMat4 ReMat4Translate(struct ReMat4 ctx, struct ReVec3 pos)
{
  struct ReMat4 rtn = ReMat4Identity();

  rtn.m[3][0] = pos.x;
  rtn.m[3][1] = pos.y;
  rtn.m[3][2] = pos.z;

  return ReMat4Mul(ctx, rtn);
}

struct ReMat4 ReMat4Scale(struct ReMat4 ctx, struct ReVec3 scale)
{
  int i = 0;
  struct ReMat4 rtn = ReMat4Identity();

  for(i = 0; i < 4; ++i)
  {
    rtn.m[0][i] = ctx.m[0][i] * scale.x;
    rtn.m[1][i] = ctx.m[1][i] * scale.y;
    rtn.m[2][i] = ctx.m[2][i] * scale.z;
    rtn.m[3][i] = ctx.m[3][i];
  }

  return rtn;
}

struct ReMat4 ReMat4Inverse(struct ReMat4 ctx)
{
  struct ReMat4 rtn = {0};

  float s[6] = {0};
  float c[6] = {0};
  s[0] = ctx.m[0][0]*ctx.m[1][1] - ctx.m[1][0]*ctx.m[0][1];
  s[1] = ctx.m[0][0]*ctx.m[1][2] - ctx.m[1][0]*ctx.m[0][2];
  s[2] = ctx.m[0][0]*ctx.m[1][3] - ctx.m[1][0]*ctx.m[0][3];
  s[3] = ctx.m[0][1]*ctx.m[1][2] - ctx.m[1][1]*ctx.m[0][2];
  s[4] = ctx.m[0][1]*ctx.m[1][3] - ctx.m[1][1]*ctx.m[0][3];
  s[5] = ctx.m[0][2]*ctx.m[1][3] - ctx.m[1][2]*ctx.m[0][3];

  c[0] = ctx.m[2][0]*ctx.m[3][1] - ctx.m[3][0]*ctx.m[2][1];
  c[1] = ctx.m[2][0]*ctx.m[3][2] - ctx.m[3][0]*ctx.m[2][2];
  c[2] = ctx.m[2][0]*ctx.m[3][3] - ctx.m[3][0]*ctx.m[2][3];
  c[3] = ctx.m[2][1]*ctx.m[3][2] - ctx.m[3][1]*ctx.m[2][2];
  c[4] = ctx.m[2][1]*ctx.m[3][3] - ctx.m[3][1]*ctx.m[2][3];
  c[5] = ctx.m[2][2]*ctx.m[3][3] - ctx.m[3][2]*ctx.m[2][3];

  /* Assumes it is invertible */
  float idet = 1.0f/(s[0]*c[5]-s[1]*c[4]+s[2]*c[3]+s[3]*c[2]-s[4]*c[1]+s[5]*c[0]);

  rtn.m[0][0] = ( ctx.m[1][1] * c[5] - ctx.m[1][2] * c[4] + ctx.m[1][3] * c[3]) * idet;
  rtn.m[0][1] = (-ctx.m[0][1] * c[5] + ctx.m[0][2] * c[4] - ctx.m[0][3] * c[3]) * idet;
  rtn.m[0][2] = ( ctx.m[3][1] * s[5] - ctx.m[3][2] * s[4] + ctx.m[3][3] * s[3]) * idet;
  rtn.m[0][3] = (-ctx.m[2][1] * s[5] + ctx.m[2][2] * s[4] - ctx.m[2][3] * s[3]) * idet;

  rtn.m[1][0] = (-ctx.m[1][0] * c[5] + ctx.m[1][2] * c[2] - ctx.m[1][3] * c[1]) * idet;
  rtn.m[1][1] = ( ctx.m[0][0] * c[5] - ctx.m[0][2] * c[2] + ctx.m[0][3] * c[1]) * idet;
  rtn.m[1][2] = (-ctx.m[3][0] * s[5] + ctx.m[3][2] * s[2] - ctx.m[3][3] * s[1]) * idet;
  rtn.m[1][3] = ( ctx.m[2][0] * s[5] - ctx.m[2][2] * s[2] + ctx.m[2][3] * s[1]) * idet;

  rtn.m[2][0] = ( ctx.m[1][0] * c[4] - ctx.m[1][1] * c[2] + ctx.m[1][3] * c[0]) * idet;
  rtn.m[2][1] = (-ctx.m[0][0] * c[4] + ctx.m[0][1] * c[2] - ctx.m[0][3] * c[0]) * idet;
  rtn.m[2][2] = ( ctx.m[3][0] * s[4] - ctx.m[3][1] * s[2] + ctx.m[3][3] * s[0]) * idet;
  rtn.m[2][3] = (-ctx.m[2][0] * s[4] + ctx.m[2][1] * s[2] - ctx.m[2][3] * s[0]) * idet;

  rtn.m[3][0] = (-ctx.m[1][0] * c[3] + ctx.m[1][1] * c[1] - ctx.m[1][2] * c[0]) * idet;
  rtn.m[3][1] = ( ctx.m[0][0] * c[3] - ctx.m[0][1] * c[1] + ctx.m[0][2] * c[0]) * idet;
  rtn.m[3][2] = (-ctx.m[3][0] * s[3] + ctx.m[3][1] * s[1] - ctx.m[3][2] * s[0]) * idet;
  rtn.m[3][3] = ( ctx.m[2][0] * s[3] - ctx.m[2][1] * s[1] + ctx.m[2][2] * s[0]) * idet;

  return rtn;
}

struct ReMat4 ReMat4Transpose(struct ReMat4 ctx)
{
  struct ReMat4 rtn = {0};
  int i = 0;
  int j = 0;

  for(j = 0; j < 4; ++j)
  {
    for(i = 0; i < 4; ++i)
    {
      rtn.m[i][j] = ctx.m[j][i];
    }
  }

  return rtn;
}

struct ReVec4 ReMat4MulVec4(struct ReMat4 ctx, struct ReVec4 val)
{
  int i = 0;
  int j = 0;
  float v[4] = {0};
  float r[4] = {0};
  struct ReVec4 rtn = {0};

  v[0] = val.x;
  v[1] = val.y;
  v[2] = val.z;
  v[3] = val.w;

  for(j = 0; j < 4; ++j)
  {
    r[j] = 0.f;

    for(i=0; i<4; ++i)
    {
      r[j] += ctx.m[i][j] * v[i];
    }
  }

  rtn.x = r[0];
  rtn.y = r[1];
  rtn.z = r[2];
  rtn.w = r[3];

  return rtn;
}
