/**
 * @file excetdef.h
 * @author ghy (ghy_mike@163.com)
 * @brief  抛出异常
 * @version 1.0
 * @date 2022-08-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#pragma once
#ifndef _EXCETDEF_
#define _EXCETDEF_

#include <stdexcept>
#include <cassert>

namespace GHYSTL{

#define GHYSTL_DEBUG(expr) \
    assert(expr)

#define THROW_LENGTH_ERROR_IF(expr, what) \
    if((expr)) throw std::length_error(what)

#define THROW_OUT_OF_RANGE_IF(expr, what) \
    if((expr)) throw std::out_of_range(what)

#define THROW_RUNTIME_ERROR_IF(expr, what) \
    if((expr)) throw std::runtime_error(what)

#define THROW_BAD_ALLOC \
    throw std::bad_alloc()

#define _BAD_ALLOC \
    std::bad_alloc
    
}
#endif