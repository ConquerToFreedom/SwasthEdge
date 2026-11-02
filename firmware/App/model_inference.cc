#include "model_inference.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "model.h" // Your teammate's generated model
#include <math.h>

// 1. Tensor Arena (Defined in model.h as 2048 bytes)
static uint8_t tensor_arena[TENSOR_ARENA_SIZE_BYTES];

// 2. Pointers
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

extern "C" void setup_tinyml(void) {
    model = tflite::GetModel(g_autoencoder_model_data);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        return; // Error: Model version mismatch
    }

    // Register ops used by the Autoencoder (Dense/MatMul + Relu)
    static tflite::MicroMutableOpResolver<3> resolver;
    resolver.AddFullyConnected();
    resolver.AddRelu();

    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, TENSOR_ARENA_SIZE_BYTES);
    interpreter = &static_interpreter;

    if (interpreter->AllocateTensors() != kTfLiteOk) {
        return; // Error: Arena size too small
    }

    input = interpreter->input(0);
    output = interpreter->output(0);
}

extern "C" float run_inference(float temp, float current, float voltage) {
    if (input == nullptr) return -1.0f;

    // --- 1. NORMALIZE RAW DATA ---
    float x_norm[3];
    x_norm[0] = (temp - SENSOR_MEAN[0]) / SENSOR_STD_DEV[0];
    x_norm[1] = (current - SENSOR_MEAN[1]) / SENSOR_STD_DEV[1];
    x_norm[2] = (voltage - SENSOR_MEAN[2]) / SENSOR_STD_DEV[2];

    // --- 2. QUANTIZE TO INT8 ---
    input->data.int8[0] = (int8_t)roundf(x_norm[0] / INPUT_SCALE) + INPUT_ZERO_POINT;
    input->data.int8[1] = (int8_t)roundf(x_norm[1] / INPUT_SCALE) + INPUT_ZERO_POINT;
    input->data.int8[2] = (int8_t)roundf(x_norm[2] / INPUT_SCALE) + INPUT_ZERO_POINT;

    // --- 3. RUN TINYML INFERENCE ---
    if (interpreter->Invoke() != kTfLiteOk) {
        return -1.0f; 
    }

    // --- 4. DEQUANTIZE OUTPUT ---
    float x_hat[3];
    x_hat[0] = ((float)output->data.int8[0] - OUTPUT_ZERO_POINT) * OUTPUT_SCALE;
    x_hat[1] = ((float)output->data.int8[1] - OUTPUT_ZERO_POINT) * OUTPUT_SCALE;
    x_hat[2] = ((float)output->data.int8[2] - OUTPUT_ZERO_POINT) * OUTPUT_SCALE;

    // --- 5. CALCULATE MEAN SQUARED ERROR (MSE) ---
    float mse = 0.0f;
    mse += powf(x_hat[0] - x_norm[0], 2);
    mse += powf(x_hat[1] - x_norm[1], 2);
    mse += powf(x_hat[2] - x_norm[2], 2);
    mse = mse / 3.0f;

    return mse;
}