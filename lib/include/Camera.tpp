#include "Camera.h"
#include <type_traits>

// setValue 的模板实现
template<typename T>
int Camera::setValue(const std::string& nodeName, T value) {
    if(m_handle == nullptr) return -1;
    // 使用 C++17 的 if constexpr，可以在编译时根据类型选择代码分支
    // // 只有 T = "对应类型" 时编译对应部分
    if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, int>) {
        int64_t intValue = static_cast<int64_t>(value);
        m_nRet = MV_CC_SetIntValueEx(m_handle, nodeName.c_str(), intValue);
        if (m_nRet != MV_OK) {
            std::cerr << "设置整型参数 " << nodeName << " 失败! 错误码=" << m_nRet << std::endl;
        }
        return m_nRet;
    }
    else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        float floatValue = static_cast<float>(value);
        m_nRet = MV_CC_SetFloatValue(m_handle, nodeName.c_str(), floatValue);
        if (m_nRet != MV_OK) {
            std::cerr << "设置浮点参数 " << nodeName << " 失败! 错误码=" << m_nRet << std::endl;
        }
        return m_nRet;
    }
    else if constexpr (std::is_same_v<T, bool>) {
        m_nRet = MV_CC_SetBoolValue(m_handle, nodeName.c_str(), value);
        if (m_nRet != MV_OK) {
            std::cerr << "设置布尔参数 " << nodeName << " 失败! 错误码=" << m_nRet << std::endl;
        }
        return m_nRet;
    }
    else if constexpr (std::is_same_v<T, std::string> || std::is_convertible_v<T, std::string>) {
        const std::string strValue = value;
        // 尝试按枚举设置，如果失败再尝试按字符串设置
        m_nRet = MV_CC_SetEnumValueByString(m_handle, nodeName.c_str(), strValue.c_str());
        if (m_nRet != MV_OK) {
            m_nRet = MV_CC_SetStringValue(m_handle, nodeName.c_str(), strValue.c_str());
            if (m_nRet != MV_OK) {
                std::cerr << "设置字符串参数 " << nodeName << " 失败! 错误码=" << m_nRet << std::endl;
            }
        }
        return m_nRet;
    }
    else if constexpr (std::is_same_v<T, unsigned int>) {
        MVCC_ENUMVALUE stEnumValue{};
        stEnumValue.nCurValue = value;
        m_nRet = MV_CC_SetEnumValue(m_handle, nodeName.c_str(), stEnumValue.nCurValue);
        if (m_nRet != MV_OK) {
            std::cerr << "设置枚举参数 " << nodeName << " 失败! 错误码=" << m_nRet << std::endl;
        }
        return m_nRet;
    }
    else {
        // 如果传入了不支持的类型，编译时就会报错
        static_assert(sizeof(T) == 0, "setValue不支持此类型。仅支持整型、浮点型、布尔型、字符串型和枚举类型。");
        return -1;
    }
}

// getValue 的模板实现
template<typename T>
int Camera::getValue(const std::string& nodeName, T& value) {
    if(m_handle == nullptr) return -1;
    if constexpr (std::is_same_v<T, int>) {
        MVCC_INTVALUE_EX val;
        m_nRet = MV_CC_GetIntValueEx(m_handle, nodeName.c_str(), &val);
        if(m_nRet == MV_OK) {
            value = static_cast<int>(val.nCurValue);
        }else {
            std::cerr << "获取整型参数 " << nodeName << " 失败! 错误码=" << m_nRet << std::endl;
        }
        return m_nRet;
    }
    else if constexpr (std::is_same_v<T, float>) {
        MVCC_FLOATVALUE val;
        m_nRet = MV_CC_GetFloatValue(m_handle, nodeName.c_str(), &val);
        if(m_nRet == MV_OK) {
            value = val.fCurValue;
        }else {
            std::cerr << "获取浮点参数 " << nodeName << " 失败! 错误码=" << m_nRet << std::endl;
        }
        return m_nRet;
    }
    else if constexpr (std::is_same_v<T, bool>) {
        m_nRet = MV_CC_GetBoolValue(m_handle, nodeName.c_str(), &value);
        if (m_nRet != MV_OK) {
            std::cerr << "获取布尔参数 " << nodeName << " 失败! 错误码=" << m_nRet << std::endl;
        }
        return m_nRet;
    }
    else if constexpr (std::is_same_v<T, std::string>) {
        // 先获取节点类型，因为可能两类：纯字符串和枚举
        MV_XML_InterfaceType nodeType;
        m_nRet = getNodeType(nodeName, nodeType);
        if(m_nRet != MV_OK) return m_nRet;
        if(nodeType == IFT_IString) {
            MVCC_STRINGVALUE val;
            m_nRet = MV_CC_GetStringValue(m_handle, nodeName.c_str(), &val);
            if(m_nRet == MV_OK) value = val.chCurValue;
            else {
                std::cerr << "获取字符串参数 " << nodeName << " 失败! 错误码=" << m_nRet << std::endl;
            }
            return m_nRet;
        } else if (nodeType == IFT_IEnumeration) {
            MVCC_ENUMVALUE enum_v;
            m_nRet = MV_CC_GetEnumValue(m_handle, nodeName.c_str(), &enum_v);
            if (m_nRet != MV_OK) {
                std::cerr << "获取枚举参数 " << nodeName << " 失败! 错误码=" << m_nRet << std::endl;
                return m_nRet;
            }
            MVCC_ENUMENTRY enum_entry = {0};
            enum_entry.nValue = enum_v.nCurValue;
            m_nRet = MV_CC_GetEnumEntrySymbolic(m_handle, nodeName.c_str(), &enum_entry);
            if (m_nRet == MV_OK) value = enum_entry.chSymbolic;
            else {
                std::cerr << "获取枚举符号值 " << nodeName << " 失败! 错误码=" << m_nRet << std::endl;
            }
            return m_nRet;
        }else {
            std::cerr << "不支持字符串获取的节点类型: " << nodeName << std::endl;
            return -1;
        }
    }
    else if constexpr (std::is_same_v<T, unsigned int>) {
        MVCC_ENUMVALUE val;
        m_nRet = MV_CC_GetEnumValue(m_handle, nodeName.c_str(), &val);
        if (m_nRet == MV_OK) {
            value = val.nCurValue;
        } else {
            std::cerr << "获取枚举参数 " << nodeName << " 失败! 错误码=" << m_nRet << std::endl;
        }
        return m_nRet;
    }
    else {
        static_assert(sizeof(T) == 0, "getValue不支持此类型，仅支持整型、浮点型、布尔型、字符串型和枚举类型。");
        return -1;
    }
}