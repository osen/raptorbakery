#include "Model.h"
#include "../re/util.h"
#include "MaterialGroup.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <re/re.h>

#include <stdio.h>

struct Material
{
  ref(sstream) name;
  ref(ReTexture) texture;
  ref(ReTexture) lightMap;
};

ref(MaterialGroup) ModelMaterialGroup(ref(Model) ctx, ref(ReTexture) texture)
{
  ref(MaterialGroup) rtn = NULL;
  size_t mgi = 0;

  for(mgi = 0; mgi < vector_size(_(ctx).groups); mgi++)
  {
    rtn = vector_at(_(ctx).groups, mgi);

    if(_(rtn).texture == texture)
    {
      return rtn;
    }
  }

  rtn = allocate(MaterialGroup);
  _(rtn).texture = texture;
  _(rtn).mesh = ReContextCreateMesh(_(ctx).context);
  vector_push_back(_(ctx).groups, rtn);

  return rtn;
}

void ModelDestroy(ref(Model) ctx)
{
  size_t mgi = 0;

  for(mgi = 0; mgi < vector_size(_(ctx).groups); mgi++)
  {
    ref(MaterialGroup) mg = vector_at(_(ctx).groups, mgi);

    //ReTextureDestroy(_(mg).texture);
    ReMeshDestroy(_(mg).mesh);
    release(mg);
  }

  vector_delete(_(ctx).groups);
  release(ctx);
}

ref(Model) ModelCreate()
{
  ref(Model) rtn = NULL;

  rtn = allocate(Model);
  _(rtn).groups = vector_new(ref(MaterialGroup));

  return rtn;
}

static void processMtl(ref(Model) ctx, ref(sstream) path, vector(ref(Material)) materials)
{
  ref(ifstream) file = ifstream_open(path);
  ref(sstream) line = sstream_new();

  if(!file)
  {
    panic("Failed to open material file");
  }

  printf("Path: %s\n", sstream_cstr(path));

  while(!ifstream_eof(file))
  {
    ifstream_getline(file, line);
    //printf("Line: %s\n", sstream_cstr(line));
  }

  ifstream_close(file);
  sstream_delete(line);
}

ref(ReTexture) TextureLoad(ref(ReContext) context, char *path)
{
  int w = 0;
  int h = 0;
  int channels = 0;
  unsigned char *data = stbi_load(path, &w, &h, &channels, 4);
  int x = 0;
  int y = 0;
  size_t c = 0;
  ref(ReTexture) rtn = ReContextCreateTexture(context, w, h);

  if(!data)
  {
    panic("Failed to load image");
  }

  for(y = h - 1; y >= 0; y--)
  {
    for(x = 0; x < w; x++)
    {
      ReTextureSetPixel(rtn, x, y, data[c], data[c + 1], data[c + 2], data[c + 3]);
      c += 4;
    }
  }

  free(data);

  return rtn;
}

