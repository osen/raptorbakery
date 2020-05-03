#include "internal.h"

void _RePollForError()
{
  GLenum err = 0;

  err = glGetError();

  if(err == GL_NO_ERROR)
  {
    return;
  }

  panic("OpenGL emitted an error");
  //panic((char*)gluErrorString(err));
}
