#include <cassert>
#include <cfloat>
#include "swaml_bitquant.h"

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

void QuantizeBits::quantize(const swaml_fp32_t* input, size_t size) {
	switch (block_.getType()) {
		case QuantizationType::QUANTIZE4:
			quantize4(input, size);
			break;
		case QuantizationType::QUANTIZE4D:
			quantize4d(input, size);
			break;
	}
}

void QuantizeBits::dequantize(swaml_fp32_t* output, size_t size) {
	switch (block_.getType()) {
		case QuantizationType::QUANTIZE4:
			dequantize4(output, size);
			break;
		case QuantizationType::QUANTIZE4D:
			dequantize4d(output, size);
			break;
	}
}

void QuantizeBits::quantize4(const swaml_fp32_t* input, size_t size) {    // QK4_0
	static const int qk = 32;
	assert(size % qk == 0);

	const int nb = size / qk;
	std::vector<uint8_t> quantizedValues(nb * qk / 2);

	for (int i = 0; i < nb; i++) {
		float amax = 0.0f;
		float max = 0.0f;

		for (int j = 0; j < qk; j++) {
			const float v = input[i * qk + j];
			if (amax < std::fabs(v)) {
				amax = std::fabs(v);
				max = v;
			}
		}

		const float d = max / -8;
		const float id = d ? 1.0f / d : 0.0f;

		block_.setDelta(swaml_fp_32to16(d));

		for (int j = 0; j < qk / 2; ++j) {
			const float x0 = input[i * qk + j] * id;
			const float x1 = input[i * qk + qk / 2 + j] * id;

			const uint8_t xi0 = MIN(15, static_cast<int8_t>(x0 + 8.5f));
			const uint8_t xi1 = MIN(15, static_cast<int8_t>(x1 + 8.5f));

			quantizedValues[i * qk / 2 + j] = xi0 | (xi1 << 4);
		}
	}

	block_.setQuantizedValues(quantizedValues.data(), quantizedValues.size());
}

void QuantizeBits::quantize4d(const swaml_fp32_t* input, size_t size) {    // QK4_1
	static const int qk = 32;
	assert(size % qk == 0);

	const int nb = size / qk;
	std::vector<uint8_t> quantizedValues(nb * qk / 2);

	for (int i = 0; i < nb; i++) {
		float min = FLT_MAX;
		float max = -FLT_MAX;

		for (int j = 0; j < qk; j++) {
			const float v = input[i * qk + j];

			if (v < min) min = v;
			if (v > max) max = v;
		}

		const float d  = (max - min) / ((1 << 4) - 1);
		const float id = d ? 1.0f/d : 0.0f;

		block_.setDelta(swaml_fp_32to16(d));
		block_.setMin(swaml_fp_32to16(min));

		for (int j = 0; j < qk/2; ++j) {
			const float x0 = (input[i*qk + 0    + j] - min)*id;
			const float x1 = (input[i*qk + qk/2 + j] - min)*id;

			const uint8_t xi0 = MIN(15, static_cast<int8_t>(x0 + 0.5f));
			const uint8_t xi1 = MIN(15, static_cast<int8_t>(x1 + 0.5f));

			quantizedValues[i * qk / 2 + j] = xi0 | (xi1 << 4);
		}
	}

	block_.setQuantizedValues(quantizedValues.data(), quantizedValues.size());
}

void QuantizeBits::dequantize4(swaml_fp32_t* output, size_t size) {    // QK4_0
	static const int qk = 32;
	assert(size % qk == 0);

	const int nb = size / qk;
	const float d = swaml_fp_16to32(block_.getDelta());
	const uint8_t* quantizedValues = block_.getQuantizedValues();

	for (int i = 0; i < nb; i++) {
		for (int j = 0; j < qk / 2; ++j) {
			const int x0 = (quantizedValues[i * qk / 2 + j] & 0x0F) - 8;
			const int x1 = (quantizedValues[i * qk / 2 + j] >> 4) - 8;

			output[i * qk + j] = x0 * d;
			output[i * qk + j + qk / 2] = x1 * d;
		}
	}
}

void QuantizeBits::dequantize4d(swaml_fp32_t* output, size_t size) {    // QK4_1
	static const int qk = 32;
	assert(size % qk == 0);

	const int nb = size / qk;
	const float d = swaml_fp_16to32(block_.getDelta());
	const float m = swaml_fp_16to32(block_.getMin());
	const uint8_t* quantizedValues = block_.getQuantizedValues();

	for (int i = 0; i < nb; i++) {
		for (int j = 0; j < qk / 2; ++j) {
			const int x0 = (quantizedValues[i * qk / 2 + j] & 0x0F);
			const int x1 = (quantizedValues[i * qk / 2 + j] >> 4);

			output[i * qk + j] = x0 * d + m;
			output[i * qk + j + qk / 2] = x1 * d + m;
		}
	}
}
