#ifndef MODEL_INFERENCE_H
#define MODEL_INFERENCE_H

#ifdef __cplusplus
extern "C" {
#endif

// Call this once at startup
void setup_tinyml(void);

// Call this in your while(1) loop. Returns the MSE (Mean Squared Error).
// If the returned value is > 0.25, the machine is in a FAULT state.
float run_inference(float temp, float current, float voltage);

#ifdef __cplusplus
}
#endif

#endif // MODEL_INFERENCE_H