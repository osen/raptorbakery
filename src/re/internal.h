#include "stent.h"
#include "mathutil.h"

#include <GL/glew.h>

struct ReContext
{
  int todo;
};

struct ReBuffer
{
  GLuint id;
  int type;
  int dirty;
  vector(float) data;
};

GLuint _ReBufferId(ref(ReBuffer) ctx);
int _ReBufferType(ref(ReBuffer) ctx);

struct ReMesh
{
  ref(ReContext) context;
  ref(ReBuffer) position;
  ref(ReBuffer) normal;
  ref(ReBuffer) texture;
  ref(ReBuffer) lightMap;
};

struct ReShader
{
  GLuint id;
  GLint colorLoc;
  GLint modelLoc;
  GLint viewLoc;
  GLint projectionLoc;
  GLint normalLoc;
  GLint textureLoc;
  GLint lightMapLoc;
  GLint enableLightingLoc;
  GLint enableTexturingLoc;
};

ref(ReShader) _ReShaderCreate(ref(ReContext) context);
void _ReShaderDestroy(ref(ReShader) ctx);
GLuint _ReShaderId(ref(ReShader) ctx);
GLint _ReShaderColorLoc(ref(ReShader) ctx);
GLint _ReShaderModelLoc(ref(ReShader) ctx);
GLint _ReShaderViewLoc(ref(ReShader) ctx);
GLint _ReShaderProjectionLoc(ref(ReShader) ctx);
GLint _ReShaderNormalLoc(ref(ReShader) ctx);
GLint _ReShaderTextureLoc(ref(ReShader) ctx);
GLint _ReShaderLightMapLoc(ref(ReShader) ctx);
GLint _ReShaderEnableLightingLoc(ref(ReShader) ctx);
GLint _ReShaderEnableTexturingLoc(ref(ReShader) ctx);

struct ReRenderer
{
  ref(ReShader) shader;
  ref(ReBuffer) position;
  ref(ReBuffer) normal;
  ref(ReBuffer) texture;
  ref(ReBuffer) lightMap;

  ref(ReTexture) tSampler;
  ref(ReTexture) lmSampler;

  int depthTest;
  int backfaceCull;
  int blend;
  int lighting;
  int texturing;
  struct ReVec4 color;
  struct ReMat4 model;
  struct ReMat4 view;
  struct ReMat4 projection;
};

void _RePollForError();
