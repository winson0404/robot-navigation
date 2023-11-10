#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>

int main() {
    // Load the ONNX model
    const std::string modelPath = "your_model.onnx";
    cv::dnn::Net net = cv::dnn::readNet(modelPath);

    if (net.empty()) {
        std::cerr << "Could not load the model: " << modelPath << std::endl;
        return -1;
    }

    // Open a video capture object (default camera or a specific camera can be specified)
    cv::VideoCapture cap(0); // Use 0 for the default camera (you might need to change this)

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the camera." << std::endl;
        return -1;
    }

    // Main loop for real-time classification
    while (true) {
        cv::Mat frame;
        cap >> frame; // Capture a frame from the camera

        if (frame.empty()) {
            std::cerr << "End of video stream." << std::endl;
            break;
        }

        // Prepare the input blob for the model
        cv::Mat inputBlob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(224, 224), cv::Scalar(104, 117, 123), false, false);

        // Set the input for the network
        net.setInput(inputBlob);

        // Perform inference
        cv::Mat output = net.forward();

        // Process the output to get classification results
        cv::Mat probMat = output.reshape(1, 1);

        // Find the class with the highest probability
        cv::Point classIdPoint;
        double confidence;
        cv::minMaxLoc(probMat, nullptr, &confidence, nullptr, &classIdPoint);
        int classId = classIdPoint.x;

        // Display the classification result
        std::string className = "Unknown";
        if (classId >= 0 && classId < num_classes) {
            className = class_names[classId];
        }

        std::string resultText = "Class: " + className + ", Confidence: " + std::to_string(confidence);
        cv::putText(frame, resultText, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 2);

        // Display the frame with the classification result
        cv::imshow("Real-time Classification", frame);

        // Exit on ESC key press
        if (cv::waitKey(1) == 27)
            break;
    }

    // Release the camera and close OpenCV windows
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
