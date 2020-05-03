#ifndef RE_MATHUTIL_H
#define RE_MATHUTIL_H

struct ReVec2
{
  float x;
  float y;
};

struct ReVec2 ReVec2Xy(float x, float y);

struct ReVec3
{
  float x;
  float y;
  float z;
};

struct ReVec3 ReVec3Xyz(float x, float y, float z);

struct ReVec4
{
  float x;
  float y;
  float z;
  float w;
};

struct ReVec4 ReVec4Xyzw(float x, float y, float z, float w);
struct ReVec4 ReVec4Rgba(float r, float g, float b, float a);

struct ReMat4
{
  float m[4][4];
};

struct ReMat4 ReMat4Identity();
struct ReMat4 ReMat4Perspective(float fov, float aspect, float near, float far);
struct ReMat4 ReMat4Mul(struct ReMat4 a, struct ReMat4 b);
struct ReMat4 ReMat4Translate(struct ReMat4 ctx, struct ReVec3 pos);
struct ReMat4 ReMat4RotateX(struct ReMat4 ctx, float angle);
struct ReMat4 ReMat4RotateY(struct ReMat4 ctx, float angle);
struct ReMat4 ReMat4RotateZ(struct ReMat4 ctx, float angle);
struct ReMat4 ReMat4Scale(struct ReMat4 ctx, struct ReVec3 scale);
struct ReMat4 ReMat4Inverse(struct ReMat4 ctx);
struct ReMat4 ReMat4Transpose(struct ReMat4 ctx);
struct ReVec4 ReMat4MulVec4(struct ReMat4 ctx, struct ReVec4 val);

#endif
