import timeit
import _matrix


def main():
    size = 500
    l_m = _matrix.Matrix(size, size)
    r_m = _matrix.Matrix(size, size)
    for r in range(size):
        for c in range(size):
            l_m[r, c] = r * size + c
            r_m[r, c] = r * size + c

    with open("performance.txt", "w") as writer:
        writer.write("Recording Performance for Matrix Multiplication.\n\n")

        dic = {
            "_matrix": _matrix,
            "l_m": l_m,
            "r_m": r_m,
        }

        naive_timer = timeit.Timer("_matrix.multiply_naive(l_m, r_m)", globals=dic)
        naive_time = naive_timer.timeit(number=1)
        writer.write(f"Naive: {naive_time}\n")

        mkl_timer = timeit.Timer("_matrix.multiply_mkl(l_m, r_m)", globals=dic)
        mkl_time = mkl_timer.timeit(number=1)
        writer.write(f"MKL: {mkl_time}\n")

        for tile in [16, 32, 54, 128, 256]:
            dic["tile"] = tile
            tile_timer = timeit.Timer("_matrix.multiply_tile(l_m, r_m, tile)", globals=dic)
            tile_time = tile_timer.timeit(number=1)
            writer.write(f"Tile with {tile} block: {tile_time}\n")

if __name__ == "__main__":
    main()