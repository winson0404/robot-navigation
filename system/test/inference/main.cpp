#include <iostream>
#include <vector>
#include <onnxruntime/core/providers/cpu/cpu_execution_provider.h>
#include <onnxruntime/core/providers/providers.h>
#include <onnxruntime/core/session/onnxruntime_cxx_api.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>

// Define target_dict as a global constant
const std::unordered_map<int, std::string> target_dict = {
    {0, "left"},
    {1, "front"},
    {2, "right"},
    {3, "back"}};

int main() {
    const std::string model_path = "output/Table_CustomNet/tf_convert_test3/best_model.onnx";
    const float threshold = 0.5;

    // Initialize ONNX Runtime
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "ONNXRuntime");
    Ort::SessionOptions session_options;
    OrtStatus* status;
    OrtSession* session;

    status = OrtCreateSession(env, model_path.c_str(), session_options, &session);
    if (status != nullptr) {
        OrtGetErrorMessage(status, ORT_GET_LAST_STATUS, nullptr, 0);
        std::cerr << "Error initializing ONNX Runtime session." << std::endl;
        OrtReleaseStatus(status);
        return 1;
    }

    // Open the camera
    cv::VideoCapture cap("/dev/video0");
    if (!cap.isOpened()) {
        std::cerr << "Error opening video capture device." << std::endl;
        return 1;
    }

    while (true) {
        cv::Mat frame;
        cap.read(frame);

        if (frame.empty()) {
            std::cerr << "Error reading frame." << std::endl;
            break;
        }

        // TODO: Perform preprocessing on 'frame' if needed

        // Convert the frame to a format suitable for inference
        // TODO: Implement the conversion logic

        // Perform inference
        std::vector<const char*> input_names = {"images"};
        std::vector<Ort::Value> input_tensors;
        // TODO: Populate input_tensors with the converted frame

        std::vector<const char*> output_names;
        std::vector<Ort::Value> output_tensors;
        status = OrtRun(session, nullptr, input_names.data(), input_tensors.data(), input_names.size(), output_names.data(), output_names.size(), output_tensors.data());
        if (status != nullptr) {
            OrtGetErrorMessage(status, ORT_GET_LAST_STATUS, nullptr, 0);
            std::cerr << "Error running inference." << std::endl;
            OrtReleaseStatus(status);
            break;
        }

        // Post-processing and visualization
        // TODO: Implement post-processing and visualization based on 'output_tensors'

        // Send data via TCP client
        // TODO: Implement TCP client to send data

        // Sleep for 2 seconds
        // usleep(2000000);

        // Break the loop if 'q' is pressed
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // Release resources
    OrtReleaseSession(session);
    OrtReleaseEnv(env);

    return 0;
}
