#include "internal.h"

const char *vertSrc =
  "attribute vec4 a_Position;                                    " \
  "attribute vec2 a_Texture;                                     " \
  "attribute vec3 a_Normal;                                      " \
  "attribute vec2 a_LightMap;                                    " \
  "                                                              " \
  "uniform mat4 u_Model;                                         " \
  "uniform mat4 u_View;                                          " \
  "uniform mat4 u_Projection;                                    " \
  "uniform mat4 u_Normal;                                        " \
  "uniform bool u_EnableLighting;                                " \
  "uniform bool u_EnableTexturing;                               " \
  "                                                              " \
  "varying vec2 v_Texture;                                       " \
  "varying vec2 v_LightMap;                                      " \
  "varying vec3 v_Normal;                                        " \
  "varying vec3 v_FragPos;                                       " \
  "                                                              " \
  "void main()                                                   " \
  "{                                                             " \
  "  if(u_EnableLighting)                                        " \
  "  {                                                           " \
  "    v_Normal = vec3(u_Normal * vec4(a_Normal, 0));            " \
  "    v_FragPos = vec3(u_Model * a_Position);                   " \
  "  }                                                           " \
  "  if(u_EnableTexturing)                                       " \
  "  {                                                           " \
  "    v_Texture = a_Texture;                                    " \
  "    v_LightMap = a_LightMap;                                  " \
  "  }                                                           " \
  "  gl_Position = u_Projection * u_View * u_Model * a_Position; " \
  "}                                                             " \
  "                                                              ";

/*
  "  v_Normal = vec3(transpose(inverse(u_Model)) * vec4(a_Normal, 0)); " \
  "  v_Normal = vec3(u_Model * vec4(a_Normal, 0));                 " \
*/

const char *fragSrc =
  "uniform vec4 u_Color;                               " \
  "uniform bool u_EnableLighting;                      " \
  "uniform bool u_EnableTexturing;                     " \
  "uniform sampler2D u_Texture;                        " \
  "uniform sampler2D u_LightMap;                       " \
  "                                                    " \
  "varying vec2 v_Texture;                             " \
  "varying vec2 v_LightMap;                            " \
  "varying vec3 v_Normal;                              " \
  "varying vec3 v_FragPos;                             " \
  "                                                    " \
  "void main()                                         " \
  "{                                                   " \
  "  vec4 col = vec4(1, 1, 1, 1) * u_Color;            " \
  "  if(u_EnableTexturing)                             " \
  "  {                                                 " \
  "    col = texture2D(u_Texture, v_Texture);          " \
  "  }                                                 " \
  "  if(u_EnableLighting)                              " \
  "  {                                                 " \
  "    vec3 lightPos = vec3(50, 100, 75);              " \
  "    vec3 N = normalize(v_Normal);                   " \
  "    vec3 L = normalize(lightPos - v_FragPos);       " \
  "    float diff = max(dot(N, L), 0.0);               " \
  "    col.xyz = col.xyz * diff;                       " \
  "  }                                                 " \
  "  else                                              " \
  "  {                                                 " \
  "    vec4 lm = texture2D(u_LightMap, v_LightMap);    " \
  "    col = col * lm;                                 " \
  "  }                                                 " \
  "  if(col.w < 0.5) discard;                          " \
  "  gl_FragColor = col;                               " \
  "}                                                   ";

