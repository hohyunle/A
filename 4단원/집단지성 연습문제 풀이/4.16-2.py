# 전체 코드
def powerMat(x, n):
    if n == 1:                                        # 종료조건
        return x
    elif n % 2 == 0:
        return powerMat(multMul(x, x), n // 2)         # n이 짝수
    else:
        return multMul(x, powerMat(x, x), (n - 1) // 2)  # n이 홀수


def multMul(M1, M2):
    rows = len(M1)                          # 첫 번째 행렬의 행 수
    cols = len(M2[0])                       # 두 번째 행렬의 열 수
    result = [[0] * cols for _ in range(rows)]  # 결과 행렬 초기화
    for i in range(rows):                   # 결과 행렬의 행 반복
        for j in range(cols):               # 결과 행렬의 열 반복
            for k in range(len(M2)):
                result[i][j] += M1[i][k] * M2[k][j]

    return result


# 예시
x = [[1, 0, 0], [0, 1, 0], [0, 0, 1]]  # 3x3 단위 행렬
n = 4
result = powerMat(x, n)
print(result)