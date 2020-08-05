#include <stdio.h>
#include <string.h>
void main()
{
	FILE * fp = fopen("./1","a+");
	if (fp < 0 )
	{
		printf("file open failed!\n");
		return ;
	}
	char * ctx = "hello\n";
	printf("%ld", strlen(ctx));
	fwrite(ctx,1, strlen(ctx), fp);
	//fwrite(ctx, strlen(ctx)+1, 1, fp);
	fclose(fp);
}
