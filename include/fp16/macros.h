/*
 * The MIT License (MIT)

Copyright (c) 2017 Facebook Inc.
Copyright (c) 2017 Georgia Institute of Technology
Copyright 2019 Google LLC

Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation files
 (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify,
 merge, publish, distribute, sublicense, and/or sell copies of the
 Software, and to permit persons to whom the Software is furnished to
 do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once
#ifndef FP16_MACROS_H
#define FP16_MACROS_H

#ifndef FP16_USE_NATIVE_CONVERSION
	#if (defined(__INTEL_COMPILER) || defined(__GNUC__)) && defined(__F16C__)
		#define FP16_USE_NATIVE_CONVERSION 1
	#elif defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64)) && defined(__AVX2__)
		#define FP16_USE_NATIVE_CONVERSION 1
	#elif defined(_MSC_VER) && defined(_M_ARM64)
		#define FP16_USE_NATIVE_CONVERSION 1
	#elif defined(__GNUC__) && defined(__aarch64__)
		#define FP16_USE_NATIVE_CONVERSION 1
	#endif
	#if !defined(FP16_USE_NATIVE_CONVERSION)
		#define FP16_USE_NATIVE_CONVERSION 0
	#endif  // !defined(FP16_USE_NATIVE_CONVERSION)
#endif  // !define(FP16_USE_NATIVE_CONVERSION)

#ifndef FP16_USE_FLOAT16_TYPE
	#if !defined(__clang__) && !defined(__INTEL_COMPILER) && defined(__GNUC__) && (__GNUC__ >= 12)
#if defined(__F16C__)
			#define FP16_USE_FLOAT16_TYPE 1
		#endif
	#endif
	#if !defined(FP16_USE_FLOAT16_TYPE)
		#define FP16_USE_FLOAT16_TYPE 0
	#endif  // !defined(FP16_USE_FLOAT16_TYPE)
#endif  // !defined(FP16_USE_FLOAT16_TYPE)

#ifndef FP16_USE_FP16_TYPE
	#if defined(__clang__)
		#if defined(__F16C__) || defined(__aarch64__)
			#define FP16_USE_FP16_TYPE 1
		#endif
	#elif defined(__GNUC__)
#if defined(__aarch64__)
			#define FP16_USE_FP16_TYPE 1
		#endif
	#endif
	#if !defined(FP16_USE_FP16_TYPE)
		#define FP16_USE_FP16_TYPE 0
	#endif  // !defined(FP16_USE_FP16_TYPE)
#endif  // !defined(FP16_USE_FP16_TYPE)

#endif /* FP16_MACROS_H */