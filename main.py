from concurrent.futures import ThreadPoolExecutor
from functools import reduce

def parallel_reduce(op, data, chunks=4):
    size = len(data) // chunks
    parts = [data[i*size:(i+1)*size] for i in range(chunks-1)] + [data[(chunks-1)*size:]]
    with ThreadPoolExecutor() as executor:
        reduced_parts = executor.map(lambda part: reduce(op, part), parts)
    return reduce(op, reduced_parts)

# User input
data = list(map(int, input("Enter numbers separated by space: ").split()))

# Operations
min_val = parallel_reduce(min, data)
max_val = parallel_reduce(max, data)
sum_val = parallel_reduce(lambda x, y: x + y, data)
avg_val = sum_val / len(data)

print(f"Min: {min_val}, Max: {max_val}, Sum: {sum_val}, Avg: {avg_val:.2f}")
