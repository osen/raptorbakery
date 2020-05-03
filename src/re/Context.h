#include "stent.h"

struct ReContext;
struct ReRenderer;
struct ReMesh;
struct ReBuffer;
struct ReTexture;

ref(ReContext) ReContextCreate();
void ReContextDestroy(ref(ReContext) ctx);

ref(ReRenderer) ReContextCreateRenderer(ref(ReContext) ctx);
ref(ReBuffer) ReContextCreateBuffer(ref(ReContext) ctx);
ref(ReMesh) ReContextCreateMesh(ref(ReContext) ctx);
ref(ReMesh) ReContextCreatePrimitive(ref(ReContext) ctx, int type);
ref(ReTexture) ReContextCreateTexture(ref(ReContext) ctx, int width, int height);
