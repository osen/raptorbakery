#include "util.h"

void SplitString(ref(sstream) ss, char splitter, vector(ref(sstream)) out)
{
  size_t max = 0;
  size_t i = 0;
  ref(sstream) curr = NULL;
  char ch = 0;

  /*
   * If out array already larger than 0, use that existing string
   * after blanking it.
   */
  if(max >= vector_size(out))
  {
    curr = sstream_new();
  }
  else
  {
    curr = vector_at(out, max);
    sstream_str_cstr(curr, "");
  }

  for(i = 0; i < sstream_length(ss); i++)
  {
    ch = sstream_at(ss, i);

    if(ch == splitter)
    {
      /*
       * String is already in array if max is still smaller than array
       */
      if(max >= vector_size(out))
      {
        vector_push_back(out, curr);
      }
      max++;

      /*
       * If out array already larger than max, use that existing string
       * after blanking it.
       */
      if(max >= vector_size(out))
      {
        curr = sstream_new();
      }
      else
      {
        curr = vector_at(out, max);
        sstream_str_cstr(curr, "");
      }
    }
    else
    {
      sstream_append_char(curr, ch);
    }
  }

  /*
   * If remaining curr is not blank. Add it to out array
   * if not already reusing an element.
   */
  if(sstream_length(curr) > 0)
  {
    if(max >= vector_size(out))
    {
      vector_push_back(out, curr);
    }
    max++;
  }
  else
  {
    if(max >= vector_size(out))
    {
      sstream_delete(curr);
    }
  }

  /*
   * Erase the remaining elements. They are not needed.
   */
  while(vector_size(out) > max)
  {
    sstream_delete(vector_at(out, vector_size(out) - 1));
    vector_erase(out, vector_size(out) - 1, 1);
  }
}

void SplitStringWhitespace(ref(sstream) ss, vector(ref(sstream)) out)
{
  size_t max = 0;
  size_t i = 0;
  ref(sstream) curr = NULL;
  char ch = 0;

  /*
   * If out array already larger than 0, use that existing string
   * after blanking it.
   */
  if(max >= vector_size(out))
  {
    curr = sstream_new();
  }
  else
  {
    curr = vector_at(out, max);
    sstream_str_cstr(curr, "");
  }

  for(i = 0; i < sstream_length(ss); i++)
  {
    ch = sstream_at(ss, i);

    if(ch == ' ' || ch == '\t')
    {
      /*
       * Add to out array if not an empty token
       */
      if(sstream_length(curr) > 0)
      {
        /*
         * String is already in array if max is still smaller than array
         */
        if(max >= vector_size(out))
        {
          vector_push_back(out, curr);
        }
        max++;

        /*
         * If out array already larger than max, use that existing string
         * after blanking it.
         */
        if(max >= vector_size(out))
        {
          curr = sstream_new();
        }
        else
        {
          curr = vector_at(out, max);
          sstream_str_cstr(curr, "");
        }
      }
    }
    else
    {
      sstream_append_char(curr, ch);
    }
  }

  /*
   * If remaining curr is not blank. Add it to out array
   * if not already reusing an element.
   */
  if(sstream_length(curr) > 0)
  {
    if(max >= vector_size(out))
    {
      vector_push_back(out, curr);
    }
    max++;
  }
  else
  {
    if(max >= vector_size(out))
    {
      sstream_delete(curr);
    }
  }

  /*
   * Erase the remaining elements. They are not needed.
   */
  while(vector_size(out) > max)
  {
    sstream_delete(vector_at(out, vector_size(out) - 1));
    vector_erase(out, vector_size(out) - 1, 1);
  }
}

void SplitStringNewline(ref(sstream) ss, vector(ref(sstream)) out)
{
  size_t max = 0;
  size_t i = 0;
  ref(sstream) curr = NULL;
  char ch = 0;

  /*
   * If out array already larger than 0, use that existing string
   * after blanking it.
   */
  if(max >= vector_size(out))
  {
    curr = sstream_new();
  }
  else
  {
    curr = vector_at(out, max);
    sstream_str_cstr(curr, "");
  }

  for(i = 0; i < sstream_length(ss); i++)
  {
    ch = sstream_at(ss, i);

    if(ch == '\n')
    {
      /*
       * Add to out array if not an empty token
       */
      if(sstream_length(curr) > 0)
      {
        /*
         * String is already in array if max is still smaller than array
         */
        if(max >= vector_size(out))
        {
          vector_push_back(out, curr);
        }
        max++;

        /*
         * If out array already larger than max, use that existing string
         * after blanking it.
         */
        if(max >= vector_size(out))
        {
          curr = sstream_new();
        }
        else
        {
          curr = vector_at(out, max);
          sstream_str_cstr(curr, "");
        }
      }
    }
    else if(ch == '\r')
    {
      /* Ignore */
    }
    else
    {
      sstream_append_char(curr, ch);
    }
  }

  /*
   * If remaining curr is not blank. Add it to out array
   * if not already reusing an element.
   */
  if(sstream_length(curr) > 0)
  {
    if(max >= vector_size(out))
    {
      vector_push_back(out, curr);
    }
    max++;
  }
  else
  {
    if(max >= vector_size(out))
    {
      sstream_delete(curr);
    }
  }

  /*
   * Erase the remaining elements. They are not needed.
   */
  while(vector_size(out) > max)
  {
    sstream_delete(vector_at(out, vector_size(out) - 1));
    vector_erase(out, vector_size(out) - 1, 1);
  }
}

void vector_sstream_delete(vector(ref(sstream)) ctx)
{
  size_t i = 0;

  for(i = 0; i < vector_size(ctx); i++)
  {
    sstream_delete(vector_at(ctx, i));
  }

  vector_delete(ctx);
}