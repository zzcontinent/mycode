#include <stdio.h>
int main(void)
{
	int inB;
	while (scanf("%d", &inB) != EOF) {
		int cnt = 0, maxcnt = 0;
		for (int i = 0; i < 8; i++) {
			if (inB & (1 << i)) {
				cnt++;
			} else {
				cnt = 0;
			}
			if (maxcnt < cnt) {
				maxcnt = cnt;
			}
		}
		printf("%d\n", maxcnt);
	}
	printf("exit");
}
