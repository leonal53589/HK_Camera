#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cstring>
#include <string>
#include <MvCameraControl.h>

class Camera
{
public:
    // 构造函数和析构函数
    Camera();
    ~Camera();
    // 打开相应索引的相机
    int OpenDeviceIndex(int DeviceIndex);
    // 打印设备信息
    void printDeviceInformation();
    // 节点类型查询
    int getNodeType(const std::string& nodeName, MV_XML_InterfaceType& nodeType);
    // 暴露出来的曝光和增益接口
    int setExposureTime(float exposureTime);
    int getExposureTime(float& exposureTime);
    int setGain(float gain);
    int getGain(float& gain);
    // 模板参数设置和获取
    template<typename T> int setValue(const std::string& nodeName, T value);
    template<typename T> int getValue(const std::string& nodeName, T& value);
    // 图像采集
    int startGrabbing();
    int stopGrabbing();
    // 拉取每一帧进行处理
    cv::Mat captureFrame();
    // 滑动条功能
    void initTrackbar(const std::string windowName, float& currExposureTime, float& currGain);

private:
    // SDK初始化和反初始化
    static bool initSDK();
    static bool FinalSDK();
    // 图像处理
    cv::Mat convertToMat(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo);
    cv::Mat convertUsingSDK(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo);
    // 设备启动和停止
    int initCamera();
    int destoryCamera();
    static void getExposureTime(int pos, void *pCamera);
    static void getGain(int pos, void* pCamera);

private:
    int m_nRet = MV_OK;
    MV_CC_DEVICE_INFO_LIST m_DeviceList;
    void *m_handle = nullptr;
    static int m_cameraCount;
    static bool m_isSDKInit;
};

#include "Camera.tpp"