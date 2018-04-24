#pragma once

//Percent of memory that can be used on the device
const double MAX_MEM_AVAIL = 0.95;

#define NUM_BINS (256)
#define MAX_KERNEL_DIM (25)
#define CONST_KERNEL_NUM_EL (MAX_KERNEL_DIM*MAX_KERNEL_DIM*MAX_KERNEL_DIM)

#define SQR(x) ((x)*(x))
#define MAX(x,y) (((x)>(y))?(x):(y))
#define MIN(x,y) (((x)<(y))?(x):(y))

#define mat_to_c(x) ((x)-1)
#define c_to_mat(x) ((x)+1)

#define SIGN(x) (((x)>0) ? (1) : (((x)<0.000001 || (x)>-0.00001) ? (0) : (-1)))
#define CLAMP(val,minVal,maxVal) ((val>=maxVal) ? (maxVal) : ((val<=minVal) ? (minVal) : (val)))

enum ReductionMethods
{
	REDUC_MEAN, REDUC_MEDIAN, REDUC_MIN, REDUC_MAX, REDUC_GAUS
};

// These defines check for non-narrowing (valid) implicit conversions from SrcType -> DstType
#define NON_NARROWING_IMPLICIT(SrcType,DstType) std::is_same<typename std::common_type<SrcType,DstType>::type,DstType>::value

#define VALID_IMPLICIT(SrcType,DstType) typename std::enable_if<NON_NARROWING_IMPLICIT(SrcType,DstType)>::type
#define INVALID_IMPLICIT(SrcType,DstType) typename std::enable_if<!NON_NARROWING_IMPLICIT(SrcType,DstType)>::type

// Helpers for SFINAE overload resolution to avoid implicit narrowing conversions
#define VALID_IMPLICIT_ARG(SrcType,DstType) VALID_IMPLICIT(SrcType,DstType)* = nullptr
#define INVALID_IMPLICIT_ARG(SrcType,DstType) INVALID_IMPLICIT(SrcType,DstType)* = nullptr
