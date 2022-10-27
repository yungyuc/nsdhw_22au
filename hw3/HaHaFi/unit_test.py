from statistics import mean
from _matrix import Matrix as M
from _matrix import multiply_mkl,multiply_naive,multiply_tile
import statistics
import pytest
from timeit import Timer
import sys

class TestClass:
    def make_indentity_matrix(self,size):
        ret=M(size,size)
        for i in range(size):
            ret[i,i]=1
        return ret

    def make_matrices(self, size):

        mat1 = M(size,size)
        mat2 = M(size,size)
        mat3 = M(size,size)

        for it in range(size):
            for jt in range(size):
                mat1[it, jt] = it * size + jt + 1
                mat2[it, jt] = it * size + jt + 1
                mat3[it, jt] = 0

        return mat1, mat2, mat3

    def test_basic(self):
        size = 100
        mat1, mat2, mat3 = self.make_matrices(size)

        assert size == mat1.nrow
        assert size == mat1.ncol
        assert size == mat2.nrow
        assert size == mat2.ncol
        assert size == mat3.nrow
        assert size == mat3.ncol

        assert 2== mat1[0,1]
        assert size+2== mat1[1,1]
        assert size*2== mat1[1,size-1]
        assert size*size== mat1[size-1,size-1]


        for i in range(mat1.nrow):
            for j in range(mat1.ncol):
                assert 0!= mat1[i,j]
                assert mat1[i,j]== mat2[i,j]
                assert 0== mat3[i,j]


        assert mat1== mat2
        assert mat1 is not mat2


    def test_tile(self):
        mat1=M(1000,1000)
        mat2=M(1000,1000)
        mat1[0,0]=1
        mat1[0,1]=1
        mat2[0,0]=1
        mat2[1,0]=1
        ret=multiply_tile(mat1,mat2,64)
        assert ret[0,0]==2


    def test_navie(self):
        mat1=M(1000,1000)
        mat2=M(1000,1000)
        mat1[0,0]=1
        mat1[0,1]=1
        mat2[0,0]=1
        mat2[1,0]=1
        ret=multiply_naive(mat1,mat2)
        assert ret[0,0]==2
 

    def test_mkl(self):
        mat1=M(1000,1000)
        mat2=M(1000,1000)
        mat1[0,0]=1
        mat1[0,1]=1
        mat2[0,0]=1
        mat2[1,0]=1
        ret=multiply_mkl(mat1,mat2)
        
        assert ret[0,0]==2

    def test_mix(self):
        size=250
        mat1, mat2, mat3 = self.make_matrices(size)
        mkl,navie,tile=multiply_mkl(mat1,mat2),multiply_naive(mat1,mat2),multiply_tile(mat1,mat2,128)
        assert mkl==navie
        assert  tile == navie
        assert tile ==mkl
        mkl,navie,tile=multiply_mkl(mat1,mat3),multiply_naive(mat1,mat3),multiply_tile(mat1,mat3,128)
        assert  mkl==mat3
        assert navie==mat3
        assert tile==mat3
        I=self.make_indentity_matrix(size)
        mkl,navie,tile=multiply_mkl(mat1,I),multiply_naive(mat1,I),multiply_tile(mat1,I,128)
        assert  mkl==mat1
        assert navie==mat1
        assert tile==mat1

class Writer:

    def __init__(self, streams):

        self.streams = streams

    def write(self, msg):

        for stream in self.streams:
            stream.write(msg)




if __name__ =="__main__":
    retcode = pytest.main()
    setup="""
import _matrix

size = 1000

mat1 = _matrix.Matrix(size,size)
mat2 = _matrix.Matrix(size,size)

for it in range(size):
    for jt in range(size):
        mat1[it, jt] = it * size + jt + 1
        mat2[it, jt] = it * size + jt + 1
    
    """

    with open("performance.txt","w")as f:
        w = Writer([sys.stdout,f])
        repeat=5
        size=1000

        str_format="%.3f\t%.3f\t%.3f\t%.3f\t\n"
        title_format="%s\t%s\t%s\t%s\t\n"%("min","max",'mean',"std_dev")
        
        w.write(f"start to test three type of multiplication function consume with matrix size({size}*{size}) ")
        w.write(f"and all the function test will repeat {repeat} times\n\n")
        
        # start timer for navie
        naive_time = Timer('_matrix.multiply_naive(mat1, mat2)', setup=setup)
        
        
        # only take min value because of timeit documentation.
        sec =naive_time.repeat(repeat,1)
        navie_min = min(sec)
        navie_max = max(sec)
        navie_mean = mean(sec)
        navie_std_dev = statistics.stdev(sec)

        w.write("Notice!!! you should only care about the min time for the lowerbound and others are only for reference.\n\n")

        w.write(f"multiply_naive time below:\n")
        w.write(title_format)
        w.write(str_format%(navie_min,navie_max,navie_mean,navie_std_dev))
        w.write(f"\n\n")

        # start timer for mkl
        mkl_time = Timer('_matrix.multiply_mkl(mat1, mat2)', setup=setup)
        

        # only take min value because of timeit documentation.
        sec = mkl_time.repeat(repeat,1)
        mkl_min = min(sec)
        mkl_max = max(sec)
        mkl_mean = mean(sec)
        mkl_std_dev = statistics.stdev(sec)

        w.write(f"multiply_mkl time below:\n")
        w.write(title_format)
        w.write(str_format%(mkl_min,mkl_max,mkl_mean,mkl_std_dev))

        w.write(f"the navie/mkl time : {navie_min/mkl_min} \n") 
        w.write(f"the mkl/navie time : {mkl_min/navie_min} \n") 
        w.write(f"\n\n")

        # start timer for tile
        tile_list=[8,16,32,64]
        tile_time = [Timer(f'_matrix.multiply_tile(mat1, mat2,{i})', setup=setup) for i in tile_list ]

        # only take min value because of timeit documentation.
        for index,timer in enumerate( tile_time):
            sec = timer.repeat(repeat,1)
            tile_min = min(sec)
            tile_max = max(sec)
            tile_mean = mean(sec)
            tile_std_dev = statistics.stdev(sec)
            w.write(f"multiply_tile with tile_size->{tile_list[index]} time below:\n")
            w.write(title_format)
            w.write(str_format%(tile_min,tile_max,tile_mean,tile_std_dev))
            w.write(f"the tile/navie time : {tile_min/navie_min} \n") 
            w.write(f"the tile/mkl time : {tile_min/mkl_min} \n") 
            w.write(f"\n\n")
        

