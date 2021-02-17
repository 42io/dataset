#include "c_speech_features.h"

csf_float*
csf_powspec(const csf_float* aFrames, int aNFrames, int aNFFT)
{
  int i;
  const int fft_out = aNFFT / 2 + 1;
  csf_float* pspec = csf_magspec(aFrames, aNFrames, aNFFT);

  // Compute the power spectrum
  for (i = 0; i < aNFrames * fft_out; i++) {
    pspec[i] = (1.0/aNFFT) * powf(pspec[i], 2.0);
  }

  return pspec;
}