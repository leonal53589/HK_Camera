#include "Camera.h"
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

int main() {
    Camera m_camera;
    cv::Mat image;
    int saved_image_count = 0;
    int ret = 0; // 返回值
    do {
        // 打开相机
        if(m_camera.OpenDeviceIndex(0) != 0) {
            std::cerr << "打开相机失败!" << std::endl;
            ret = -1;
            break;
        }
        // 获取曝光时间和增益
        float currExposureTime, currGain;
        if(m_camera.getValue("ExposureTime", currExposureTime) != 0) {
            std::cerr << "获取曝光时间失败!" << std::endl;
            ret = -1;
            break;
        }
        if(m_camera.getValue("Gain", currGain) != 0) {
            std::cerr << "获取增益失败!" << std::endl;
            ret = -1;
            break;
        }
        // 初始化滑动条
        std::string windowName = "捕获图像>..<";
        m_camera.initTrackbar(windowName, currExposureTime, currGain);
        // 开始采集
        if(m_camera.startGrabbing() != 0) {
            std::cerr << "开始采集失败!" << std::endl;
            ret = -1;
            break;
        }
        while(true) {
            image = m_camera.captureFrame();
            if(image.empty()) {
                std::cerr << "无法采集图像!" << std::endl;
                ret = -1;
                break;
            }
            cv::imshow(windowName, image);
            int key = cv::waitKey(1);
            if(key == 27) { // ESC键退出
                std::cout << "正在退出程序..." << std::endl;
                break;
            }
            if(!image.empty() && (key == 's' || key == 'S')) {  // 按s保存
                std::string filename = "../image/captured_image_" + std::to_string(saved_image_count++) + ".png";
                if(cv::imwrite(filename, image)) {
                    std::cout << "已保存图像到 " << filename << std::endl;
                } else {
                    std::cerr << "保存图像失败: " << filename << std::endl;
                }
            }
        }
        
    } while(0);
    cv::destroyAllWindows();
    m_camera.stopGrabbing();
    return ret;
}