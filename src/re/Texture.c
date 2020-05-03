#include "Texture.h"
#include "internal.h"

void ReTextureDestroy(ref(ReTexture) ctx)
{
  glDeleteTextures(1, &_(ctx).id);
  _RePollForError();
  vector_delete(_(ctx).data);
  release(ctx);
}

void ReTextureSetPixel(ref(ReTexture) ctx, int x, int y,
  unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
  size_t i = (y * _(ctx).width * 4) + (x * 4);

  vector_at(_(ctx).data, i) = r;
  vector_at(_(ctx).data, i + 1) = g;
  vector_at(_(ctx).data, i + 2) = b;
  vector_at(_(ctx).data, i + 3) = a;

  _(ctx).dirty = 1;
}

GLuint _ReTextureId(ref(ReTexture) ctx)
{
  if(_(ctx).dirty)
  {
    glBindTexture(GL_TEXTURE_2D, _(ctx).id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _(ctx).width, _(ctx).height, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, &vector_at(_(ctx).data, 0));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    _(ctx).dirty = 0;
  }

  return _(ctx).id;
}