ref(ReShader) _ReShaderCreate(ref(ReContext) context)
{
  ref(ReShader) rtn = NULL;
  GLuint vertId = 0;
  GLuint fragId = 0;
  int success = 0;
  int length = 0;
  vector(GLchar) infoLog = NULL;

  rtn = allocate(ReShader);
  _(rtn).id = glCreateProgram();
  _RePollForError();

  if(_(rtn).id == 0)
  {
    panic("Failed to create shader program");
  }

  vertId = glCreateShader(GL_VERTEX_SHADER);
  _RePollForError();

  glShaderSource(vertId, 1, &vertSrc, NULL);
  _RePollForError();

  glCompileShader(vertId);
  _RePollForError();

  glGetShaderiv(vertId, GL_COMPILE_STATUS, &success);
  _RePollForError();

  if(!success)
  {
    glGetShaderiv(vertId, GL_INFO_LOG_LENGTH, &length);
    _RePollForError();

    infoLog = vector_new(GLchar);
    vector_resize(infoLog, length);

    glGetShaderInfoLog(vertId, length, NULL, &vector_at(infoLog, 0));
    _RePollForError();

    panic(&vector_at(infoLog, 0));
  }

  fragId = glCreateShader(GL_FRAGMENT_SHADER);
  _RePollForError();

  glShaderSource(fragId, 1, &fragSrc, NULL);
  _RePollForError();

  glCompileShader(fragId);
  _RePollForError();

  glGetShaderiv(fragId, GL_COMPILE_STATUS, &success);
  _RePollForError();

  if(!success)
  {
    glGetShaderiv(fragId, GL_INFO_LOG_LENGTH, &length);
    _RePollForError();

    infoLog = vector_new(GLchar);
    vector_resize(infoLog, length);

    glGetShaderInfoLog(fragId, length, NULL, &vector_at(infoLog, 0));
    _RePollForError();

    panic(&vector_at(infoLog, 0));
  }

  glAttachShader(_(rtn).id, vertId);
  _RePollForError();

  glAttachShader(_(rtn).id, fragId);
  _RePollForError();

  glBindAttribLocation(_(rtn).id, 0, "a_Position");
  _RePollForError();

  glBindAttribLocation(_(rtn).id, 1, "a_Texture");
  _RePollForError();

  glBindAttribLocation(_(rtn).id, 2, "a_Normal");
  _RePollForError();

  glBindAttribLocation(_(rtn).id, 3, "a_LightMap");
  _RePollForError();

  glLinkProgram(_(rtn).id);
  _RePollForError();

  glGetProgramiv(_(rtn).id, GL_LINK_STATUS, &success);
  _RePollForError();

  if(!success)
  {
    glGetProgramiv(_(rtn).id, GL_INFO_LOG_LENGTH, &length);
    _RePollForError();

    infoLog = vector_new(GLchar);
    vector_resize(infoLog, length);

    glGetProgramInfoLog(_(rtn).id, length, NULL, &vector_at(infoLog, 0));
    _RePollForError();

    panic(&vector_at(infoLog, 0));
  }

  glDetachShader(_(rtn).id, vertId);
  _RePollForError();

  glDetachShader(_(rtn).id, fragId);
  _RePollForError();

  glDeleteShader(vertId);
  _RePollForError();

  glDeleteShader(fragId);
  _RePollForError();

  _(rtn).colorLoc = glGetUniformLocation(_(rtn).id, "u_Color");
  _RePollForError();
  if(_(rtn).colorLoc == -1) panic("Shader did not provide specified uniform");

  _(rtn).modelLoc = glGetUniformLocation(_(rtn).id, "u_Model");
  _RePollForError();
  if(_(rtn).modelLoc == -1) panic("Shader did not provide specified uniform");

  _(rtn).viewLoc = glGetUniformLocation(_(rtn).id, "u_View");
  _RePollForError();
  if(_(rtn).viewLoc == -1) panic("Shader did not provide specified uniform");

  _(rtn).projectionLoc = glGetUniformLocation(_(rtn).id, "u_Projection");
  _RePollForError();
  if(_(rtn).projectionLoc == -1) panic("Shader did not provide specified uniform");

  _(rtn).normalLoc = glGetUniformLocation(_(rtn).id, "u_Normal");
  _RePollForError();
  if(_(rtn).normalLoc == -1) panic("Shader did not provide specified uniform");

  _(rtn).textureLoc = glGetUniformLocation(_(rtn).id, "u_Texture");
  _RePollForError();
  if(_(rtn).textureLoc == -1) panic("Shader did not provide specified uniform");

  _(rtn).lightMapLoc = glGetUniformLocation(_(rtn).id, "u_LightMap");
  _RePollForError();
  if(_(rtn).lightMapLoc == -1) panic("Shader did not provide specified uniform");

  _(rtn).enableLightingLoc = glGetUniformLocation(_(rtn).id, "u_EnableLighting");
  _RePollForError();
  if(_(rtn).enableLightingLoc == -1) panic("Shader did not provide specified uniform");

  _(rtn).enableTexturingLoc = glGetUniformLocation(_(rtn).id, "u_EnableTexturing");
  _RePollForError();
  if(_(rtn).enableTexturingLoc == -1) panic("Shader did not provide specified uniform");

  return rtn;
}

void _ReShaderDestroy(ref(ReShader) ctx)
{
  glDeleteProgram(_(ctx).id);
  release(ctx);
}

GLuint _ReShaderId(ref(ReShader) ctx)
{
  return _(ctx).id;
}

GLint _ReShaderColorLoc(ref(ReShader) ctx)
{
  return _(ctx).colorLoc;
}

GLint _ReShaderLightMapLoc(ref(ReShader) ctx)
{
  return _(ctx).lightMapLoc;
}

GLint _ReShaderTextureLoc(ref(ReShader) ctx)
{
  return _(ctx).textureLoc;
}

GLint _ReShaderModelLoc(ref(ReShader) ctx)
{
  return _(ctx).modelLoc;
}

GLint _ReShaderViewLoc(ref(ReShader) ctx)
{
  return _(ctx).viewLoc;
}

GLint _ReShaderProjectionLoc(ref(ReShader) ctx)
{
  return _(ctx).projectionLoc;
}

GLint _ReShaderNormalLoc(ref(ReShader) ctx)
{
  return _(ctx).normalLoc;
}

GLint _ReShaderEnableTexturingLoc(ref(ReShader) ctx)
{
  return _(ctx).enableTexturingLoc;
}

GLint _ReShaderEnableLightingLoc(ref(ReShader) ctx)
{
  return _(ctx).enableLightingLoc;
}
