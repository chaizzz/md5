#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <openssl/aes.h>

int main(const char argc, char** argv)
{
	if(argc != 5)
	{
		fprintf(stderr, "Usage error!\n");
		return 1;
	}

	if(strcmp(argv[1], "-e") == 0)
	{
		FILE* fp_plain = NULL;
		FILE* fp_encrypted = NULL;
		unsigned char plain_data[16], encrypted_data[16];
		int ret = 0, file_size = 0;
		char user_key[17] = "";
		AES_KEY key;


		strcpy(user_key, argv[4]);
		AES_set_encrypt_key(user_key, 128, &key);

		fp_plain = fopen(argv[2], "rb");

		if(NULL == fp_plain)
		{
			fprintf(stderr, "open %s fail : %s\n", argv[2], strerror(errno));
			exit(1);
		}

		fp_encrypted = fopen(argv[3], "wb");
	
		if(NULL == fp_encrypted)
                {
                        fprintf(stderr, "open %s fail : %s\n", argv[3], strerror(errno));
			fclose(fp_plain);
                        exit(1);
                }

		while(ret = fread(plain_data, 1, 16, fp_plain))
		{
			AES_encrypt(plain_data, encrypted_data, &key);
			fwrite(encrypted_data, 1, 16, fp_encrypted);

			if(ret < 16) break;
		}

		fwrite(&ret, 1, 1, fp_encrypted);

		fclose(fp_plain);
		fclose(fp_encrypted);
	}

	else if(strcmp(argv[1], "-c") == 0)
	{
		FILE* fp_plain = NULL;
		FILE* fp_encrypted = NULL;
		unsigned char plain_data[16], encrypted_data[18];
		int ret = 0, file_size = 0;
		char user_key[17] = "";
		AES_KEY key;


		strcpy(user_key, argv[4]);
		AES_set_decrypt_key(user_key, 128, &key);

		fp_encrypted = fopen(argv[2], "rb");

		if(NULL == fp_encrypted)
                {
                        fprintf(stderr, "open %s fail : %s\n", argv[2], strerror(errno));
                        exit(1);
                }
		
		fp_plain = fopen(argv[3], "wb");

		if(NULL == fp_plain)
                {
                        fprintf(stderr, "open %s fail : %s\n", argv[3], strerror(errno));
                	fclose(fp_encrypted);
		        exit(1);
                }

		while(ret = fread(encrypted_data, 1, 18, fp_encrypted))
		{

			AES_decrypt(encrypted_data, plain_data, &key);

			if(ret < 18)
			{
				fwrite(plain_data, 1, encrypted_data[16], fp_plain);
				break;
			}

			fwrite(plain_data, 1, 16, fp_plain);

			fseek(fp_encrypted, -2, SEEK_CUR);
		}

		fclose(fp_plain);
		fclose(fp_encrypted);
	}

	else
	{
		fprintf(stderr, "Usage error\n");
	}
	return 0;
}
