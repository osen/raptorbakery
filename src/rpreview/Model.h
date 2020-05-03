#include <re/re.h>

#include <stent.h>

struct MaterialGroup;

struct Model
{
  ref(ReContext) context;
  vector(ref(MaterialGroup)) groups;
};

ref(Model) ModelLoad(ref(ReContext) context, char *path);
void ModelDestroy(ref(Model) ctx);

ref(ReMesh) ModelMesh(ref(Model) ctx);

ref(ReTexture) TextureLoad(ref(ReContext) context, char *path);
