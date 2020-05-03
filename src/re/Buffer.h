#include "stent.h"

struct ReBuffer;
struct ReVec2;
struct ReVec3;

size_t ReBufferSize(ref(ReBuffer) ctx);
void ReBufferDestroy(ref(ReBuffer) ctx);
void ReBufferAddVec2(ref(ReBuffer) ctx, struct ReVec2 val);
void ReBufferAddVec3(ref(ReBuffer) ctx, struct ReVec3 val);
