#include "stent.h"

void SplitStringWhitespace(ref(sstream) ss, vector(ref(sstream)) out);
void SplitStringNewline(ref(sstream) ss, vector(ref(sstream)) out);
void SplitString(ref(sstream) ss, char splitter, vector(ref(sstream)) out);
void vector_sstream_delete(vector(ref(sstream)) out);