#include <stdlib.h>
#include "c_speech_features.h"

void
csf_lifter(csf_float* aCepstra, int aNFrames, int aNCep, int aCepLifter)
{
  int i, j, idx;

  csf_float lifter = aCepLifter / 2.0;
  csf_float* factors = malloc(sizeof(csf_float) * aNCep);
  for (i = 0; i < aNCep; i++) {
    factors[i] = 1 + lifter * csf_sin(M_PI * i / (csf_float)aCepLifter);
  }

  for (i = 0, idx = 0; i < aNFrames; i++) {
    for (j = 0; j < aNCep; j++, idx++) {
      aCepstra[idx] *= factors[j];
    }
  }

  free(factors);
}