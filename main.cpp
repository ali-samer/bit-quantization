#include "swaml_bitquant.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <filesystem>

int main() {
	const int k = 32;
	std::vector<swaml_fp32_t> data = {1.0, 2.5, 3.0, 4.5, 1.0, 2.5, 3.0, 4.5, 1.0, 2.5, 3.0, 4.5, 1.0, 2.5, 3.0, 4.5,
	                                  1.0, 2.5, 3.0, 4.5, 1.0, 2.5, 3.0, 4.5, 1.0, 2.5, 3.0, 4.5, 1.0, 2.5, 3.0, 4.5};
	std::vector<swaml_fp32_t> dequantized(data.size());

	// Ensure the ./data directory exists
	std::filesystem::create_directories("./data");

	std::ofstream file("./data/quantization_output.txt");

	auto print_and_write = [&](const std::string& text) {
	  std::cout << text << std::endl;
	  if (file.is_open()) {
		  file << text << std::endl;
	  }
	};

	print_and_write("Original data:");
	std::cout << std::fixed << std::setprecision(2);
	if (file.is_open()) {
		file << std::fixed << std::setprecision(2);
	}
	for (size_t i = 0; i < data.size(); ++i) {
		std::cout << std::setw(6) << data[i] << " ";
		if (file.is_open()) {
			file << std::setw(6) << data[i] << " ";
		}
		if ((i + 1) % 8 == 0) {
			std::cout << std::endl;
			if (file.is_open()) {
				file << std::endl;
			}
		}
	}
	std::cout << std::endl;
	if (file.is_open()) {
		file << std::endl;
	}

	QuantizeBits quantizer(QuantizationType::QUANTIZE4D);
	quantizer.quantize(data.data(), data.size());

	print_and_write("Quantized data:");
	print_and_write("Delta: " + std::to_string(quantizer.getDelta()));
	std::string quantized_values = "Quantized values: ";
	for (size_t i = 0; i < quantizer.getQuantizedSize(); ++i) {
		quantized_values += std::to_string(static_cast<int>(quantizer.getQuantizedValues()[i])) + " ";
		if ((i + 1) % 8 == 0) {
			quantized_values += "\n                 ";
		}
	}
	print_and_write(quantized_values);

	quantizer.dequantize(dequantized.data(), dequantized.size());

	print_and_write("Dequantized data:");
	for (size_t i = 0; i < dequantized.size(); ++i) {
		std::cout << std::setw(6) << dequantized[i] << " ";
		if (file.is_open()) {
			file << std::setw(6) << dequantized[i] << " ";
		}
		if ((i + 1) % 8 == 0) {
			std::cout << std::endl;
			if (file.is_open()) {
				file << std::endl;
			}
		}
	}
	std::cout << std::endl;
	if (file.is_open()) {
		file << std::endl;
	}

	size_t memory_before = data.size() * sizeof(swaml_fp32_t);
	size_t memory_after = quantizer.getQuantizedSize() + sizeof(swaml_fp16_t);
	print_and_write("Memory usage before quantization: " + std::to_string(memory_before) + " bytes");
	print_and_write("Memory usage after quantization: " + std::to_string(memory_after) + " bytes");

	if (file.is_open()) {
		file.close();
	}

	return 0;
}
