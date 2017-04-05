#include <cstdlib>
#include <iostream>

struct AB
{
    int w;
    int h;
    double *a;
    double d;
    float f;
};

float *ImageArray(int w, int h);

const int width = 3000;
int const height= 3000;
float aa[width * height * 4];

int main()
{
//    int height = 1;
//    std::cout << "tree"
//                 "hhfhfh"
//                 "jdkf"
//              << "fhg"
//              << height;

//    int a = 200;
//    int *p = (int *)0x8000;
//    float *p1 = (float *)&a;
//    void *p2 = &a;
//    int *p3 = &a;
//    float *p4 = (float *)0x8330;
//    void *p5 = (void *)0x8320;
//    int *p6;
//    p6 = (int *)0x8310;

//    AB str1;
//    struct AB str2;
//    str1.h = 1;
//    str1.w = 2;
//    str2.h = 1;
//    str2.w = 2;

//    AB *pstr1;
//    pstr1 = (AB *)&a;
//    AB *pstr2 = &str1;
//    pstr2 = (AB *)0x8300;
//    p1 = (float *)&str1;
//    p2 = &str1;

    AB *pab;
    char *pc;
    int *pn;
    long long *pll;
    long long int *plln;
    double d;
    long double ld;
    long l;
    long long ll;
    std::cout << "AB: " << sizeof(AB) << std::endl
              << "point AB: " << sizeof(pab) << std::endl
              << "point char *: " << sizeof(pc) << std::endl
              << "point short int *: " << sizeof(short int *) << std::endl
              << "point int *: " << sizeof(pn) << std::endl
              << "point long int *: " << sizeof(long int *) << std::endl
              << "point long long int *: " << sizeof(plln) << std::endl
              << "point long *: " << sizeof(long *) << std::endl
              << "point long long *: " << sizeof(pll) << std::endl
              << "point float *: " << sizeof(float *) << std::endl
              << "point double *: " << sizeof(double *) << std::endl
              << "point long double *: " << sizeof(long double *) << std::endl
              << "char: " << sizeof(char) << std::endl
              << "short int: " << sizeof(short int) << std::endl
              << "int: " << sizeof(int) << std::endl
              << "long int: " << sizeof(long int) << std::endl
              << "long long int: " << sizeof(long long int) << std::endl
              << "long: " << sizeof(l) << std::endl
              << "long long: " << sizeof(ll) << std::endl
              << "double: " << sizeof(d) << std::endl
              << "long double: " << sizeof(ld) << std::endl
              << "float: " << sizeof(float) << std::endl
              << "decimal with suffix l: " << sizeof(1.0l) << std::endl
              << "decimal with suffix L: " << sizeof(1.0L) << std::endl
              << "decimal with suffix f: " << sizeof(1.0f) << std::endl
              << "decimal with suffix F: " << sizeof(1.0F) << std::endl
              << "decimal with no suffix: " << sizeof(1.0) << std::endl
              << "integer with suffix l: " << sizeof(1l) << std::endl
              << "integer with suffix L: " << sizeof(1L) << std::endl
              << "integer with suffix ll: " << sizeof(1ll) << std::endl
              << "integer with suffix LL: " << sizeof(1LL) << std::endl
              << "integer with no suffix: " << sizeof(1) << std::endl;

//    float *p = ImageArray(8000, 8000);
//    p[4000000] = 1;
//    aa[4000000] = 2;

    return 0;
}

float *ImageArray(int w, int h)
{
    float *a;
    a = (float *)malloc(sizeof(int) * w * h);

    return a;
}

