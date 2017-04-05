#include <cstdlib>
#include <iostream>
#include "Matrices.h"
#include "Timer.h"

using namespace std;

int main(int argc, char *argv[])
{
    //   m1     *     m2      =      m3
    // 1    2       1    0         3    4
    // 3    4       1    2         7    8

    //   m2     *     m1      =      m3
    // 1    0       1    2         1    2
    // 1    2       3    4         7    10
    Matrix2 m1, m2, m3;
    m1.set(1, 2, 3, 4);
    cout << "m1 =\n";
    cout << m1;
    m2.set(1, 0, 1, 2);
    cout << "\nm2 =\n";
    cout << m2;
    m3 = m1 * m2;
    cout << "\nm2 * m1 =\n";
    cout << m3;
    m3 = m2 * m1;
    cout << "\nm1 * m2 =\n";
    cout << m3;

    //  m31   *   m32   =   m33
    // 3 1 1     1 1 1     5 5 5
    // 0 1 1     1 1 1     2 2 2
    // 1 1 1     1 1 1     3 3 3

    //  m32   *   m31   =   m33
    // 1 1 1     3 1 1     4 3 3
    // 1 1 1     0 1 1     4 3 3
    // 1 1 1     1 1 1     4 3 3
    Matrix3 m31, m32, m33;
    m31.set(3, 1, 1,
            0, 1, 1,
            1, 1, 1);
    m32.set(1, 1, 1,
            1, 1, 1,
            1, 1, 1);
    cout << "\nm31 =\n";
    cout << m31;
    cout << "\nm32 =\n";
    cout << m32;
    m33 = m31 * m32;
    cout << "\nm32 * m31 =\n";
    cout << m33;
    m33 = m32 * m31;
    cout << "\nm31 * m32 =\n";
    cout << m33;

    //   m41    *    m42    =    m43
    // 3 1 1 1     1 1 1 1     6 6 6 6
    // 0 1 1 1     1 1 1 1     3 3 3 3
    // 1 1 1 1     1 1 1 1     4 4 4 4
    // 1 1 1 1     1 1 1 1     4 4 4 4

    //   m42    *    m41    =    m43
    // 1 1 1 1     3 1 1 1     5 4 4 4
    // 1 1 1 1     0 1 1 1     5 4 4 4
    // 1 1 1 1     1 1 1 1     5 4 4 4
    // 1 1 1 1     1 1 1 1     5 4 4 4
    Matrix4 m41, m42, m43;
    m41.set(3, 1, 1, 1,
            0, 1, 1, 1,
            1, 1, 1, 1,
            1, 1, 1, 1);
    m42.set(1, 1, 1, 1,
            1, 1, 1, 1,
            1, 1, 1, 1,
            1, 1, 1, 1);
    cout << "\nm41 =\n";
    cout << m41;
    cout << "\nm42 =\n";
    cout << m42;

    m43 = m41 * m42;
    cout << "\nm42 * m41 =\n";
    cout << m43;
    m43 = m42 * m41;
    cout << "\nm41 * m42 =\n";
    cout << m43;
//    m1.set(2,-1,0,0, 1,2,5,0, 1,0,4,0, 3,1,2,1);
//    m2 = m1;

//    cout << "ORIGINAL\n";
//    cout << m1;
//    cout << "\nDET: " << m1.getDeterminant() << endl;

//    Timer t;
//    t.start();
//    for(int i = 0; i < 10000000; ++i)
//    {
//        m2 = m1;
//        m2.invertGeneral();
//    }
//    t.stop();
//    cout << "\nINVERSE (General)\n";
//    cout << m2;
//    cout << "ELAPSED TIME (General): " << t.getElapsedTime() << endl;

//    t.start();
//    for(int i = 0; i < 10000000; ++i)
//    {
//        m2 = m1;
//        m2.invertAffine();
//    }
//    t.stop();
//    cout << "ELAPSED TIME (Affine) : " << t.getElapsedTime() << "\n\n";
//    cout << m2;

    system("PAUSE");
    return EXIT_SUCCESS;
}
