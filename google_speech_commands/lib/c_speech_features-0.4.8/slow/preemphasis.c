#include <stdlib.h>
#include "c_speech_features.h"

csf_float*
csf_preemphasis(const short* aSignal, unsigned int aSignalLen, csf_float aCoeff)
{
  int i;
  csf_float* preemph = (csf_float*)malloc(sizeof(csf_float) * aSignalLen);

  for (i = aSignalLen - 1; i >= 1; i--) {
    preemph[i] = aSignal[i] - aSignal[i-1] * aCoeff;
  }
  preemph[0] = (csf_float)aSignal[0];

  return preemph;
}