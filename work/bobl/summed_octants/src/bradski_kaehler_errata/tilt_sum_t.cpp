#include <cv.h>
using namespace cv;

#include <iostream>
using namespace std;

#include <getopt.h>
#include <assert.h>

void m_printWithTarget(Mat &m, int x0, int y0)
{
    for (int y = 0; y < m.rows; y++) {
        for (int x = 0; x < m.cols; x++) {
            if (x == x0 && y == y0)
                cout << "  " << "+";
            else
                cout << "  " << m.at<double>(y,x);
        }
        cout << '\n';
    }
}

int main(int argc, char *argv[])
{
    int n = 5;
    int x0 = n/2;
    int y0 = n/2;
    int ch;
    int sdepth = CV_64F;
    Mat m(n, n, sdepth);
    Mat sum(n+1, n+1, sdepth);
    Mat sqsum(n+1, n+1, CV_64F);
    Mat tilted(n+1, n+1, sdepth);

    while ((ch = getopt(argc, argv, "x:y:")) != -1) {
        switch (ch) {

        case 'x':
            x0 = atoi(optarg);
            break;

        case 'y':
            y0 = atoi(optarg);
            break;
        
        default:
            assert(0);
        }
    }

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            if (x == x0 && y == y0)
                m.at<double>(y,x) = 1.0;
            else
                m.at<double>(y,x) = 0.0;
        }
    }

    cout << "input:\n";
    m_printWithTarget(m, x0, y0);

    integral(m, sum, sqsum, tilted, sdepth);

    cout << '\n';

    cout << "sum:\n";
    m_printWithTarget(sum, x0, y0);

    cout << '\n';

    cout << "tilted:\n";
    m_printWithTarget(tilted, x0, y0);
    return 0;
}
