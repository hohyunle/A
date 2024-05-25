def factorial(n):
    result = 1
    for k in range(n, 0, -1):
        result = result * k
    return result

print(factorial(5))
