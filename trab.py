from typing import List


def Max_rectangle_sum_getter(it_pixels: List[List[int]]):

    # lv_linhas = len (it_pixels)
    # lv_colunas = len (it_pixels[0])

    lv_linhas = len(matrix1)
    lv_colunas = len(matrix1[0])

    lv_maior_soma = float('-inf')
    prefix = [[0 for i in range(lv_colunas)]
              for j in range(lv_linhas)]

    for linha in range(lv_linhas):
        for coluna in range(lv_colunas):

            # for r in range(lv_linhas):
            #     prefix[r] += it_pixels[r][right_edge]
            if (coluna == 0):
                prefix[linha][coluna] = matrix1[linha][coluna]
            else:
                prefix[linha][coluna] = matrix1[linha][coluna] + prefix[linha][coluna - 1]

    for linha in range(lv_linhas):
        for coluna in range(linha, lv_colunas):
            v = []
            # Traverse every row
            for k in range(lv_linhas):
                # Store the sum of the
                # kth row
                el = 0

                # Update the prefix
                # sum
                if (linha == 0):
                    el = prefix[k][coluna]
                else:
                    el = prefix[k][coluna] - prefix[k][linha - 1]
                # Push it in a vector
                v.append(el)

            # Update the maximum
            # overall sum
            lv_maior_soma = max(lv_maior_soma, Kadanes_bigger_sum_getter(v))

    print("Maior soma: " + str(lv_maior_soma))


def Kadanes_bigger_sum_getter(it_pixels: List[int]) -> int:
    currSum = 0
    maxSum = float('-inf')

    # Traverse the array v
    for i in range(len(it_pixels)):

        # Add the value of the
        # current element
        currSum += it_pixels[i]

        # Update the maximum sum
        if (currSum > maxSum):
            maxSum = currSum
        if (currSum < 0):
            currSum = 0

    # Return the maximum sum
    return maxSum


def create_matrix(file_content: str) -> List[List[int]]:
    lines = file_content.split("\n")
    matrix_height = int(lines[0])
    matrix = []
    for i in range(1, matrix_height + 1):
        line = lines[i]
        matrix_line = []
        for number in line.split(" "):
            matrix_line.append(int(number))
        matrix.append(matrix_line)

    return matrix


f = open("tests/in1", "r")
matrix1 = create_matrix(f.read())
# matrix1 = [[4],
#           [-1, -2, -3, -4],
#           [-5, -6, -7, -8],
#           [-9, -10, -11, -12],
#           [-13, -14, -15, -16],
#           ]
Max_rectangle_sum_getter(matrix1)