ref(Model) ModelLoad(ref(ReContext) context, char *path)
{
  ref(Model) rtn = NULL;

  ref(ifstream) file = NULL;
  ref(sstream) filePath = NULL;
  ref(sstream) mtlPath = NULL;
  int ci = 0;
  ref(ReTexture) ct = NULL;
  vector(ref(Material)) materials = NULL;

  ref(sstream) line = NULL;
  vector(ref(sstream)) tokens = NULL;
  vector(struct ReVec3) positions = NULL;
  vector(struct ReVec2) texcoords = NULL;
  vector(struct ReVec3) normals = NULL;
  vector(struct ReVec2) lightmaps = NULL;
  vector(ref(sstream)) sub = NULL;

  tokens = vector_new(ref(sstream));
  positions = vector_new(struct ReVec3);
  texcoords = vector_new(struct ReVec2);
  lightmaps = vector_new(struct ReVec2);
  normals = vector_new(struct ReVec3);
  sub = vector_new(ref(sstream));

  filePath = sstream_new();
  sstream_str_cstr(filePath, path);
  file = ifstream_open(filePath);

  if(!file)
  {
    panic("Failed to open model file");
  }

  for(ci = sstream_length(filePath) - 1; ci >= 0; ci--)
  {
    char c = sstream_at(filePath, ci);

    if(c == '/' || c == '\\')
    {
      break;
    }
  }

  if(ci == -1)
  {
    sstream_str_cstr(filePath, "");
  }
  else
  {
    sstream_erase(filePath, ci, sstream_length(filePath) - ci);
  }

  rtn = ModelCreate();
  line = sstream_new();
  mtlPath = sstream_new();
  materials = vector_new(ref(Material));

  while(!ifstream_eof(file))
  {
    ifstream_getline(file, line);
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
    else if(strcmp(sstream_cstr(vector_at(tokens, 0)), "vt") == 0)
    {
      vector_push_back(texcoords, ReVec2Xy(
        atof(sstream_cstr(vector_at(tokens, 1))),
        atof(sstream_cstr(vector_at(tokens, 2)))));
    }
    else if(strcmp(sstream_cstr(vector_at(tokens, 0)), "vl") == 0)
    {
      vector_push_back(lightmaps, ReVec2Xy(
        atof(sstream_cstr(vector_at(tokens, 1))),
        atof(sstream_cstr(vector_at(tokens, 2)))));
    }
    else if(strcmp(sstream_cstr(vector_at(tokens, 0)), "f") == 0)
    {
      ref(MaterialGroup) mg = NULL;
      struct ReFace f = {0};
      struct ReFace qf = {0};

      mg = ModelMaterialGroup(rtn, ct);

      SplitString(vector_at(tokens, 1), '/', sub);
      if(vector_size(sub) >= 1 && sstream_length(vector_at(sub, 0)) > 0) f.a.position = vector_at(positions, atoi(sstream_cstr(vector_at(sub, 0))) - 1);
      if(vector_size(sub) >= 2 && sstream_length(vector_at(sub, 1)) > 0) f.a.texCoord = vector_at(texcoords, atoi(sstream_cstr(vector_at(sub, 1))) - 1);
      if(vector_size(sub) >= 3 && sstream_length(vector_at(sub, 2)) > 0) f.a.normal = vector_at(normals, atoi(sstream_cstr(vector_at(sub, 2))) - 1);
      if(vector_size(sub) >= 4 && sstream_length(vector_at(sub, 3)) > 0) f.a.lmCoord = vector_at(lightmaps, atoi(sstream_cstr(vector_at(sub, 3))) - 1);

      SplitString(vector_at(tokens, 2), '/', sub);
      if(vector_size(sub) >= 1 && sstream_length(vector_at(sub, 0)) > 0) f.b.position = vector_at(positions, atoi(sstream_cstr(vector_at(sub, 0))) - 1);
      if(vector_size(sub) >= 2 && sstream_length(vector_at(sub, 1)) > 0) f.b.texCoord = vector_at(texcoords, atoi(sstream_cstr(vector_at(sub, 1))) - 1);
      if(vector_size(sub) >= 3 && sstream_length(vector_at(sub, 2)) > 0) f.b.normal = vector_at(normals, atoi(sstream_cstr(vector_at(sub, 2))) - 1);
      if(vector_size(sub) >= 4 && sstream_length(vector_at(sub, 3)) > 0) f.b.lmCoord = vector_at(lightmaps, atoi(sstream_cstr(vector_at(sub, 3))) - 1);

      SplitString(vector_at(tokens, 3), '/', sub);
      if(vector_size(sub) >= 1 && sstream_length(vector_at(sub, 0)) > 0) f.c.position = vector_at(positions, atoi(sstream_cstr(vector_at(sub, 0))) - 1);
      if(vector_size(sub) >= 2 && sstream_length(vector_at(sub, 1)) > 0) f.c.texCoord = vector_at(texcoords, atoi(sstream_cstr(vector_at(sub, 1))) - 1);
      if(vector_size(sub) >= 3 && sstream_length(vector_at(sub, 2)) > 0) f.c.normal = vector_at(normals, atoi(sstream_cstr(vector_at(sub, 2))) - 1);
      if(vector_size(sub) >= 4 && sstream_length(vector_at(sub, 3)) > 0) f.c.lmCoord = vector_at(lightmaps, atoi(sstream_cstr(vector_at(sub, 3))) - 1);

      ReMeshAddFace(_(mg).mesh, f);

      if(vector_size(tokens) > 4)
      {
        qf.a.position = f.c.position;
        qf.a.texCoord = f.c.texCoord;
        qf.a.normal = f.c.normal;
        qf.a.lmCoord = f.c.lmCoord;

        SplitString(vector_at(tokens, 4), '/', sub);
        if(vector_size(sub) >= 1 && sstream_length(vector_at(sub, 0)) > 0) qf.b.position = vector_at(positions, atoi(sstream_cstr(vector_at(sub, 0))) - 1);
        if(vector_size(sub) >= 2 && sstream_length(vector_at(sub, 1)) > 0) qf.b.texCoord = vector_at(texcoords, atoi(sstream_cstr(vector_at(sub, 1))) - 1);
        if(vector_size(sub) >= 3 && sstream_length(vector_at(sub, 2)) > 0) qf.b.normal = vector_at(normals, atoi(sstream_cstr(vector_at(sub, 2))) - 1);
        if(vector_size(sub) >= 4 && sstream_length(vector_at(sub, 3)) > 0) qf.b.lmCoord = vector_at(lightmaps, atoi(sstream_cstr(vector_at(sub, 3))) - 1);

        qf.c.position = f.a.position;
        qf.c.texCoord = f.a.texCoord;
        qf.c.normal = f.a.normal;
        qf.c.lmCoord = f.a.lmCoord;

        ReMeshAddFace(_(mg).mesh, qf);
      }
    }
    else if(strcmp(sstream_cstr(vector_at(tokens, 0)), "mtllib") == 0)
    {
      sstream_str(mtlPath, filePath);
      sstream_append_char(mtlPath, '/');
      sstream_append(mtlPath, vector_at(tokens, 1));
      processMtl(rtn, mtlPath, materials);
    }
  }

  for(ci = 0; ci < vector_size(materials); ci++)
  {
    ref(Material) m = vector_at(materials, ci);

    release(m);
  }

  vector_delete(materials);
  sstream_delete(mtlPath);
  sstream_delete(filePath);
  ifstream_close(file);
  vector_sstream_delete(sub);
  vector_delete(normals);
  vector_delete(positions);
  vector_delete(texcoords);
  vector_delete(lightmaps);
  vector_sstream_delete(tokens);
  sstream_delete(line);

  return rtn;
}

ref(ReMesh) ModelMesh(ref(Model) ctx)
{
  ref(MaterialGroup) mg = vector_at(_(ctx).groups, 0);

  return _(mg).mesh;
}
