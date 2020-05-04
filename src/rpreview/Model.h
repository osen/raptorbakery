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

void ModelRender(ref(Model) ctx, ref(ReRenderer) renderer);

ref(ReTexture) TextureLoad(ref(ReContext) context, char *path);
