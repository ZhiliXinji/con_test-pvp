
#include <cstdio>
#include <cstdlib>
#include <signal.h>
#include <errno.h>

using namespace std;

int provide_random_number() {
    // Chosen by fair dice roll.
    return 4;
}

int main(int argc, char **argv) {
    signal(SIGPIPE, SIG_IGN);

    FILE *fin, *fout, *fifo1_in, *fifo1_out, *fifo2_in, *fifo2_out;

    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");
    fifo1_in = fopen(argv[2], "w");
    fifo1_out = fopen(argv[1], "r");
    fifo2_in = fopen(argv[4], "w");
    fifo2_out = fopen(argv[3], "r");

    int a, b, res;
    int c = provide_random_number();

    fscanf(fin, "%d %d", &a, &b);
    fprintf(fifo1_in, "%d %d\n", a, b);
    fflush(fifo1_in);
    fprintf(fifo2_in, "%d %d\n", a, b);
    fflush(fifo2_in);

    double sc1, sc2;

    bool fail = false;

    while (true) {
      if(fscanf(fifo1_out, "%d", &res) == 0){
        fail = true;
        break;
      }
      if (res < 0) {
        fprintf(fifo1_in, "%d\n", c);
        fflush(fifo1_in);
        if(errno == EPIPE){
          fail = true;
          break;
        }
      } else {
        fprintf(fout, "%d\n", res);
        fflush(fout);
        break;
      }
    }
    sc1 = fail ? 0.0 : ((res == a + b + c) ? 1.0 : 0.0);

    fail = false;
    while (true) {      
      if(fscanf(fifo2_out, "%d", &res) == 0){
        fail = true;
        break;
      }
      if (res < 0) {
        fprintf(fifo2_in, "%d\n", c);
        fflush(fifo2_in);
        if(errno == EPIPE){
          fail = true;
          break;
        }
      } else {
        fprintf(fout, "%d\n", res);
        fflush(fout);
        break;
      }
    }
    sc2 = fail ? 0.0 : ((res == a + b + c) ? 1.0 : 0.0);

    printf("%.2lf %.2lf\n", sc1, sc2);

    fclose(fin);
    fclose(fout);
    fclose(fifo1_in);
    fclose(fifo1_out);
    fclose(fifo2_in);
    fclose(fifo2_out);
}

