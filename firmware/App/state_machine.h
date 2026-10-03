/**
 * @file state_machine.h
 * @brief SwasthEdge state machine — Normal, Warning, Degraded, Critical
 * @author Chetan I K
 * @date October 2026
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

typedef enum {
    STATE_NORMAL = 0,
    STATE_WARNING,
    STATE_DEGRADED,
    STATE_CRITICAL
} SwasthEdgeState;

#define CONFIDENCE_NORMAL_MIN        75.0f
#define CONFIDENCE_WARNING_MIN       50.0f
#define CONFIDENCE_DEGRADED_MIN      25.0f
#define CONFIDENCE_RECOVERY_NORMAL   85.0f
#define CONFIDENCE_RECOVERY_WARNING  60.0f
#define CONFIDENCE_RECOVERY_DEGRADED 35.0f

#define HYSTERESIS_BAND              10.0f
#define CONSECUTIVE_READINGS         3

void state_machine_init(void);
void state_machine_update(float confidence);
SwasthEdgeState state_machine_get_state(void);
const char* state_machine_get_name(void);
const char* state_machine_get_name_by_state(SwasthEdgeState state);

#endif /* STATE_MACHINE_H */