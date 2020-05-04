#include "mathutil.h"

struct ReVertex
{
  struct ReVec3 position;
  struct ReVec2 texCoord;
  struct ReVec3 normal;
  struct ReVec2 lmCoord;
};

struct ReFace
{
  struct ReVertex a;
  struct ReVertex b;
  struct ReVertex c;
};
