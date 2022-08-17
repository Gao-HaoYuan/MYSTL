/**
 * @file string.h
 * @author ghy (ghy_mike@163.com)
 * @brief string 类的接口3
 * 
 * 
 * @version 1.0
 * @date 2022-08-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#ifndef _STRING_H_
#define _STRING_H_

#include "base_string.h"

namespace GHYSTL
{
    using string    = GHYSTL::base_string<char>;
    using wstring   = GHYSTL::base_string<wchar_t>;
    using u16string = GHYSTL::base_string<char16_t>;
    using u32string = GHYSTL::base_string<char32_t>;
} // namespace GHYSTL

#endif