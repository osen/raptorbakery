#include "Mesh.h"
#include "internal.h"
#include "Buffer.h"
#include "Context.h"
#include "util.h"
#include "mathutil.h"

#include <stdio.h>

struct Face
{
  struct ReVec3 pa;
  struct ReVec3 pb;
  struct ReVec3 pc;

  struct ReVec3 na;
  struct ReVec3 nb;
  struct ReVec3 nc;
};

void ReMeshDestroy(ref(ReMesh) ctx)
{
  if(_(ctx).position)
  {
    ReBufferDestroy(_(ctx).position);
  }

  if(_(ctx).normal)
  {
    ReBufferDestroy(_(ctx).normal);
  }

  if(_(ctx).texture)
  {
    ReBufferDestroy(_(ctx).texture);
  }

  if(_(ctx).lightMap)
  {
    ReBufferDestroy(_(ctx).lightMap);
  }

  release(ctx);
}

ref(ReBuffer) ReMeshPositionBuffer(ref(ReMesh) ctx)
{
  return _(ctx).position;
}

ref(ReBuffer) ReMeshLightMapBuffer(ref(ReMesh) ctx)
{
  return _(ctx).lightMap;
}

ref(ReBuffer) ReMeshTextureBuffer(ref(ReMesh) ctx)
{
  return _(ctx).texture;
}

ref(ReBuffer) ReMeshNormalBuffer(ref(ReMesh) ctx)
{
  return _(ctx).normal;
}

void ReMeshSetPositionBuffer(ref(ReMesh) ctx, ref(ReBuffer) buffer)
{
  _(ctx).position = buffer;
}

void ReMeshSetTextureBuffer(ref(ReMesh) ctx, ref(ReBuffer) buffer)
{
  _(ctx).texture = buffer;
}

void ReMeshSetLightMapBuffer(ref(ReMesh) ctx, ref(ReBuffer) buffer)
{
  _(ctx).lightMap = buffer;
}

void ReMeshSetNormalBuffer(ref(ReMesh) ctx, ref(ReBuffer) buffer)
{
  _(ctx).normal = buffer;
}

void ReMeshParse(ref(ReMesh) ctx, char *objData)
{
  ref(sstream) data = NULL;
  vector(ref(sstream)) lines = NULL;
  size_t li = 0;
  ref(sstream) line = NULL;
  vector(ref(sstream)) tokens = NULL;
  vector(struct ReVec3) positions = NULL;
  vector(struct ReVec3) normals = NULL;
  vector(ref(sstream)) sub = NULL;
  struct Face f = {0};
  struct Face qf = {0};

  data = sstream_new();
  sstream_str_cstr(data, objData);
  lines = vector_new(ref(sstream));
  SplitStringNewline(data, lines);
  sstream_delete(data);
  tokens = vector_new(ref(sstream));
  positions = vector_new(struct ReVec3);
  normals = vector_new(struct ReVec3);
  sub = vector_new(ref(sstream));

  //printf("Parsing: [%s] [%i]\n", objData, (int)vector_size(lines));

  for(li = 0; li < vector_size(lines); li++)
  {
    line = vector_at(lines, li);
    if(sstream_length(line) < 1) continue;
    SplitStringWhitespace(line, tokens);
    if(vector_size(tokens) < 1) continue;

    if(strcmp(sstream_cstr(vector_at(tokens, 0)), "v") == 0)
    {
      vector_push_back(positions, ReVec3Xyz(
        atof(sstream_cstr(vector_at(tokens, 1))),
        atof(sstream_cstr(vector_at(tokens, 2))),
        atof(sstream_cstr(vector_at(tokens, 3)))));
    }
    else if(strcmp(sstream_cstr(vector_at(tokens, 0)), "vn") == 0)
    {
      vector_push_back(normals, ReVec3Xyz(
        atof(sstream_cstr(vector_at(tokens, 1))),
        atof(sstream_cstr(vector_at(tokens, 2))),
        atof(sstream_cstr(vector_at(tokens, 3)))));
    }
    else if(strcmp(sstream_cstr(vector_at(tokens, 0)), "f") == 0)
    {
      SplitString(vector_at(tokens, 1), '/', sub);
      if(vector_size(sub) >= 1) f.pa = vector_at(positions, atoi(sstream_cstr(vector_at(sub, 0))) - 1);
      if(vector_size(sub) >= 3) f.na = vector_at(normals, atoi(sstream_cstr(vector_at(sub, 2))) - 1);

      SplitString(vector_at(tokens, 2), '/', sub);
      if(vector_size(sub) >= 1) f.pb = vector_at(positions, atoi(sstream_cstr(vector_at(sub, 0))) - 1);
      if(vector_size(sub) >= 3) f.nb = vector_at(normals, atoi(sstream_cstr(vector_at(sub, 2))) - 1);

      SplitString(vector_at(tokens, 3), '/', sub);
      if(vector_size(sub) >= 1) f.pc = vector_at(positions, atoi(sstream_cstr(vector_at(sub, 0))) - 1);
      if(vector_size(sub) >= 3) f.nc = vector_at(normals, atoi(sstream_cstr(vector_at(sub, 2))) - 1);

      if(vector_size(positions) > 0)
      {
        if(!_(ctx).position) _(ctx).position = ReContextCreateBuffer(_(ctx).context);
        ReBufferAddVec3(_(ctx).position, f.pa);
        ReBufferAddVec3(_(ctx).position, f.pb);
        ReBufferAddVec3(_(ctx).position, f.pc);
      }

      if(vector_size(normals) > 0)
      {
        if(!_(ctx).normal) _(ctx).normal = ReContextCreateBuffer(_(ctx).context);
        ReBufferAddVec3(_(ctx).normal, f.na);
        ReBufferAddVec3(_(ctx).normal, f.nb);
        ReBufferAddVec3(_(ctx).normal, f.nc);
      }

      if(vector_size(tokens) < 5) continue;
      qf.pa = f.pc;
      qf.na = f.nc;

      SplitString(vector_at(tokens, 4), '/', sub);
      if(vector_size(sub) >= 1) qf.pb = vector_at(positions, atoi(sstream_cstr(vector_at(sub, 0))) - 1);
      if(vector_size(sub) >= 3) qf.nb = vector_at(normals, atoi(sstream_cstr(vector_at(sub, 2))) - 1);

      qf.pc = f.pa;
      qf.nc = f.na;

      if(vector_size(positions) > 0)
      {
        ReBufferAddVec3(_(ctx).position, qf.pa);
        ReBufferAddVec3(_(ctx).position, qf.pb);
        ReBufferAddVec3(_(ctx).position, qf.pc);
      }

      if(vector_size(normals) > 0)
      {
        ReBufferAddVec3(_(ctx).normal, qf.na);
        ReBufferAddVec3(_(ctx).normal, qf.nb);
        ReBufferAddVec3(_(ctx).normal, qf.nc);
      }
    }
  }

  vector_sstream_delete(sub);
  vector_delete(normals);
  vector_delete(positions);
  vector_sstream_delete(tokens);
  vector_sstream_delete(lines);
}
