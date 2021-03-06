/*
	rugp save decryption
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	FILE *infile = NULL,  *outfile = NULL;
	unsigned char *buffer = NULL;
	unsigned int key=0x0732CE9A;
	unsigned int size1=0, size2=0;
	int decAmt=0x20, i=0, x=0;

	if(argc!=3)
	{
		printf("usage: %s infile outfile\n",argv[0]);
		return 0;
	}
	
	infile = fopen(argv[1],"rb");
	if(!infile)
	{
		printf("Could not open %s\n",argv[1]);
		return -1;
	}
	
	outfile = fopen(argv[2],"wb");
	
	fread(&size1,1,4,infile);
	fread(&size2,1,4,infile);	

	size1 = ~size1^0xC92E568B;
	size2 = (size2^0xC92E568F)>>3;

	buffer = (unsigned char*)calloc(size1,sizeof(unsigned char));

	for(i=size1; i>0;)
	{	
		decAmt = 0x20;
		
		if(i-decAmt<0)
			decAmt = i;
		
		i -= decAmt;

		fread(buffer+x,1,decAmt,infile);

		for(; decAmt>0; decAmt--, x++)
		{
			buffer[x] ^= (key&0x000000ff);
			key = ~(key*2+((key>>0x0f)&1)+0xA3B376C9);
		}
		
		fread(&size2,1,2,infile);
	}
	
	fwrite(buffer,1,size1,outfile);
	free(buffer);
	
	fclose(infile);
	fclose(outfile);
	
	return 0;
}
