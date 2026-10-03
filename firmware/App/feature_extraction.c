/**
 * @file feature_extraction.c
 * @brief Feature extraction implementation
 * @author Chetan I K
 * @date October 2026
 */

#include "feature_extraction.h"
#include <math.h>

void extract_features(const float *buffer, int size, float *features) {
    if (buffer == 0 || features == 0 || size <= 0) return;

    float sum = 0.0f;
    float sum_sq = 0.0f;
    float min_val = buffer[0];
    float max_val = buffer[0];

    for (int i = 0; i < size; i++) {
        float x = buffer[i];
        sum += x;
        sum_sq += x * x;
        if (x < min_val) min_val = x;
        if (x > max_val) max_val = x;
    }

    float mean = sum / (float)size;
    float variance = (sum_sq / (float)size) - (mean * mean);
    float rms = sqrtf(sum_sq / (float)size);

    float sum_x = 0.0f, sum_y = 0.0f, sum_xy = 0.0f, sum_xx = 0.0f;
    for (int i = 0; i < size; i++) {
        float x = (float)i;
        float y = buffer[i];
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_xx += x * x;
    }
    float denom = ((float)size * sum_xx) - (sum_x * sum_x);
    float slope = (denom != 0.0f)
        ? (((float)size * sum_xy) - (sum_x * sum_y)) / denom
        : 0.0f;

    float rate = (size >= 2) ? (buffer[size - 1] - buffer[size - 2]) : 0.0f;
    float ewma  = compute_ewma(buffer, size, 0.1f);
    float cusum = compute_cusum(buffer, size, 0.5f, 5.0f);

    features[FEAT_MEAN]     = mean;
    features[FEAT_VARIANCE] = variance;
    features[FEAT_SLOPE]    = slope;
    features[FEAT_RMS]      = rms;
    features[FEAT_EWMA]     = ewma;
    features[FEAT_CUSUM]    = cusum;
    features[FEAT_RATE]     = rate;
    features[FEAT_RANGE]    = max_val - min_val;
}

float compute_ewma(const float *buffer, int size, float alpha) {
    if (buffer == 0 || size <= 0) return 0.0f;
    float ewma = buffer[0];
    for (int i = 1; i < size; i++) {
        ewma = alpha * buffer[i] + (1.0f - alpha) * ewma;
    }
    return ewma;
}

float compute_cusum(const float *buffer, int size, float k, float h) {
    if (buffer == 0 || size <= 0) return 0.0f;
    (void)h;
    float sum = 0.0f;
    for (int i = 0; i < size; i++) sum += buffer[i];
    float mean = sum / (float)size;
    float cusum = 0.0f;
    for (int i = 0; i < size; i++) {
        cusum = cusum + (buffer[i] - mean) - k;
        if (cusum < 0.0f) cusum = 0.0f;
    }
    return cusum;
}