#include <stdlib.h>
#include "c_speech_features.h"
#include "c_speech_features_private.h"

#define CSF_HZ2MEL(x) (2595.0 * csf_log10(1.0+(x)/700.0))
#define CSF_MEL2HZ(x) (700.0 * (csf_pow(10.0, (x)/2595.0) - 1.0))

csf_float*
csf_get_filterbanks(int aNFilters, int aNFFT, int aSampleRate,
                    int aLowFreq, int aHighFreq)
{
  int i, j, idx;
  int feat_width = aNFFT / 2 + 1;
  csf_float lowmel = CSF_HZ2MEL(aLowFreq);
  csf_float highmel = CSF_HZ2MEL((aHighFreq <= aLowFreq) ?
                             aSampleRate / 2 : aHighFreq);
  int* bin = (int*)malloc(sizeof(int) * (aNFilters + 2));
  csf_float* fbank =
    (csf_float*)calloc(sizeof(csf_float), aNFilters * feat_width);

  for (i = 0; i < aNFilters + 2; i++) {
    csf_float melpoint = ((highmel - lowmel) /
                          (csf_float)(aNFilters + 1) * i) + lowmel;
    bin[i] = (int)csf_floor((aNFFT + 1) *
                            CSF_MEL2HZ(melpoint) / (csf_float)aSampleRate);
  }

  for (i = 0, idx = 0; i < aNFilters; i++, idx += feat_width) {
    int start = MIN(bin[i], bin[i+1]);
    int end = MAX(bin[i], bin[i+1]);
    for (j = start; j < end; j++) {
      fbank[idx + j] = (j - bin[i]) / (csf_float)(bin[i+1]-bin[i]);
    }
    start = MIN(bin[i+1], bin[i+2]);
    end = MAX(bin[i+1], bin[i+2]);
    for (j = start; j < end; j++) {
      fbank[idx + j] = (bin[i+2]-j) / (csf_float)(bin[i+2]-bin[i+1]);
    }
  }
  free(bin);

  return fbank;
}