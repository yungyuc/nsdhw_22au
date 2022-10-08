import angle

def main():
    vec1 = angle.VectorFloat([1, 0])
    vec2 = angle.VectorFloat([1, 1])
    print(angle.angle_between(vec1, vec2))


if __name__ == "__main__":
    main()
