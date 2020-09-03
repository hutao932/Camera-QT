#ifndef __CAMERA_OPENCV__
#define __CAMERA_OPENCV__

#include<QImage>

#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

class OpenCV_Camera {

public:
    ~OpenCV_Camera();

    static OpenCV_Camera& get_instance() {
        static OpenCV_Camera camera_inst;
        return camera_inst;
    }
    int start(int index);
    QImage camera_get_frame();

    void stop();
    int get_camera_res_width() {
        return m_res_width;
    }
    int get_camera_res_height() {
        return m_res_height;
    }
    QImage MatToQImage(cv::Mat mtx);

    // UVC get and set
    double get_brightness();
    void set_brightness();

private:
    OpenCV_Camera();
    cv::VideoCapture m_capture;
    int m_res_width = 1280;
    int m_res_height = 720;
    int m_framerate;
    int m_camera_index = 1;
};

#endif
