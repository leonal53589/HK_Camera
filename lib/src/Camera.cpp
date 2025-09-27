#include "Camera.h"
#include <cstring>
#include <iomanip>


// 计数器
int Camera::m_cameraCount = 0;
// 判断SDK是否初始化
bool Camera::m_isSDKInit = false;

Camera::Camera() {
    initSDK();
    initCamera();
}

Camera::~Camera() {
    destoryCamera();
    FinalSDK();
}

bool Camera::initSDK() {
    m_cameraCount++;
    if (!m_isSDKInit) {
        m_isSDKInit = true;
        int nRet = MV_CC_Initialize();
        if (nRet == MV_OK) {
            std::cout << "SDK初始化成功!" << std::endl;
            return true;
        } else {
            std::cerr << "SDK初始化失败! 错误码=" << nRet << std::endl;
            return false;
        }
    }
    return true;
}

bool Camera::FinalSDK() {
    m_cameraCount--;
    if (!m_cameraCount && m_isSDKInit) {
        int nRet = MV_OK;
        m_isSDKInit = false;
        if ((nRet = MV_CC_Finalize()) == MV_OK) {
            std::cout << "SDK释放成功!" << std::endl;
            return true;
        } else {
            std::cerr << "SDK释放失败! 错误码=" << nRet << std::endl;
            return false;
        }
    }
    return true;
}

int Camera::initCamera() {
    std::memset(&m_DeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    m_nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_DeviceList);
    if (m_nRet != MV_OK) {
        std::cerr << "枚举设备失败! 错误码=" << m_nRet << std::endl;
        return m_nRet;
    }
    if (!m_DeviceList.nDeviceNum) {
        std::cerr << "未找到任何设备!" << std::endl;
        return m_nRet;
    }
    std::cout << "枚举设备成功!" << std::endl;
    std::cout << "设备信息如下————" << std::endl;
    printDeviceInformation();
    return 0;
}

int Camera::OpenDeviceIndex(int cameraDeviceIndex) {
    // 索引大于枚举到的相机设备总量
    if (m_DeviceList.nDeviceNum < cameraDeviceIndex) {
        std::cerr << "设备索引错误!" << std::endl;
        return -1;
    }
    m_nRet = MV_CC_CreateHandle(&m_handle, m_DeviceList.pDeviceInfo[cameraDeviceIndex]);
    if (m_nRet != MV_OK) {
        std::cerr << "创建句柄失败! 错误码=" << m_nRet << std::endl;
        return m_nRet;
    }
    m_nRet = MV_CC_OpenDevice(m_handle);
    if (m_nRet != MV_OK) {
        std::cerr << "无法打开相机设备! 相机索引=" << cameraDeviceIndex << " 错误码=" << m_nRet << std::endl;
        MV_CC_DestroyHandle(m_handle);
        m_handle = nullptr;
        return m_nRet;
    }
    std::cout << "打开相机设备_" << cameraDeviceIndex << " 成功!" << std::endl;
    return 0;
}

int Camera::destoryCamera() {
    if (!m_handle) return m_nRet;
    m_nRet = MV_CC_CloseDevice(m_handle);
    if (m_nRet != MV_OK) {
        std::cerr << "关闭相机设备失败! 错误码=" << m_nRet << std::endl;
        return m_nRet;
    }
    std::cout << "关闭相机设备成功!" << std::endl;
    m_nRet = MV_CC_DestroyHandle(m_handle);
    if (m_nRet != MV_OK) {
        std::cerr << "销毁句柄失败! 错误码=" << m_nRet << std::endl;
        return m_nRet;
    }
    std::cout << "销毁句柄成功!" << std::endl;
    return m_nRet;
}

void Camera::printDeviceInformation() {
    // 指向设备的指针是否为空
    if (m_DeviceList.pDeviceInfo == nullptr) {
        std::cerr << "错误: 未找到设备!" << std::endl;
        return;
    }
    unsigned int nDevices = m_DeviceList.nDeviceNum;
    for (unsigned int i = 0; i < nDevices; i++) {
        if (!m_DeviceList.pDeviceInfo[i]) continue;
        std::cout << "设备索引:" << i << std::endl;
        switch (m_DeviceList.pDeviceInfo[i]->nTLayerType) {
        case MV_GIGE_DEVICE:
            std::cout << "设备类型: GigE" << std::endl;
            std::cout << "设备型号: " << m_DeviceList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chModelName << std::endl;
            std::cout << "设备版本: " << m_DeviceList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chDeviceVersion << std::endl;
            break;
        case MV_USB_DEVICE:
            std::cout << "设备类型: USB" << std::endl;
            std::cout << "设备型号: " << m_DeviceList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chModelName << std::endl;
            std::cout << "设备版本: " << m_DeviceList.pDeviceInfo[i]->SpecialInfo.stUsb3VInfo.chDeviceVersion << std::endl;
            break;
        default:
            std::cout << "设备类型: " << m_DeviceList.pDeviceInfo[i]->nTLayerType << std::endl;
            std::cout << "设备型号: 未知" << std::endl;
            std::cout << "设备版本: 未知" << std::endl;
            break;
        }
        std::cout << std::endl;
    }
}

