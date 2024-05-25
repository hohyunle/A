def multMul(M1, M2):
    rows = len(M1)                          #첫 번째 행렬의 행 수
    cols = len(M2[0])                       #두 번째 행렬의 열 수
    result = [[0]*cols for _ in range(rows)]#결과 행렬 초기화
    for i in range(rows):                   #결과 행렬의 행 반복
        for j in range(cols):               #결과 행렬의 열 반복
            for k in range(len(M2)):
                result[i][j] += M1[i][k] * M2[k][j]

    return result