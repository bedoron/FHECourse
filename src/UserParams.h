#include <vector>
#include "NumbTh.h"

using std::vector;

#define DEFAULT_P 19
#define DEFAULT_R 1
#define DEFAULT_L 32
#define DEFAULT_SECURITY 16
#define DEFAULT_SIZE 10

class UserParams {
public:
    bool dry=false;
    long R=1;
    long p=DEFAULT_P;
    long r=DEFAULT_R;
    long d=1;
    long c=2;
    long k=DEFAULT_SECURITY; // Security param
    long L=DEFAULT_L;
    long s=0;
    long repeat=1;
    long chosen_m=0;
    vector<long> mvec;
    vector<long> gens;
    vector<long> ords;
    long seed=0;
    long nt=1;
    bool noPrint;

    UserParams();
    void parse(int argc, char** argv);

private:
    ArgMapping amap;
};