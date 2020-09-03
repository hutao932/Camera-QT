
#include "stdio.h"
#include "camera.h"

OpenCV_Camera::OpenCV_Camera(){
}

OpenCV_Camera::~OpenCV_Camera() {

}
int OpenCV_Camera::start(int index) {

    m_camera_index = index;
    m_capture = cv::VideoCapture(m_camera_index);

    if (!m_capture.isOpened()) {
            std::cout << "Failed to open camera！" << std::endl;
            return -1;
        } else {
        std::cout << "Opened UVC camera, index: " << index << std::endl;
    }

    m_capture.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
    m_capture.set(CV_CAP_PROP_FPS, 30);
    m_capture.set(CV_CAP_PROP_FRAME_WIDTH, m_res_width);
    m_capture.set(CV_CAP_PROP_FRAME_HEIGHT, m_res_height);

    return 0;
}

void OpenCV_Camera::stop() {
    m_capture.release();
}

QImage OpenCV_Camera::camera_get_frame() {

    if (!m_capture.isOpened()) {
            std::cout << "Camera has not been opened yet" << std::endl;
        }

    cv::Mat frame;
    m_capture.read(frame);

    if (frame.empty()) {
        std::cout << "empty frame" << std::endl;
    }

    QImage Qimageframe = MatToQImage(frame);
    return Qimageframe;
}

QImage OpenCV_Camera::MatToQImage(cv::Mat mtx) {
    switch (mtx.type())
    {
    case CV_8UC1:
        {
            QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.cols, QImage::Format_Grayscale8);
            return img;
        }
        break;
    case CV_8UC3:
        {
            QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.cols * 3, QImage::Format_RGB888);
            return img.rgbSwapped();
        }
        break;
    case CV_8UC4:
        {
            QImage img((const unsigned char *)(mtx.data), mtx.cols, mtx.rows, mtx.cols * 4, QImage::Format_ARGB32);
            return img;
        }
        break;
    default:
        {
            QImage img;
            return img;
        }
        break;
    }
}

double OpenCV_Camera::get_brightness() {
    return m_capture.get(CV_CAP_PROP_BRIGHTNESS);
}

// Set with default 0.5
void OpenCV_Camera::set_brightness() {

    m_capture.set(CV_CAP_PROP_BRIGHTNESS, 0.5);
}
