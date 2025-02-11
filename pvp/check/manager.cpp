
#include <cstdio>
#include <cstdlib>
#include <signal.h>

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
    while (true) {
      fscanf(fifo1_out, "%d", &res);
      if (res < 0) {
        fprintf(fifo1_in, "%d\n", c);
        fflush(fifo1_in);
      } else {
        fprintf(fout, "%d\n", res);
        fflush(fout);
        break;
      }
    }

	if (a+b+c == res) {
		fprintf(stderr, "A-ha, you're the best adding program I've ever met!\n");
		printf("1.0 0.0\n");
	} else {
		fprintf(stderr, "How dreadful, never met anyone as dumb as you...\n");
		printf("0.0 1.0\n");
	}

	fclose(fin);
	fclose(fout);
	fclose(fifo1_in);
	fclose(fifo1_out);

}

