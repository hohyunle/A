def fibonacci_queue(n):
    if n <= 0:                                                          
        print("계산할 수 없습니다.")
        return 0
    elif n == 1:                                                        
        return 1                                                      
    fib_queue = [0, 1]                                               
    for i in range(2, n+1):                                           
        fib_queue.append(fib_queue[-2] + fib_queue[-1])             
    return fib_queue[-1]
n = int(input("계산할 피보나치 수열의 F(n)번째 항을 입력하세요: "))
print(f"F({n})번째 항의 피보나치 수열 값은 {fibonacci_queue(n)}입니다.")

