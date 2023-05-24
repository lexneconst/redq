#include "rmsvw.h"

char *sign_digest(unsigned char *p,unsigned int len)
	{
	static char bufs[10][20];
	static int bnum=0;
	char *ret;
	int i;
	static char *f="0123456789ABCDEF";

	ret= &(bufs[bnum++][0]);
	bnum%=10;
	for (i=0; i<len/2; i++)
		{
		ret[i*2]=f[(p[i]>>4)&0xf];
		ret[i*2+1]=f[p[i]&0xf];
		}
	ret[len]='\0';
	return(ret);
}

void *dat_setfile(const char *dir, icore_read *sdata, const char *message)
{
	SHA512_CTX  sha512_ctx;
	MD5_CTX     md5_ctx;
	u_int8_t    sha512_digest[SHA512_DIGEST_LENGTH];
	char        md5_digest[16];
	char        szfile[MAX_PATH];
	struct _dat_node head;
	int              fd = -1;
	
	SHA512_Init(&sha512_ctx);
	SHA512_Update(&sha512_ctx, sdata->buffer, sdata->length);
	SHA512_Final(sha512_digest, &sha512_ctx);
	 
	MD5Init(&md5_ctx);
	MD5Update(&md5_ctx, sdata->buffer, sdata->length);
	MD5Final(md5_digest, &md5_ctx);
	
	//printf("MD5(%s)\n", sign_digest(md5_digest, 16) );
	 
	sprintf(szfile, "%s/%s.DAT\0", dir, sign_digest(sha512_digest, SHA512_DIGEST_LENGTH) );
	//printf("FILE(%s)\n", szfile);
	
	cmemset(&head, 0, sizeof(struct _dat_node));
	
	cstrcpy(head.filedev, sha512_digest );
	cstrcpy(head.signdev, md5_digest);
	cstrcpy(head.message, message); 
	head.length = sdata->length;
	head.len_old=  MGC_SEEK1 * SSC_STREAM_BLOCK * 64;
	head.ctime  = time(NULL);
	
	fd = open(szfile, O_CREAT | O_RDWR | O_APPEND );
	if(fd != -1){
		do{
		if(write(fd, &head, sizeof(struct _dat_node)) <= 0){
			fatal("write file header", __FILE__, __LINE__, 0);
		    break;
		}
		if(write(fd, sdata->buffer, sdata->length)<=0){
			fatal("write file data", __FILE__, __LINE__, 0);
			break;
		}
		}while(0);
		close(fd);
	}
	
	return VSS_SUCCESS;	
}

void *dat_getfile(const char *file, icore_read *sdata, struct _dat_node *msg)
{
	int              fd = -1;
	
do{
	//printf(".......................\n");
	if(sdata==NULL){
		fatal("segment memory", __FILE__, __LINE__, 0);
		break;
	}
	//if(sdata->buffer==NULL){
	//	fatal("segment memory", __FILE__, __LINE__, 0);
	//	break;
	//}
	//printf(".......................\n");
	//printf("...........[%d]............\n", __LINE__);
	if(msg==NULL){
		fatal("segment memory", __FILE__, __LINE__, 0);
		break;
	}
	if(access(file, F_OK)!=0){
	   printf("%s:: initialized (%s) \n", __FUNCTION__, file );
	}
	//
	//printf("...........[%d]............\n", __LINE__);
	if(chmod(file, S_IXUSR | S_IRUSR | S_IWUSR  ) != 0){
	     fatal("chmod file", __FILE__, __LINE__, 0);	
	}
	//printf("...........[%d]............\n", __LINE__);
	fd = open(file, O_RDONLY );
	if(fd != -1){
		do{
	     //printf("...........[%d]............\n", __LINE__);		
		//printf("Loading.. \n");
		lseek (fd, 0, SEEK_END);
		lseek (fd, 0, SEEK_SET);
		//printf("...........[%d]............\n", __LINE__);
		if(read(fd, msg, sizeof(struct _dat_node)) <= 0){
			fatal("read file header", __FILE__, __LINE__, 0);
		    break;
		}
		//printf("...........[%d]............\n", __LINE__);
		#ifdef DEBUG_H
		printf("SIGN(%s)\n", sign_digest(msg->signdev, 16) );
		printf("SIGN(%s)\n", sign_digest(msg->filedev, 64) );
		printf("Length: (%d) (%d) \n", msg->length, msg->len_old);
		#endif
		sdata->length = msg->length;
		
		if(sdata->length <= 0) {
			fatal("stack memory", __FILE__, __LINE__, 0);
			//break;
		}
		if(sdata->length<=0) sdata->length = MGC_SEEK1 * SSC_STREAM_BLOCK * 2 /*64*/;
		if(sdata->length <= 0) {
			fatal("stack memory", __FILE__, __LINE__, 0);
			break;
		}
		//printf("...........[%d]............\n", __LINE__);
		sdata->buffer = (char *)cmalloc( sdata->length+1 );
		if(sdata->buffer==NULL){
		    fatal("allocate memory ", __FILE__, __LINE__, 0);
		    break;
		}
		//printf("...........[%d]............\n", __LINE__);
		lseek (fd, sizeof(struct _dat_node) , SEEK_SET);
		//printf("...........[%d]............\n", __LINE__);
		if(read(fd, sdata->buffer, sdata->length)<=0){
			fatal("read file data", __FILE__, __LINE__, 0);
			break;
		}
		//printf("...........[%d]............\n", __LINE__);
		}while(0);
		close(fd);
	}else{
		printf("file no found (*%s)\n", file );
	}
}while(0);
    //printf("...........[%d]............\n", __LINE__);
	return VSS_SUCCESS;
}

void *dat_sethdr(const char *file, struct _dat_node *head)
{
	int fd;
	//printf("file: (%s) \n", file);
do{
	printf("FILE{%s}\n", file);	
	if(strlen(file)<=0) break;
	if(chmod(file, S_IXUSR | S_IRUSR | S_IWUSR  ) != 0){
	     fatal("chmod file", __FILE__, __LINE__, 0);	
	}
	fd = open(file, O_WRONLY );
	if(fd != -1){
		do{
		lseek(fd, 0, SEEK_SET);
		if(write(fd, head, sizeof(struct _dat_node)) <= 0){
			fatal("write file header", __FILE__, __LINE__, 0);
		    break;
		}
		
		}while(0);
		close(fd);
	}
}while(0);
    return VSS_SUCCESS;
}

RUNC_DLL void *dat_remove(const char *file)
{
do{
	printf("FILE{%s}\n", file);
	if(strlen(file)<=0) break;
	if(chmod(file, S_IXUSR | S_IRUSR | S_IWUSR  ) != 0){
	     fatal("chmod file", __FILE__, __LINE__, 0);	
	}
	//unlink(file);
	if(remove(file) != 0){
		fatal("remove file", __FILE__, __LINE__, 0);
	}
}while(0);	
	return VSS_SUCCESS;
}
