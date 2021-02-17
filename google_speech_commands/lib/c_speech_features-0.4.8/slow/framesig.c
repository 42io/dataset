#include <stdlib.h>
#include "c_speech_features.h"
#include "c_speech_features_private.h"

int
csf_framesig(const csf_float* aSignal, unsigned int aSignalLen, int aFrameLen,
             int aPaddedFrameLen, int aFrameStep, csf_float* aWinFunc,
             csf_float** aFrames)
{
  int* indices;
  csf_float* frames;
  int i, j, idx, iidx, n_frames;
  int frame_width = MAX(aPaddedFrameLen, aFrameLen);

  if (aSignalLen > aFrameLen) {
    n_frames = 1 + (int)csf_ceil((aSignalLen - aFrameLen) /
                                 (csf_float)aFrameStep);
  } else {
    n_frames = 1;
  }

  indices = (int*)malloc(sizeof(int) * n_frames * aFrameLen);
  for (i = 0, idx = 0; i < n_frames; i++) {
    int base = i * aFrameStep;
    for (j = 0; j < aFrameLen; j++, idx++) {
      indices[idx] = base + j;
    }
  }

  frames = (csf_float*)malloc(sizeof(csf_float) * n_frames * frame_width);
  for (i = 0, idx = 0, iidx = 0; i < n_frames; i++) {
    for (j = 0; j < aFrameLen; j++, idx++, iidx++) {
      int index = indices[iidx];
      frames[idx] = index < aSignalLen ? aSignal[index] : 0.0;
      if (aWinFunc) {
        frames[idx] *= aWinFunc[j];
      }
    }
    for (j = aFrameLen; j < aPaddedFrameLen; j++, idx++) {
      frames[idx] = 0.0;
    }
  }
  free(indices);

  *aFrames = frames;
  return n_frames;
}