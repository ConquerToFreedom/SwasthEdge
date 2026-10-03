/**
 * @file health_fusion.h
 * @brief Multi-layer health fusion
 * @author Chetan I K
 * @date October 2026
 */

#ifndef HEALTH_FUSION_H
#define HEALTH_FUSION_H

#define WEIGHT_SENSOR      0.30f
#define WEIGHT_ELECTRICAL  0.20f
#define WEIGHT_CONTROLLER  0.15f
#define WEIGHT_MEMORY      0.15f
#define WEIGHT_TINYML      0.20f

float health_fusion(float sensor, float electrical, float controller,
                    float memory, float tinyml);
float compute_memory_health_score(void);
float compute_controller_health_score(void);

#endif /* HEALTH_FUSION_H */