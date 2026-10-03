/**
 * @file health_fusion.c
 * @brief Multi-layer health fusion implementation
 * @author Chetan I K
 * @date October 2026
 */

#include "health_fusion.h"

float health_fusion(float sensor, float electrical, float controller,
                    float memory, float tinyml) {
    float fused = WEIGHT_SENSOR     * sensor +
                  WEIGHT_ELECTRICAL * electrical +
                  WEIGHT_CONTROLLER * controller +
                  WEIGHT_MEMORY     * memory +
                  WEIGHT_TINYML     * tinyml;

    if (fused < 0.0f)   fused = 0.0f;
    if (fused > 100.0f) fused = 100.0f;

    return fused;
}

float compute_memory_health_score(void) {
    return 100.0f;
}

float compute_controller_health_score(void) {
    return 100.0f;
}