#pragma once

#include <cstdint>
#include <cstddef>

#include "swaml_types.h"
#include "swaml_quantizeBlock.h"

class QuantizeBits {
public:
	QuantizeBits(QuantizationType type) : block_(type) {}

	void quantize(const swaml_fp32_t* input, size_t size);
	void dequantize(swaml_fp32_t* output, size_t size);

	[[nodiscard]] const uint8_t* getQuantizedValues() const { return block_.getQuantizedValues(); }
	swaml_fp16_t getDelta() const { return block_.getDelta(); }
	QuantizationType getType() const { return block_.getType(); }
	size_t getQuantizedSize() const { return block_.getQuantizedSize();}

private:
	QuantizationBlock block_;

	void quantize4(const swaml_fp32_t* input, size_t size);
	void quantize4d(const swaml_fp32_t* input, size_t size);

	void dequantize4(swaml_fp32_t* output, size_t size);
	void dequantize4d(swaml_fp32_t* output, size_t size);
};

