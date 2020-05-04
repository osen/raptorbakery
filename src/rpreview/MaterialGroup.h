#include <re/re.h>

#include <stent.h>

struct MaterialGroup
{
  ref(ReTexture) texture;
  ref(ReTexture) lightMap;
  ref(ReMesh) mesh;
  ref(sstream) name;
};
