#include "Buffer.h"
#include "internal.h"
#include "mathutil.h"

void ReBufferDestroy(ref(ReBuffer) ctx)
{
  glDeleteBuffers(1, &_(ctx).id);
  _RePollForError();
  vector_delete(_(ctx).data);
  release(ctx);
}

void ReBufferAddVec2(ref(ReBuffer) ctx, struct ReVec2 val)
{
  if(_(ctx).type != 0 && _(ctx).type != 2)
  {
    panic("Attempt to add mixed types to buffer");
  }

  _(ctx).dirty = 1;
  _(ctx).type = 2;
  vector_push_back(_(ctx).data, val.x);
  vector_push_back(_(ctx).data, val.y);
}

void ReBufferAddVec3(ref(ReBuffer) ctx, struct ReVec3 val)
{
  if(_(ctx).type != 0 && _(ctx).type != 3)
  {
    panic("Attempt to add mixed types to buffer");
  }

  _(ctx).dirty = 1;
  _(ctx).type = 3;
  vector_push_back(_(ctx).data, val.x);
  vector_push_back(_(ctx).data, val.y);
  vector_push_back(_(ctx).data, val.z);
}


size_t ReBufferSize(ref(ReBuffer) ctx)
{
  if(!_(ctx).type)
  {
    panic("Buffer is empty");
  }

  return vector_size(_(ctx).data) / _(ctx).type;
}

GLuint _ReBufferId(ref(ReBuffer) ctx)
{
  if(_(ctx).dirty)
  {
    glBindBuffer(GL_ARRAY_BUFFER, _(ctx).id);
    _RePollForError();

    if(_(ctx).type == 1 || _(ctx).type == 2 || _(ctx).type == 3 || _(ctx).type == 4)
    {
      glBufferData(GL_ARRAY_BUFFER, sizeof(vector_at(_(ctx).data, 0)) * vector_size(_(ctx).data),
        &vector_at(_(ctx).data, 0), GL_STATIC_DRAW);

      _RePollForError();
    }
    else
    {
      panic("TODO: Support other types of data");
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    _RePollForError();

    _(ctx).dirty = 0;
  }

  return _(ctx).id;
}

int _ReBufferType(ref(ReBuffer) ctx)
{
  return _(ctx).type;
}
