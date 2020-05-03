#include <stent.h>

#include <GL/glew.h>

struct ReTexture
{
  vector(unsigned char) data;
  GLuint id;
  int dirty;
  int width;
  int height;
};

void ReTextureDestroy(ref(ReTexture) ctx);

void ReTextureSetPixel(ref(ReTexture) ctx, int x, int y,
  unsigned char r, unsigned char g, unsigned char b, unsigned char a);

GLuint _ReTextureId(ref(ReTexture) ctx);
