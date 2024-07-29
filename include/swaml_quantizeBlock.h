#include "swaml_types.h"
#include <vector>

enum class QuantizationType {
	QUANTIZE4,
	QUANTIZE4D,
};


class QuantizationBlock {
public:
	QuantizationBlock(QuantizationType type) : type_(type) {}

	void setDelta(swaml_fp16_t delta) { d_ = delta; }
	swaml_fp16_t getDelta() const { return d_; }

	void setMin(swaml_fp16_t min) { m_ = min; }
	swaml_fp16_t getMin() const { return m_; }

	void setQuantizedValues(const uint8_t* values, size_t size) {
		quantizedValues_.assign(values, values + size);
	}

	size_t getQuantizedSize() const { return quantizedValues_.size(); }
	const uint8_t* getQuantizedValues() const { return quantizedValues_.data(); }
	QuantizationType getType() const { return type_; }

private:
	QuantizationType type_;
	swaml_fp16_t d_;
	swaml_fp16_t m_;
	std::vector<uint8_t> quantizedValues_;
};
