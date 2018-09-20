#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "md5.h"

int main(int argc, char** argv)
{
	//printf("%s\n", md5_encrypt(argv[1], strlen(argv[1])));

	FILE* fp = NULL;
	int file_size = 0;
	unsigned char* buff = NULL;

	if(argc != 2)
	{
		fprintf(stderr, "Usage error!\n");//标准错误输出
		return 1;
	}

	fp = fopen(argv[1], "rb");

	if(NULL == fp)
	{
		fprintf(stderr, "open %s fail: %s\n", argv[1], strerror(errno));
	}

	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	rewind(fp);

	buff = malloc(file_size);

	if(NULL == buff)
	{
		perror("allocate memory fail");
		fclose(fp);
		return 1;
	}

	if(fread(buff, 1, file_size, fp) != file_size)
	{
		fprintf(stderr, "read %s fail!\n", argv[1]);
		fclose(fp);
		return 1;
	}

	fclose(fp);

	printf("%s\n", md5_encrypt(buff, file_size));

	
	free(buff);


	return 0;
}
