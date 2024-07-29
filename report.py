import matplotlib.pyplot as plt

def read_data(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    original_data = []
    quantized_data = []
    dequantized_data = []
    delta = 0.0
    memory_before = 0
    memory_after = 0

    i = 0
    while i < len(lines):
        line = lines[i]
        if line.startswith("Original data:"):
            i += 1
            while i < len(lines) and lines[i].strip():
                original_data.extend(map(float, lines[i].strip().split()))
                i += 1
        elif line.startswith("Quantized data:"):
            i += 1
            if lines[i].startswith("Delta:"):
                delta = float(lines[i].split()[1])
                i += 1
            while i < len(lines) and lines[i].strip():
                if "Quantized values:" in lines[i]:
                    quantized_values_line = lines[i].split('Quantized values:')[1].strip()
                    quantized_data.extend(map(int, quantized_values_line.split()))
                else:
                    quantized_data.extend(map(int, lines[i].strip().split()))
                i += 1
        elif line.startswith("Dequantized data:"):
            i += 1
            while i < len(lines) and lines[i].strip():
                dequantized_data.extend(map(float, lines[i].strip().split()))
                i += 1
        elif line.startswith("Memory usage before quantization:"):
            memory_before = int(line.strip().split()[-2])
        elif line.startswith("Memory usage after quantization:"):
            memory_after = int(line.strip().split()[-2])
        i += 1

    return original_data, quantized_data, dequantized_data, delta, memory_before, memory_after

def plot_data(original, quantized, dequantized):
    plt.figure(figsize=(12, 6))

    plt.subplot(3, 1, 1)
    plt.plot(original, 'bo-', label='Original Data')
    plt.title('Original Data')
    plt.xlabel('Index')
    plt.ylabel('Value')
    plt.legend()

    plt.subplot(3, 1, 2)
    plt.stem(range(len(quantized)), quantized, 'r', markerfmt='ro', basefmt='r-', label='Quantized Data')
    plt.title('Quantized Data')
    plt.xlabel('Index')
    plt.ylabel('Quantized Value')
    plt.legend()

    plt.subplot(3, 1, 3)
    plt.plot(dequantized, 'go-', label='Dequantized Data')
    plt.title('Dequantized Data')
    plt.xlabel('Index')
    plt.ylabel('Value')
    plt.legend()

    plt.tight_layout()
    plt.savefig('quantization_plot.png')
    plt.show()

def print_statistics(original, dequantized, delta, memory_before, memory_after):
    print("Quantization Report")
    print("===================")
    print(f"Delta value used for quantization: {delta}")
    print(f"Memory usage before quantization: {memory_before} bytes")
    print(f"Memory usage after quantization: {memory_after} bytes")
    print(f"Memory saved: {memory_before - memory_after} bytes")
    print()
    print("Statistics:")
    mean_absolute_error = sum(abs(o - d) for o, d in zip(original, dequantized)) / len(original)
    max_absolute_error = max(abs(o - d) for o, d in zip(original, dequantized))
    print(f"Mean Absolute Error: {mean_absolute_error}")
    print(f"Maximum Absolute Error: {max_absolute_error}")

def main():
    original_data, quantized_data, dequantized_data, delta, memory_before, memory_after = read_data("data/quantization_output.txt")

    plot_data(original_data, quantized_data, dequantized_data)

    print_statistics(original_data, dequantized_data, delta, memory_before, memory_after)

if __name__ == "__main__":
    main()
