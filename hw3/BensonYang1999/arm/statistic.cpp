#define NPYBIND
#include "_matrix.cpp"

#include <chrono>

using namespace std;
using namespace chrono;

int main(int argc, char **argv)
{
    const size_t a = 1000, b = 1200, c = 1500;

    vector<double> v1(a * b);
    vector<double> v2(b * c);
    iota(v1.begin(), v1.end(), 0);
    iota(v2.begin(), v2.end(), 0);

    Matrix m1(a, b, v1);
    Matrix m2(b, c, v2);

    cout << "------------ Execution time ------------" << endl;
    system_clock::time_point t1 = high_resolution_clock::now();
    Matrix m3 = multiply_naive(m1, m2);
    system_clock::time_point t2 = high_resolution_clock::now();
    system_clock::duration dt_n = duration_cast<nanoseconds>(t2 - t1);
    cout << left << setfill(' ') << setw(20) << "naive execution time"
         << ": ";
    cout << right << setfill(' ') << setw(10) << dt_n.count() << " ns\n";

    t1 = high_resolution_clock::now();
    Matrix m4 = multiply_tile(m1, m2, 16);
    t2 = high_resolution_clock::now();
    system_clock::duration dt_t = duration_cast<nanoseconds>(t2 - t1);
    cout << left << setfill(' ') << setw(20) << "tile execution time"
         << ": ";
    cout << right << setfill(' ') << setw(10) << dt_t.count() << " ns\n";

    t1 = high_resolution_clock::now();
    Matrix m5 = multiply_mkl(m1, m2);
    t2 = high_resolution_clock::now();
    system_clock::duration dt_m = duration_cast<nanoseconds>(t2 - t1);
    cout << left << setfill(' ') << setw(20) << "blas execution time"
         << ": ";
    cout << right << setfill(' ') << setw(10) << dt_m.count() << " ns\n";

    cout << left << setfill(' ') << setw(20) << "tile-naive ratio"
         << ": ";
    cout << double(dt_t.count()) / dt_n.count() << endl;

    cout << left << setfill(' ') << setw(20) << "blas-naive ratio"
         << ": ";
    cout << double(dt_m.count()) / dt_n.count() << endl;

    cout << endl;
    cout << "--------- Compare tile block size ---------" << endl;
    cout << "+-----+---------------+------------------+\n";
    cout << "|  n  | run time (ns) | tile-naive ratio |\n";
    cout << "+-----+---------------+------------------+\n";

    for (size_t i = 0; i <= 128; i += 4)
    {
        if (i == 0){
            t1 = high_resolution_clock::now();
            Matrix m_temp = multiply_naive(m1, m2);
            t2 = high_resolution_clock::now();
        }
        else {
            t1 = high_resolution_clock::now();
            Matrix m_temp = multiply_tile(m1, m2, i);
            t2 = high_resolution_clock::now();
        }
        system_clock::duration dt = duration_cast<nanoseconds>(t2 - t1);
        cout << "| " << right << setw(4) << i;
        cout << "| " << setw(14) << dt.count();
        cout << "| " << setw(17) << double(dt.count()) / dt_n.count();
        cout << "|\n";

        cout << "+-----+---------------+------------------+\n";
    }

    cout << endl;
    cout << "----------- Answer correctness -----------" << endl;
    cout << "naive == tile: " << ((m3 == m4) ? "true" : "false") << endl;
    cout << "naive == blas: " << ((m3 == m5) ? "true" : "false") << endl;

    return 0;
}
