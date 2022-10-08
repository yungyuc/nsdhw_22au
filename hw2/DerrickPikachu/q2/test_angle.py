import _vector


def test_angle45():
    vec1 = _vector.VectorFloat([1, 0])
    vec2 = _vector.VectorFloat([1, 1])
    assert _vector.angle_between(vec1, vec2) == 45.0

def test_angle0():
    vec1 = _vector.VectorFloat([1, 0])
    vec2 = _vector.VectorFloat([2, 0])
    assert _vector.angle_between(vec1, vec2) == 0.0

def test_angle_zero_len():
    vec1 = _vector.VectorFloat([0, 0])
    vec2 = _vector.VectorFloat([1, 0])
    assert _vector.angle_between(vec1, vec2) == -1.0

def main():
    vec1 = _vector.VectorFloat([1, 0])
    vec2 = _vector.VectorFloat([1, 1])
    print(_vector.angle_between(vec1, vec2))


if __name__ == "__main__":
    main()
