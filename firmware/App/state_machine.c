/**
 * @file state_machine.c
 * @brief SwasthEdge state machine implementation
 * @author Chetan I K
 * @date October 2026
 */

#include "state_machine.h"

static SwasthEdgeState current_state;
static uint8_t consecutive_count;

static SwasthEdgeState determine_target_state(float confidence);

void state_machine_init(void) {
    current_state = STATE_NORMAL;
    consecutive_count = 0;
}

void state_machine_update(float confidence) {
    SwasthEdgeState target = determine_target_state(confidence);

    if (target != current_state) {
        consecutive_count++;
        if (consecutive_count >= CONSECUTIVE_READINGS) {
            current_state = target;
            consecutive_count = 0;
        }
    } else {
        consecutive_count = 0;
    }
}

static SwasthEdgeState determine_target_state(float confidence) {
    if (current_state == STATE_NORMAL) {
        if (confidence < CONFIDENCE_NORMAL_MIN) return STATE_WARNING;
    } else if (current_state == STATE_WARNING) {
        if (confidence < CONFIDENCE_WARNING_MIN) return STATE_DEGRADED;
        else if (confidence > CONFIDENCE_RECOVERY_NORMAL) return STATE_NORMAL;
    } else if (current_state == STATE_DEGRADED) {
        if (confidence < CONFIDENCE_DEGRADED_MIN) return STATE_CRITICAL;
        else if (confidence > CONFIDENCE_RECOVERY_WARNING) return STATE_WARNING;
    } else if (current_state == STATE_CRITICAL) {
        if (confidence > CONFIDENCE_RECOVERY_DEGRADED) return STATE_DEGRADED;
    }
    return current_state;
}

SwasthEdgeState state_machine_get_state(void) {
    return current_state;
}

const char* state_machine_get_name(void) {
    return state_machine_get_name_by_state(current_state);
}

const char* state_machine_get_name_by_state(SwasthEdgeState state) {
    switch (state) {
        case STATE_NORMAL:   return "NORMAL";
        case STATE_WARNING:  return "WARNING";
        case STATE_DEGRADED: return "DEGRADED";
        case STATE_CRITICAL: return "CRITICAL";
        default:             return "UNKNOWN";
    }
}