// 查询节点信息
int Camera::getNodeType(const std::string& nodeName, MV_XML_InterfaceType& nodeType) {
    if(m_handle == nullptr) return -1;
    MV_XML_InterfaceType type;
    m_nRet = MV_XML_GetNodeInterfaceType(m_handle, nodeName.c_str(), &type);
    if (m_nRet != MV_OK) {
        std::cerr << "获取节点类型 " << nodeName << " 失败! 错误码=" << m_nRet << std::endl;
        return m_nRet;
    }
    nodeType = (MV_XML_InterfaceType)type;
    return m_nRet;
}

// 开采
int Camera::startGrabbing() {
    // 设置节点
    m_nRet = MV_CC_SetImageNodeNum(m_handle, 5);
    if (MV_OK != m_nRet) {
        std::cerr << "设置图像节点失败! 错误码=" << m_nRet << std::endl;
        return m_nRet;
    }
    // 开采
    m_nRet = MV_CC_StartGrabbing(m_handle);
    if (MV_OK != m_nRet) {
        std::cerr << "开始采集失败! 错误码=" << m_nRet << std::endl;
        return m_nRet;
    }
    std::cout << "开始采集图像..." << std::endl;
    return m_nRet;
}

// 停采
int Camera::stopGrabbing() {
    m_nRet = MV_CC_StopGrabbing(m_handle);
    if (m_nRet != MV_OK) {
        std::cerr << "停止采集失败! 错误码=" << m_nRet << std::endl;
        return m_nRet;
    }
    std::cout << "停止采集成功!" << std::endl;
    return m_nRet;
}

// 主动拉取每一帧来处理
cv::Mat Camera::captureFrame() {
    cv::Mat frame;
    // 图像信息
    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    unsigned char *pData = nullptr;
    // 图像载荷
    MVCC_INTVALUE_EX stPayloadSize = {0};
    // 获取载荷
    m_nRet = MV_CC_GetIntValueEx(m_handle, "PayloadSize", &stPayloadSize);
    if (MV_OK != m_nRet) {
        std::cerr << "获取载荷大小失败! 错误码=" << m_nRet << std::endl;
        return frame;
    }
    unsigned int nPayloadSize = stPayloadSize.nCurValue;
    // 分配内存
    pData = new unsigned char[nPayloadSize];
    if (pData == nullptr) {
        std::cerr << "无法为图像数据分配内存." << std::endl;
        return frame;
    }
    // 获取图像数据
    m_nRet = MV_CC_GetOneFrameTimeout(m_handle, pData, nPayloadSize, &stImageInfo, 5000);
    // 转换为Mat
    if (MV_OK == m_nRet) {
        frame = convertToMat(pData, &stImageInfo);
    } else {
        std::cerr << "获取图像失败! 错误码=" << m_nRet << std::endl;
    }
    delete[] pData;
    return frame;
}

