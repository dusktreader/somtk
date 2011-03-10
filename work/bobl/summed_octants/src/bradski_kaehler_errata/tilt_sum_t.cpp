#include <cv.h>
using namespace cv;

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    int n = 5;
    Mat m(n, n, CV_32F);
    Mat sum(n+1, n+1, CV_32F);
    Mat sqsum(n+1, n+1, CV_32F);
    Mat tilted(n+1, n+1, CV_32F);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
#if 0
            m.at<float>(i,j) = 10*i + j;
#else
            m.at<float>(2,2) = 1;
#endif
        }
    }
    cout << m << '\n';
    integral(m, sum, sqsum, tilted);
    cout << "sum:\n"    << sum    << '\n';
    cout << "tilted:\n" << tilted << '\n';
    return 0;
}
