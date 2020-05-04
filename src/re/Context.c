#include "Context.h"
#include "internal.h"
#include "Mesh.h"
#include "primitives.h"
#include "Texture.h"
#include "Face.h"

#define STENT_IMPLEMENTATION
#include "stent.h"

ref(ReContext) ReContextCreate()
{
  ref(ReContext) rtn = NULL;

  rtn = allocate(ReContext);

  if(glewInit() != GLEW_OK)
  {
    panic("Failed to initialize glew");
  }

  return rtn;
}

void ReContextDestroy(ref(ReContext) ctx)
{
  release(ctx);
}

ref(ReRenderer) ReContextCreateRenderer(ref(ReContext) ctx)
{
  ref(ReRenderer) rtn = NULL;

  rtn = allocate(ReRenderer);
  _(rtn).shader = _ReShaderCreate(ctx);
  _(rtn).blend = 1;
  _(rtn).backfaceCull = 1;
  _(rtn).depthTest = 1;
  _(rtn).projection = ReMat4Identity();
  _(rtn).view = ReMat4Identity();
  _(rtn).model = ReMat4Identity();
  _(rtn).color = ReVec4Rgba(1, 1, 1, 1);

  return rtn;
}

ref(ReBuffer) ReContextCreateBuffer(ref(ReContext) ctx)
{
  ref(ReBuffer) rtn = NULL;

  rtn = allocate(ReBuffer);
  _(rtn).data = vector_new(float);

  glGenBuffers(1, &_(rtn).id);
  _RePollForError();

  if(_(rtn).id == 0)
  {
    panic("Failed to generate buffer");
  }

  return rtn;
}

ref(ReTexture) ReContextCreateTexture(ref(ReContext) ctx, int width, int height)
{
  ref(ReTexture) rtn = NULL;

  rtn = allocate(ReTexture);
  _(rtn).data = vector_new(unsigned char);
  vector_resize(_(rtn).data, width * height * 4);
  _(rtn).width = width;
  _(rtn).height = height;

  glGenTextures(1, &_(rtn).id);
  _RePollForError();

  if(_(rtn).id == 0)
  {
    panic("Failed to generate texture");
  }

  return rtn;
}

ref(ReMesh) ReContextCreateMesh(ref(ReContext) ctx)
{
  ref(ReMesh) rtn = NULL;

  rtn = allocate(ReMesh);
  _(rtn).context = ctx;
  _(rtn).faces = vector_new(struct ReFace);

  return rtn;
}

ref(ReMesh) ReContextCreatePrimitive(ref(ReContext) ctx, int type)
{
  ref(ReMesh) rtn = NULL;

  rtn = ReContextCreateMesh(ctx);

  if(type == RE_PRIMITIVE_TRIANGLE)
  {
    ReMeshParse(rtn, triangleObj);
  }
  else if(type == RE_PRIMITIVE_SQUARE)
  {
    ReMeshParse(rtn, squareObj);
  }
  else if(type == RE_PRIMITIVE_CUBE)
  {
    ReMeshParse(rtn, cubeObj);
  }
  else if(type == RE_PRIMITIVE_SPHERE)
  {
    ReMeshParse(rtn, sphereObj);
  }
  else
  {
    panic("Invalid primitive specified");
  }

  return rtn;
}