// 转换函数通常来说可以防止相机直接得到的图片不能被imshow识别
cv::Mat Camera::convertToMat(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo) {
    cv::Mat image;
    // MV_CC_IMAGE结构体的像素格式（每次看每次忘）
    switch (pFrameInfo->enPixelType) {
        // 按各几种像素格式转换
        case PixelType_Gvsp_Mono8: {
            image = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
            break;
            }
        case PixelType_Gvsp_RGB8_Packed: {
            cv::Mat temp(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3, pData);
            cv::cvtColor(temp, image, cv::COLOR_RGB2BGR);
            break;
            }
        case PixelType_Gvsp_BayerRG8:
        case PixelType_Gvsp_BayerGB8:
        case PixelType_Gvsp_BayerGR8:
        case PixelType_Gvsp_BayerBG8: {
            cv::Mat bayerImg(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
            cv::cvtColor(bayerImg, image, cv::COLOR_BayerBG2BGR);
            break;
            }
        default: {
            image = convertUsingSDK(pData, pFrameInfo);
            break;
            }
        }
    // 返回深拷贝
    return image.clone();
}

// 使用SDK自带的转换函数进行转换
cv::Mat Camera::convertUsingSDK(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo) {
    // 创建一个像素转换参数结构体
    MV_CC_PIXEL_CONVERT_PARAM stConvertParam = {0};
    // 宽度 × 高度 × 3通道
    unsigned int nDataSize = pFrameInfo->nWidth * pFrameInfo->nHeight * 3;
    // 分配内存（可以智能指针，但是忘了）
    unsigned char *pDataForRGB = new unsigned char[nDataSize];
    stConvertParam.nWidth = pFrameInfo->nWidth; // 宽
    stConvertParam.nHeight = pFrameInfo->nHeight;   // 高
    stConvertParam.pSrcData = pData;    // 源数据指针
    stConvertParam.nSrcDataLen = pFrameInfo->nFrameLen; // 源数据长度
    stConvertParam.enSrcPixelType = pFrameInfo->enPixelType;    // 像素格式
    stConvertParam.enDstPixelType = PixelType_Gvsp_RGB8_Packed; // 目标像素格式：RGB
    stConvertParam.pDstBuffer = pDataForRGB;    // 目标缓冲区
    stConvertParam.nDstBufferSize = nDataSize;  // 目标缓冲区大小
    // 像素格式转换
    m_nRet = MV_CC_ConvertPixelType(m_handle, &stConvertParam);
    cv::Mat result;
    if (MV_OK == m_nRet) {
        // 创建临时Mat对象指向转换后的RGB数据
        cv::Mat temp(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3, pDataForRGB);
        // 将RGB格式转换为OpenCV标准的BGR格式
        cv::cvtColor(temp, result, cv::COLOR_RGB2BGR);
        // 深拷贝
        result = result.clone();
    }
    delete[] pDataForRGB;
    // 返回深拷贝
    return result;
}

// 自定义曝光时间滑动条回调函数
void Camera::getExposureTime(int pos, void *pCamera) {
    if (!pCamera) {
        std::cerr << "错误: 相机指针为空!" << std::endl;
        return;
    }
    float exposureTime = static_cast<float>(pos) * 1000.0f; // 转成ms
    // 最小曝光时间限制
    if (exposureTime < 100.0f) exposureTime = 100.0f;
    // reinterpret_cast 将 void* 转换回 Camera*，再通过指针调用 Camera 类的setValue成员函数
    int success = reinterpret_cast<Camera *>(pCamera)->setValue("ExposureTime", exposureTime);
    if(success != MV_OK) {
        std::cerr << "设置曝光时间失败! 错误码" << success << std::endl;
        return;
    }
    std::cout << "设置曝光时间:" << std::setprecision(6) << exposureTime / 1000.f << "毫秒" << std::endl;
}

// 自定义增益滑动条回调函数
void Camera::getGain(int pos, void* pCamera) {
    if(!pCamera) {
        std::cerr << "错误，相机指针为空！" << std::endl;
        return;
    }
    float gainValue = static_cast<float>(pos);
    int success = reinterpret_cast<Camera *>(pCamera)->setValue("Gain", gainValue);
    if(success != MV_OK) {
        std::cerr << "设置增益失败! 错误码" << success << std::endl;
        return;
    }
    std::cout << "增益: " << gainValue << "dB" << std::endl;
}

void Camera::initTrackbar(const std::string windowName, float& currExposureTime, float& currGain) {
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    // 读取当前的曝光时间设置
    if(getValue("ExposureTime", currExposureTime) != MV_OK) {
        std::cerr << "获取曝光时间失败，使用默认值!" << std::endl;
        currExposureTime = 10000;   // 10ms
    }
    if(getValue("Gain", currGain) != MV_OK) {
        std::cerr << "获取增益失败，使用默认值!" << std::endl;
        currGain = 3;   // 3dB
    }
    // 初始位置和最大值
    int initExposurePos = static_cast<int>(currExposureTime / 1000.0f);
    int initGainPos = static_cast<int>(currGain);
    int max_Exposure_value = 300;   // 300ms
    int max_Gain_value = 16;    // 16dB
    // 创建滑动条：部分参数（方便调，别忘了）
        // nullptr：滑动条数值指针（设为nullptr，手动设置位置，确保滑动条位置真实反映相机状态，传入初始值显示设置失败）
        // getExposureTime：自定义回调函数
    cv::createTrackbar("曝光时间(ms)", windowName, nullptr, max_Exposure_value, getExposureTime, this);
    // 设置滑动条初始位置
    cv::setTrackbarPos("曝光时间(ms)", windowName, initExposurePos);
    // 同理
    cv::createTrackbar("增益(dB)", windowName, nullptr, max_Gain_value, getGain, this);
    cv::setTrackbarPos("增益(dB)", windowName, initGainPos);
}