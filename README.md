# 🎥 海康威视相机 C++ 封装库

<div align="center">

![License](https://img.shields.io/badge/License-MIT-blue.svg)
![C++](https://img.shields.io/badge/C++-17-blue.svg)
![OpenCV](https://img.shields.io/badge/OpenCV-4.x-green.svg)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows-lightgrey.svg)

一个现代化的海康威视相机 C++ 封装库，将原始的 C 语言 SDK 转换为易用的面向对象接口

</div>

## 📋 目录

- [项目简介](#-项目简介)
- [特性](#-特性)
- [项目结构](#-项目结构)
- [环境要求](#-环境要求)
- [快速开始](#-快速开始)
- [核心代码详解](#-核心代码详解)
- [学习路径推荐](#-学习路径推荐)
- [API 参考](#-api-参考)
- [示例代码](#-示例代码)
- [常见问题](#-常见问题)
- [贡献指南](#-贡献指南)
- [特别鸣谢](#-特别鸣谢)

## 🚀 项目简介

本项目是一个将海康威视相机 C 语言 SDK 封装为现代 C++ 类库的项目。通过面向对象的设计模式，提供了更加简洁、安全、易用的相机操作接口。

### 主要目标
- 🔄 **现代化改造**：将传统 C 语言 SDK 封装为 C++ 类
- 🛡️ **类型安全**：利用 C++ 模板和类型系统提供编译时类型检查
- 🎯 **易于使用**：简化相机操作流程，降低学习成本
- 🔧 **资源管理**：自动化资源管理，避免内存泄漏
- 📈 **扩展性**：面向对象设计，便于功能扩展

## ✨ 特性

### 🎛️ 核心功能
- ✅ 相机设备枚举和连接
- ✅ 图像实时采集和显示
- ✅ 参数动态调节（曝光时间、增益等）
- ✅ 多种像素格式支持
- ✅ 图像保存功能
- ✅ 交互式滑动条控制

### 🛠️ 技术特色
- 🔹 **模板元编程**：使用 `if constexpr` 实现编译时类型分发
- 🔹 **RAII 管理**：自动资源获取与释放
- 🔹 **静态成员管理**：SDK 生命周期自动管理
- 🔹 **异常安全**：完善的错误处理机制

## 📁 项目结构

```
Camera_encapsulation/
├── 📁 lib/                    # 核心库文件
│   ├── 📁 include/
│   │   ├── 📄 Camera.h        # 主要类声明
│   │   └── 📄 Camera.tpp      # 模板实现
│   ├── 📁 src/
│   │   └── 📄 Camera.cpp      # 类实现
│   └── 📄 CMakeLists.txt      # 库构建配置
├── 📁 app/                    # 应用程序
│   ├── 📁 src/
│   │   └── 📄 main.cpp        # 主程序入口
│   └── 📄 CMakeLists.txt      # 应用构建配置
├── 📁 image/                  # 图像保存目录
├── 📁 .vscode/                # VS Code 配置
├── 📄 CMakeLists.txt          # 主构建配置
└── 📄 README.md               # 项目文档
```

## 🔧 环境要求

### 系统要求
- **操作系统**：Linux (Ubuntu 18.04+) / Windows 10+
- **编译器**：支持 C++17 的编译器
  - GCC 7.0+
  - Clang 5.0+
  - MSVC 2019+

### 依赖库
- **海康威视 MVS SDK**：3.0.0+
- **OpenCV**：4.0+
- **CMake**：3.10+

### 安装依赖

#### Linux (Ubuntu)
```bash
# 安装 OpenCV
sudo apt update
sudo apt install libopencv-dev

# 安装海康威视 SDK
# 下载并安装 MVS SDK from: https://www.hikrobotics.com/cn/machinevision/service/download
```

#### Windows
```bash
# 使用 vcpkg 安装 OpenCV
vcpkg install opencv4[contrib,nonfree]:x64-windows

# 下载并安装海康威视 SDK
# 下载地址: https://www.hikrobotics.com/cn/machinevision/service/download
```

## 🚀 快速开始

### 1. 克隆项目
```bash
git clone <repository-url>
cd Camera_encapsulation
```

### 2. 编译项目
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 3. 运行示例
```bash
./app/App
```

### 4. 基本使用
```cpp
#include "Camera.h"

int main() {
    Camera camera;

    // 打开第一个相机
    if (camera.OpenDeviceIndex(0) != 0) {
        std::cerr << "打开相机失败!" << std::endl;
        return -1;
    }

    // 开始采集
    camera.startGrabbing();

    // 采集图像
    cv::Mat frame = camera.captureFrame();
    if (!frame.empty()) {
        cv::imshow("Camera", frame);
        cv::waitKey(0);
    }

    return 0;
}
```

## 🔍 核心代码详解

### 1. 类设计架构

#### 📊 类图概览
```cpp
class Camera {
public:
    // 🏗️ 构造与析构
    Camera();                              // 自动初始化 SDK
    ~Camera();                             // 自动清理资源

    // 🔌 设备管理
    int OpenDeviceIndex(int DeviceIndex);  // 打开指定索引相机
    void printDeviceInformation();         // 打印设备信息

    // ⚙️ 参数控制 (模板方法)
    template<typename T> int setValue(const std::string& nodeName, T value);
    template<typename T> int getValue(const std::string& nodeName, T& value);

    // 📷 图像采集
    int startGrabbing();                   // 开始采集
    int stopGrabbing();                    // 停止采集
    cv::Mat captureFrame();                // 获取单帧图像

private:
    // 🔧 内部管理
    static bool initSDK();                 // SDK 初始化
    static bool FinalSDK();                // SDK 清理
    cv::Mat convertToMat(/*...*/);         // 图像格式转换

    // 📊 静态成员
    static int m_cameraCount;              // 相机实例计数
    static bool m_isSDKInit;               // SDK 初始化状态
};
```

### 2. 🎯 核心技术详解

#### 🔥 模板元编程 - 类型安全的参数设置

**位置**: `lib/include/Camera.tpp:6-59`

这是项目最精彩的部分，使用 C++17 的 `if constexpr` 实现编译时类型分发：

```cpp
template<typename T>
int Camera::setValue(const std::string& nodeName, T value) {
    if(m_handle == nullptr) return -1;

    // 🔍 编译时类型检查和分发
    if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, int>) {
        // 整型参数处理
        int64_t intValue = static_cast<int64_t>(value);
        m_nRet = MV_CC_SetIntValueEx(m_handle, nodeName.c_str(), intValue);
        // ...
    }
    else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        // 浮点型参数处理
        float floatValue = static_cast<float>(value);
        m_nRet = MV_CC_SetFloatValue(m_handle, nodeName.c_str(), floatValue);
        // ...
    }
    else if constexpr (std::is_same_v<T, bool>) {
        // 布尔型参数处理
        m_nRet = MV_CC_SetBoolValue(m_handle, nodeName.c_str(), value);
        // ...
    }
    else {
        // 🚨 编译时错误 - 不支持的类型
        static_assert(sizeof(T) == 0, "setValue不支持此类型");
        return -1;
    }
}
```

**技术亮点**：
- ✨ **编译时优化**：`if constexpr` 确保只编译匹配的分支
- 🛡️ **类型安全**：`static_assert` 在编译时检查类型支持
- 🎯 **零开销抽象**：模板展开后性能等同于直接调用

#### 🏗️ RAII 资源管理

**位置**: `lib/src/Camera.cpp:11-19`

使用 RAII (Resource Acquisition Is Initialization) 模式管理相机资源：

```cpp
Camera::Camera() {
    initSDK();      // 🚀 构造时自动初始化
    initCamera();   // 🔍 枚举设备
}

Camera::~Camera() {
    destoryCamera(); // 🔒 析构时自动清理设备
    FinalSDK();      // 🗑️ 析构时自动清理SDK
}
```

**优势**：
- 🔄 **自动管理**：无需手动管理资源
- 🛡️ **异常安全**：即使发生异常也能正确清理
- 📝 **简化代码**：用户无需关心资源管理细节

#### 🔢 智能计数器 - 单例SDK管理

**位置**: `lib/src/Camera.cpp:21-51`

使用静态成员实现SDK的智能管理：

```cpp
// 📊 静态成员变量
int Camera::m_cameraCount = 0;
bool Camera::m_isSDKInit = false;

bool Camera::initSDK() {
    m_cameraCount++;  // 📈 实例计数增加
    if (!m_isSDKInit) {
        m_isSDKInit = true;
        int nRet = MV_CC_Initialize();  // 🚀 仅初始化一次
        // ...
    }
    return true;
}

bool Camera::FinalSDK() {
    m_cameraCount--;  // 📉 实例计数减少
    if (!m_cameraCount && m_isSDKInit) {
        m_isSDKInit = false;
        MV_CC_Finalize();  // 🗑️ 最后一个实例时才清理
        // ...
    }
    return true;
}
```

**设计模式**：类似于**智能指针的引用计数**
- 📈 **首次创建**：初始化SDK
- 🔄 **多实例共享**：复用已初始化的SDK
- 📉 **最后清理**：所有实例销毁后才清理SDK

#### 🎨 像素格式转换 - 多态处理

**位置**: `lib/src/Camera.cpp:216-277`

支持多种相机像素格式的智能转换：

```cpp
cv::Mat Camera::convertToMat(unsigned char *pData, MV_FRAME_OUT_INFO_EX *pFrameInfo) {
    cv::Mat image;

    switch (pFrameInfo->enPixelType) {
        case PixelType_Gvsp_Mono8: {
            // 🔳 单通道灰度图像
            image = cv::Mat(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
            break;
        }
        case PixelType_Gvsp_RGB8_Packed: {
            // 🌈 RGB彩色图像
            cv::Mat temp(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3, pData);
            cv::cvtColor(temp, image, cv::COLOR_RGB2BGR);  // OpenCV使用BGR
            break;
        }
        case PixelType_Gvsp_BayerRG8:
        case PixelType_Gvsp_BayerGB8:
        case PixelType_Gvsp_BayerGR8:
        case PixelType_Gvsp_BayerBG8: {
            // 🔶 Bayer格式处理
            cv::Mat bayerImg(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1, pData);
            cv::cvtColor(bayerImg, image, cv::COLOR_BayerBG2BGR);
            break;
        }
        default: {
            // 🔧 使用SDK转换其他格式
            image = convertUsingSDK(pData, pFrameInfo);
            break;
        }
    }

    return image.clone();  // 🔄 返回深拷贝，确保数据安全
}
```

### 3. 🎛️ 交互式控制系统

**位置**: `lib/src/Camera.cpp:312-337`

实现了基于 OpenCV 滑动条的实时参数调节：

```cpp
void Camera::initTrackbar(const std::string windowName, float& currExposureTime, float& currGain) {
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

    // 📖 读取当前相机参数
    if(getValue("ExposureTime", currExposureTime) != MV_OK) {
        currExposureTime = 10000;   // 默认10ms
    }
    if(getValue("Gain", currGain) != MV_OK) {
        currGain = 3;   // 默认3dB
    }

    // 🎚️ 创建滑动条
    cv::createTrackbar("曝光时间(ms)", windowName, nullptr, 300, getExposureTime, this);
    cv::setTrackbarPos("曝光时间(ms)", windowName, static_cast<int>(currExposureTime / 1000.0f));

    cv::createTrackbar("增益(dB)", windowName, nullptr, 16, getGain, this);
    cv::setTrackbarPos("增益(dB)", windowName, static_cast<int>(currGain));
}
```

**回调函数机制**：
```cpp
void Camera::getExposureTime(int pos, void *pCamera) {
    float exposureTime = static_cast<float>(pos) * 1000.0f;
    if (exposureTime < 100.0f) exposureTime = 100.0f;  // 最小值限制

    // 🔄 类型转换后调用模板方法
    int success = reinterpret_cast<Camera *>(pCamera)->setValue("ExposureTime", exposureTime);
    // ...
}
```

## 📚 学习路径推荐

### 🎯 入门路径 (2-3周)

#### 第一周：基础概念
1. **📖 理论学习**
   - 工业相机基础知识
   - 海康威视SDK文档阅读
   - OpenCV图像处理基础

2. **🔧 环境搭建**
   - SDK安装配置
   - 开发环境搭建
   - 运行本项目示例

3. **💻 基础实践**
   - 相机连接和图像显示
   - 参数读取和设置
   - 图像保存功能

#### 第二周：核心技术
1. **🏗️ C++ 现代特性**
   - RAII 资源管理模式
   - 模板元编程基础
   - `if constexpr` 用法

2. **📷 图像处理进阶**
   - 像素格式理解
   - 色彩空间转换
   - 图像质量优化

3. **🔄 代码重构练习**
   - 分析本项目的C++封装思路
   - 尝试添加新功能
   - 优化错误处理机制

#### 第三周：实战应用
1. **🚀 项目扩展**
   - 多相机支持
   - 录像功能实现
   - 图像算法集成

2. **🛠️ 工程化**
   - 单元测试编写
   - 文档完善
   - 性能优化

### 🚀 进阶路径 (1-2个月)

#### 阶段一：深入理解 (2周)
- **🔍 源码分析**：深入研究海康SDK API
- **⚡ 性能优化**：多线程图像处理
- **🔧 扩展开发**：添加更多相机功能

#### 阶段二：系统集成 (2-3周)
- **🌐 网络功能**：远程相机控制
- **🤖 AI集成**：结合深度学习模型
- **💾 数据管理**：图像数据库系统

#### 阶段三：产品化 (2-3周)
- **🎨 GUI开发**：用户友好界面
- **📦 打包发布**：跨平台部署
- **📊 监控系统**：实时状态监控

### 📖 推荐学习资源

#### 📚 书籍推荐
- 《Effective Modern C++》- Scott Meyers
- 《C++ Primer》- Stanley Lippman
- 《OpenCV 4 计算机视觉项目实战》

#### 🌐 在线资源
- [海康威视官方文档](https://www.hikrobotics.com/cn/machinevision/service/download)
- [OpenCV官方教程](https://opencv.org/courses/)
- [cppreference.com](https://en.cppreference.com/) - C++参考手册

## 📖 API 参考

### 🏗️ 构造函数与析构函数

```cpp
Camera();   // 构造函数：自动初始化SDK和枚举设备
~Camera();  // 析构函数：自动清理资源
```

### 🔌 设备管理

```cpp
// 打开指定索引的相机设备
int OpenDeviceIndex(int DeviceIndex);

// 打印所有枚举到的设备信息
void printDeviceInformation();
```

### ⚙️ 参数控制

```cpp
// 设置相机参数（支持多种类型）
template<typename T>
int setValue(const std::string& nodeName, T value);

// 获取相机参数（支持多种类型）
template<typename T>
int getValue(const std::string& nodeName, T& value);

// 查询节点类型
int getNodeType(const std::string& nodeName, MV_XML_InterfaceType& nodeType);
```

**支持的参数类型**：
- `int` / `int64_t` - 整型参数
- `float` / `double` - 浮点型参数
- `bool` - 布尔型参数
- `std::string` - 字符串/枚举参数
- `unsigned int` - 无符号整型参数

**常用参数名称**：
- `"ExposureTime"` - 曝光时间 (μs)
- `"Gain"` - 增益 (dB)
- `"Width"` / `"Height"` - 图像尺寸
- `"PixelFormat"` - 像素格式
- `"TriggerMode"` - 触发模式

### 📷 图像采集

```cpp
// 开始图像采集
int startGrabbing();

// 停止图像采集
int stopGrabbing();

// 获取单帧图像
cv::Mat captureFrame();
```

### 🎛️ UI控制

```cpp
// 初始化参数调节滑动条
void initTrackbar(const std::string windowName,
                  float& currExposureTime,
                  float& currGain);
```

### 📊 返回值说明

| 返回值 | 含义 | 说明 |
|--------|------|------|
| `0` (MV_OK) | 成功 | 操作执行成功 |
| `-1` | 一般错误 | 句柄为空或其他一般性错误 |
| 其他负值 | SDK错误码 | 具体错误码含义见海康SDK文档 |

## 💡 示例代码

### 📷 基础图像采集

```cpp
#include "Camera.h"
#include <opencv2/opencv.hpp>

int main() {
    Camera camera;

    // 打开第一个相机
    if (camera.OpenDeviceIndex(0) != 0) {
        std::cerr << "无法打开相机!" << std::endl;
        return -1;
    }

    // 设置参数
    camera.setValue("ExposureTime", 10000.0f);  // 10ms曝光
    camera.setValue("Gain", 5.0f);              // 5dB增益

    // 开始采集
    if (camera.startGrabbing() != 0) {
        std::cerr << "开始采集失败!" << std::endl;
        return -1;
    }

    // 图像采集循环
    while (true) {
        cv::Mat frame = camera.captureFrame();
        if (frame.empty()) continue;

        cv::imshow("Camera Preview", frame);

        int key = cv::waitKey(1);
        if (key == 27) break;  // ESC退出
    }

    camera.stopGrabbing();
    cv::destroyAllWindows();
    return 0;
}
```

### 🎚️ 交互式参数调节

```cpp
#include "Camera.h"
#include <opencv2/opencv.hpp>

int main() {
    Camera camera;
    camera.OpenDeviceIndex(0);

    // 获取当前参数
    float exposureTime, gain;
    camera.getValue("ExposureTime", exposureTime);
    camera.getValue("Gain", gain);

    // 创建交互界面
    std::string windowName = "相机控制";
    camera.initTrackbar(windowName, exposureTime, gain);

    camera.startGrabbing();

    while (true) {
        cv::Mat frame = camera.captureFrame();
        if (!frame.empty()) {
            cv::imshow(windowName, frame);
        }

        int key = cv::waitKey(1);
        if (key == 27) break;          // ESC退出
        if (key == 's' || key == 'S') { // S保存图像
            cv::imwrite("capture.jpg", frame);
            std::cout << "图像已保存!" << std::endl;
        }
    }

    return 0;
}
```

### 🔍 参数查询示例

```cpp
void printCameraInfo(Camera& camera) {
    // 查询相机基本信息
    int width, height;
    camera.getValue("Width", width);
    camera.getValue("Height", height);

    float frameRate;
    camera.getValue("AcquisitionFrameRate", frameRate);

    std::string pixelFormat;
    camera.getValue("PixelFormat", pixelFormat);

    std::cout << "📊 相机信息:" << std::endl;
    std::cout << "🖼️  分辨率: " << width << "x" << height << std::endl;
    std::cout << "🎬 帧率: " << frameRate << " fps" << std::endl;
    std::cout << "🎨 像素格式: " << pixelFormat << std::endl;
}
```

### 🛡️ 错误处理示例

```cpp
bool setupCamera(Camera& camera) {
    try {
        // 打开相机
        if (camera.OpenDeviceIndex(0) != 0) {
            throw std::runtime_error("无法打开相机设备");
        }

        // 设置参数
        if (camera.setValue("ExposureTime", 15000.0f) != 0) {
            std::cerr << "⚠️  设置曝光时间失败，使用默认值" << std::endl;
        }

        if (camera.setValue("Gain", 3.0f) != 0) {
            std::cerr << "⚠️  设置增益失败，使用默认值" << std::endl;
        }

        // 开始采集
        if (camera.startGrabbing() != 0) {
            throw std::runtime_error("无法开始图像采集");
        }

        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "❌ 错误: " << e.what() << std::endl;
        return false;
    }
}
```

## ❓ 常见问题

### 🔧 编译问题

**Q: 编译时提示找不到 MvCameraControl.h**
```bash
A: 检查海康SDK是否正确安装，确认路径配置：
   - Linux: /opt/MVS/include
   - Windows: C:\Program Files (x86)\MVS\Development\Includes
```

**Q: 链接错误：undefined reference to MV_CC_xxx**
```bash
A: 检查库文件路径和链接配置：
   - 确认 libMvCameraControl.so (Linux) 或 MvCameraControl.lib (Windows) 存在
   - 检查 CMakeLists.txt 中的库路径配置
```

### 📷 运行问题

**Q: 程序运行时提示"未找到任何设备"**
```bash
A: 检查以下项目：
   1. 相机是否正确连接
   2. 相机驱动是否安装
   3. USB/网络连接是否正常
   4. 程序是否有足够权限访问设备
```

**Q: 图像显示异常或颜色错误**
```bash
A: 可能的原因：
   1. 像素格式不匹配 - 检查 convertToMat 函数
   2. 色彩空间转换错误 - 确认 RGB/BGR 转换
   3. 相机参数设置问题 - 检查曝光和增益设置
```

### ⚡ 性能问题

**Q: 图像采集帧率很低**
```bash
A: 优化建议：
   1. 调整曝光时间，过长的曝光会降低帧率
   2. 检查网络带宽（GigE相机）
   3. 优化图像处理算法
   4. 考虑使用多线程采集
```

**Q: 内存占用过高**
```bash
A: 检查要点：
   1. 确认图像内存正确释放（使用 clone() 进行深拷贝）
   2. 避免在循环中创建大量临时对象
   3. 适当调整图像缓冲区大小
```

### 🔧 开发建议

**Q: 如何添加新的相机参数支持？**
```cpp
A: 修改模板函数，添加新的类型分支：
   1. 在 setValue 模板中添加 if constexpr 分支
   2. 在 getValue 模板中添加对应的获取逻辑
   3. 确保类型安全和错误处理
```

**Q: 如何支持多相机同时工作？**
```cpp
A: 设计建议：
   1. 将相机句柄改为非静态成员
   2. 每个 Camera 实例管理独立的设备
   3. 使用线程池进行并发图像采集
   4. 注意线程安全和资源竞争
```

## 🤝 贡献指南

我们欢迎所有形式的贡献！

### 🎯 贡献方式

1. **🐛 Bug 报告**
   - 使用 GitHub Issues 报告问题
   - 提供详细的重现步骤
   - 包含系统环境信息

2. **✨ 功能请求**
   - 在 Issues 中描述新功能需求
   - 说明使用场景和预期效果

3. **💻 代码贡献**
   - Fork 项目并创建分支
   - 遵循现有代码风格
   - 添加适当的测试和文档
   - 提交 Pull Request

### 📝 开发规范

#### 代码风格
- 使用现代 C++ 特性 (C++17+)
- 遵循 RAII 原则
- 优先使用智能指针管理资源
- 函数命名使用 camelCase
- 类名使用 PascalCase

#### 提交规范
```bash
feat: 添加新功能
fix: 修复bug
docs: 更新文档
style: 代码格式调整
refactor: 重构代码
test: 添加测试
chore: 构建或工具相关
```

### 🧪 测试指南

运行测试前请确保：
1. 连接相机设备
2. 安装所有依赖
3. 配置正确的环境变量

```bash
# 编译测试
mkdir build && cd build
cmake -DBUILD_TESTS=ON ..
make

# 运行测试
ctest --output-on-failure
```

## 📄 许可证

本项目基于 [MIT License](LICENSE) 开源。

```
MIT License

Copyright (c) 2024 Camera Encapsulation Project

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## 🙏 特别鸣谢

<div align="center">

### 👨‍💻 核心贡献者

<table>
  <tr>
    <td align="center">
      <img src="https://avatars.githubusercontent.com/u/placeholder?v=4" width="100px;" alt="leonal53589"/>
      <br />
      <sub><b>leonal53589</b></sub>
      <br />
      <span>🎯 项目发起人 & 核心开发者</span>
      <br />
      <span>💡 C++ 封装架构设计</span>
      <br />
      <span>🔧 模板元编程实现</span>
    </td>
  </tr>
</table>

### 💝 致谢

**特别感谢 [leonal53589](https://github.com/leonal53589) 的杰出贡献！**

作为本项目的发起人和核心开发者，他设计并实现了：
- 🏗️ **创新的 C++ 封装架构** - 将传统 C SDK 转换为现代面向对象接口
- ⚡ **高效的模板元编程方案** - 实现类型安全的参数设置系统
- 🛡️ **完善的资源管理机制** - 基于 RAII 的自动资源管理
- 🎯 **优雅的错误处理设计** - 编译时类型检查和运行时错误处理

这个项目不仅展现了扎实的 C++ 技术功底，更体现了对工程实践的深刻理解。通过这个封装库，开发者可以更加便捷地使用海康威视相机进行图像采集和处理。

---

### 🌟 开源精神

感谢所有为开源社区贡献力量的开发者们！正是因为有了像 **leonal53589** 这样优秀的贡献者，我们才能共同构建更好的开发工具和库。

如果这个项目对您有帮助，请考虑：
- ⭐ 给项目点个 Star
- 🔄 分享给更多需要的开发者
- 🤝 参与项目的改进和完善

让我们一起推动工业视觉和C++开发社区的发展！

</div>

---

<div align="center">

**📬 联系我们**

如有任何问题或建议，欢迎通过以下方式联系：

[![GitHub Issues](https://img.shields.io/badge/GitHub-Issues-green.svg)](https://github.com/your-repo/issues)
[![Email](https://img.shields.io/badge/Email-Contact-blue.svg)](mailto:your-email@example.com)

**🌟 如果这个项目对您有帮助，请给我们一个 Star！**

[![GitHub Stars](https://img.shields.io/github/stars/your-repo/Camera_encapsulation.svg?style=social)](https://github.com/your-repo/Camera_encapsulation)

---

*Built with ❤️ by the Camera Encapsulation Team*

</div>