#include <re/re.h>

#include <stent.h>

struct MaterialGroup
{
  ref(ReTexture) texture;
  ref(ReMesh) mesh;
};
