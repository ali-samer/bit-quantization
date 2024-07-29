#ifndef SWAML_TYPES_H
#define SWAML_TYPES_H

#include "fp16/fp16.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef float swaml_fp32_t;

#ifdef __ARM_NEON
typedef __fp16 swaml_fp16_t;
#else
typedef uint16_t swaml_fp16_t;
#endif

#ifdef __ARM_NEON
#include <arm_neon.h>

static inline swaml_fp32_t swaml_fp_16to32(swaml_fp16_t x) {
	return (float)x;
}

static inline swaml_fp16_t swaml_fp_32to16(swaml_fp32_t x) {
	return (__fp16)x;
}

#else

static inline swaml_fp32_t swaml_fp_32to16(swaml_fp16_t h) {
    #if FP16_USE_NATIVE_CONVERSION
        #if FP16_USE_FLOAT16_TYPE
            return (float)h;
        #elif FP16_USE_FP16_TYPE
            return (float)h;
        #else
            return fp16_ieee_to_fp32_value(h);
        #endif
    #else
        return fp16_ieee_to_fp32_value(h);
    #endif
}

static inline swaml_fp16_t swaml_fp_16to32(swaml_fp32_t f) {
    #if FP16_USE_NATIVE_CONVERSION
        #if FP16_USE_FLOAT16_TYPE
            return (_Float16)f;
        #elif FP16_USE_FP16_TYPE
            return (__fp16)f;
        #else
            return fp16_ieee_from_fp32_value(f);
        #endif
    #else
        return fp16_ieee_from_fp32_value(f);
    #endif
}

#endif // __ARM_NEON

#ifdef __cplusplus
}
#endif

#endif // SWAML_TYPES_H
