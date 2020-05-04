#include "Mesh.h"
#include "internal.h"
#include "Buffer.h"
#include "Context.h"
#include "util.h"
#include "mathutil.h"
#include "Face.h"

#include <stdio.h>

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

  vector_delete(_(ctx).faces);
  release(ctx);
}

static void upload(ref(ReMesh) ctx)
{
  size_t fi = 0;

  if(_(ctx).position)
  {
    for(fi = 0; fi < vector_size(_(ctx).faces); fi++)
    {
      struct ReFace f = vector_at(_(ctx).faces, fi);
      ReBufferAddVec3(_(ctx).position, f.a.position);
      ReBufferAddVec3(_(ctx).position, f.b.position);
      ReBufferAddVec3(_(ctx).position, f.c.position);
    }
  }

  if(_(ctx).normal)
  {
    for(fi = 0; fi < vector_size(_(ctx).faces); fi++)
    {
      struct ReFace f = vector_at(_(ctx).faces, fi);
      ReBufferAddVec3(_(ctx).normal, f.a.normal);
      ReBufferAddVec3(_(ctx).normal, f.b.normal);
      ReBufferAddVec3(_(ctx).normal, f.c.normal);
    }
  }

  if(_(ctx).texture)
  {
    for(fi = 0; fi < vector_size(_(ctx).faces); fi++)
    {
      struct ReFace f = vector_at(_(ctx).faces, fi);
      ReBufferAddVec2(_(ctx).texture, f.a.texCoord);
      ReBufferAddVec2(_(ctx).texture, f.b.texCoord);
      ReBufferAddVec2(_(ctx).texture, f.c.texCoord);
    }
  }

  if(_(ctx).lightMap)
  {
    for(fi = 0; fi < vector_size(_(ctx).faces); fi++)
    {
      struct ReFace f = vector_at(_(ctx).faces, fi);
      ReBufferAddVec2(_(ctx).lightMap, f.a.lmCoord);
      ReBufferAddVec2(_(ctx).lightMap, f.b.lmCoord);
      ReBufferAddVec2(_(ctx).lightMap, f.c.lmCoord);
    }
  }

  _(ctx).dirty = 0;
}

ref(ReBuffer) ReMeshPositionBuffer(ref(ReMesh) ctx)
{
  if(_(ctx).dirty) upload(ctx);
  return _(ctx).position;
}

ref(ReBuffer) ReMeshLightMapBuffer(ref(ReMesh) ctx)
{
  if(_(ctx).dirty) upload(ctx);
  return _(ctx).lightMap;
}

ref(ReBuffer) ReMeshTextureBuffer(ref(ReMesh) ctx)
{
  if(_(ctx).dirty) upload(ctx);
  return _(ctx).texture;
}

ref(ReBuffer) ReMeshNormalBuffer(ref(ReMesh) ctx)
{
  if(_(ctx).dirty) upload(ctx);
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
  struct ReFace f = {0};
  struct ReFace qf = {0};

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
      if(vector_size(sub) >= 1) f.a.position = vector_at(positions, atoi(sstream_cstr(vector_at(sub, 0))) - 1);
      if(vector_size(sub) >= 3) f.a.normal = vector_at(normals, atoi(sstream_cstr(vector_at(sub, 2))) - 1);

      SplitString(vector_at(tokens, 2), '/', sub);
      if(vector_size(sub) >= 1) f.b.position = vector_at(positions, atoi(sstream_cstr(vector_at(sub, 0))) - 1);
      if(vector_size(sub) >= 3) f.b.normal = vector_at(normals, atoi(sstream_cstr(vector_at(sub, 2))) - 1);

      SplitString(vector_at(tokens, 3), '/', sub);
      if(vector_size(sub) >= 1) f.c.position = vector_at(positions, atoi(sstream_cstr(vector_at(sub, 0))) - 1);
      if(vector_size(sub) >= 3) f.c.normal = vector_at(normals, atoi(sstream_cstr(vector_at(sub, 2))) - 1);

      ReMeshAddFace(ctx, f);

      if(vector_size(tokens) < 5) continue;
      qf.a.position = f.c.position;
      qf.a.normal = f.c.normal;

      SplitString(vector_at(tokens, 4), '/', sub);
      if(vector_size(sub) >= 1) qf.b.position = vector_at(positions, atoi(sstream_cstr(vector_at(sub, 0))) - 1);
      if(vector_size(sub) >= 3) qf.b.normal = vector_at(normals, atoi(sstream_cstr(vector_at(sub, 2))) - 1);

      qf.c.position = f.a.position;
      qf.c.normal = f.a.normal;

      ReMeshAddFace(ctx, qf);
    }
  }

  vector_sstream_delete(sub);
  vector_delete(normals);
  vector_delete(positions);
  vector_sstream_delete(tokens);
  vector_sstream_delete(lines);
}

void ReMeshAddFace(ref(ReMesh) ctx, struct ReFace face)
{
  vector_push_back(_(ctx).faces, face);
  _(ctx).dirty = 1;

  if(!_(ctx).position)
  {
    if(face.a.position.x != 0 ||
      face.a.position.y != 0 ||
      face.a.position.z != 0 ||
      face.b.position.x != 0 ||
      face.b.position.y != 0 ||
      face.b.position.z != 0 ||
      face.c.position.x != 0 ||
      face.c.position.y != 0 ||
      face.c.position.z != 0)
    {
      _(ctx).position = ReContextCreateBuffer(_(ctx).context);
    }
  }

  if(!_(ctx).normal)
  {
    if(face.a.normal.x != 0 ||
      face.a.normal.y != 0 ||
      face.a.normal.z != 0 ||
      face.b.normal.x != 0 ||
      face.b.normal.y != 0 ||
      face.b.normal.z != 0 ||
      face.c.normal.x != 0 ||
      face.c.normal.y != 0 ||
      face.c.normal.z != 0)
    {
      _(ctx).normal = ReContextCreateBuffer(_(ctx).context);
    }
  }

  if(!_(ctx).texture)
  {
    if(face.a.texCoord.x != 0 ||
      face.a.texCoord.y != 0 ||
      face.b.texCoord.x != 0 ||
      face.b.texCoord.y != 0 ||
      face.c.texCoord.x != 0 ||
      face.c.texCoord.y != 0)
    {
      _(ctx).texture = ReContextCreateBuffer(_(ctx).context);
    }
  }

  if(!_(ctx).lightMap)
  {
    if(face.a.lmCoord.x != 0 ||
      face.a.lmCoord.y != 0 ||
      face.b.lmCoord.x != 0 ||
      face.b.lmCoord.y != 0 ||
      face.c.lmCoord.x != 0 ||
      face.c.lmCoord.y != 0)
    {
      _(ctx).lightMap = ReContextCreateBuffer(_(ctx).context);
    }
  }
}
