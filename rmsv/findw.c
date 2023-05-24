
#include "rmsvw.h"


uint32_t _vsc_getfilecount(const char *dir)
{
	intptr_t  handle=-1;
	struct _cfinddata_t  fileinfo;
	char      szfile[MAX_PATH];
	uint32_t  count=0;
	
	sprintf(szfile, "%s/*.dat\0", dir);
	//printf("FILE::(%s)\n", szfile);
	handle = _cfindfirst(szfile, &fileinfo);
	do{
		if(handle <= 0) break;
		sprintf(szfile, "%s/%s", dir, fileinfo.name);
		count+= 1;
		//printf("%s::%s \n", __FUNCTION__, szfile );
	}while(_cfindnext(handle, &fileinfo)>0 );
	_cfindclose(handle);
	
	return count;
}


intptr_t _vsc_firstfile(const char *dir, char *file)
{
	intptr_t handle = -1;
	struct _cfinddata_t  fileinfo;
	char      szfile[MAX_PATH];
	sprintf(szfile, "%s/*.dat\0", dir);
	handle = _cfindfirst(szfile, &fileinfo);
	if(handle !=-1){
		cstrcpy(file, fileinfo.name);
	}
	
	return handle;
}
uint32_t _vsc_nextfile(intptr_t handle, char *szfile)
{
	uint32_t  ret = 0;
	struct _cfinddata_t  fileinfo;
	
	do{
	 if(handle == -1 || handle == 0){
	 	fatal("stack memory ", __FILE__, __LINE__, 0);
	 	break;
	 }
	 if(szfile==NULL){
	 	fatal("stack memory ", __FILE__, __LINE__, 0);
	 	break;
	 }
	 
	 //printf("##############[%d]##############\n", __LINE__);
	 if(_cfindnext(handle, (struct _cfinddata_t *) &fileinfo)){
	 	//printf("##############[%d]##############\n", __LINE__);
	    cstrcpy(szfile, fileinfo.name);
	    //printf("##############[%d]##############\n", __LINE__);
	    ret= 1;	
	 } 
	}while(0);
	return ret;
}
uint32_t _vsc_closefile(intptr_t handle)
{
  uint32_t  ret = 0;
  
  do{
	if(_cfindclose(handle)<=0){
		#ifdef DEBUG_H
		fatal("stack memory", __FILE__, __LINE__, 0);
		#endif
		break;
	}
  }while(0);
    handle = -1;
  
	return ret;
}

#ifdef WIN32_H

intptr_t _cfindfirst(const char* filespec, struct _cfinddata_t* fileinfo) {
   struct _finddata_t fileinfow;
   intptr_t  handle = _findfirst(filespec, (struct _finddata_t*)&fileinfow );
   
   cstrcpy(fileinfo->name, fileinfow.name );
   fileinfo->attrib = fileinfow.attrib;
   fileinfo->time_create = fileinfow.time_create;
   fileinfo->time_access = fileinfow.time_access;
   fileinfo->time_write  = fileinfow.time_write;
   fileinfo->size        = fileinfow.size;
    
   return handle;
}
int _cfindnext(intptr_t fhandle, struct _cfinddata_t* fileinfo) {
   struct _finddata_t fileinfow;
   int                i=0;
   //printf("##############[%d]##############\n", __LINE__);
 do{
   if(fhandle == -1 || fhandle == 0) break;
   i= _findnext(fhandle, (struct _finddata_t*)&fileinfow);
   //printf("##############[%d]##############\n", __LINE__);
   if(i==-1) return 0;
   
   cstrcpy(fileinfo->name, fileinfow.name );
   fileinfo->attrib = fileinfow.attrib;
   fileinfo->time_create = fileinfow.time_create;
   fileinfo->time_access = fileinfow.time_access;
   fileinfo->time_write  = fileinfow.time_write;
   fileinfo->size        = fileinfow.size;
 }while(0);
   
   return 1;
}
int _cfindclose(intptr_t fhandle) {
   return _findclose(fhandle);
}
#else

#define DOTDOT_HANDLE    0L
#define INVALID_HANDLE  -1L

int _match_spec(const char* spec, const char* text) {
    /*
     * If the whole specification string was consumed and
     * the input text is also exhausted: it's a match.
     */
    if (spec[0] == '\0' && text[0] == '\0') {
        return 1;
    }

    /* A star matches 0 or more characters. */
    if (spec[0] == '*') {
        /*
         * Skip the star and try to find a match after it
         * by successively incrementing the text pointer.
         */
        do {
            if (_match_spec(spec + 1, text)) {
                return 1;
            }
        } while (*text++ != '\0');
    }

    /*
     * An interrogation mark matches any character. Other
     * characters match themself. Also, if the input text
     * is exhausted but the specification isn't, there is
     * no match.
     */
    if (text[0] != '\0' && (spec[0] == '?' || spec[0] == text[0])) {
        return _match_spec(spec + 1, text + 1);
    }

    return 0;
}

int match_spec(const char* spec, const char* text) {
    /* On Windows, *.* matches everything. */
    if (strcmp(spec, "*.*") == 0) {
        return 1;
    }

    return _match_spec(spec, text);
}


typedef struct fhandle_t {
    DIR* dstream;
    short dironly;
    char* spec;
} fhandle_t;

void fill_finddata(struct stat* st, const char* name,
        struct _cfinddata_t* fileinfo);

intptr_t findfirst_dotdot(const char* filespec,
        struct _cfinddata_t* fileinfo);

intptr_t findfirst_in_directory(const char* dirpath,
        const char* spec, struct _cfinddata_t* fileinfo);

void findfirst_set_errno();

intptr_t _cfindfirst(const char* filespec, struct _cfinddata_t* fileinfo) {
    char* rmslash;      /* Rightmost forward slash in filespec. */
    const char* spec;   /* Specification string. */

    if (!fileinfo || !filespec) {
        errno = EINVAL;
        return INVALID_HANDLE;
    }

    if (filespec[0] == '\0') {
        errno = ENOENT;
        return INVALID_HANDLE;
    }

    rmslash = strrchr(filespec, '/');

    if (rmslash != NULL) {
        /*
         * At least one forward slash was found in the filespec
         * string, and rmslash points to the rightmost one. The
         * specification part, if any, begins right after it.
         */
        spec = rmslash + 1;
    } else {
        /*
         * Since no slash was found in the filespec string, its
         * entire content can be used as our spec string.
         */
        spec = filespec;
    }

    if (strcmp(spec, ".") == 0 || strcmp(spec, "..") == 0) {
        /* On Windows, . and .. must return canonicalized names. */
        return findfirst_dotdot(filespec, fileinfo);
    } else if (rmslash == filespec) {
        /*
         * Since the rightmost slash is the first character, we're
         * looking for something located at the file system's root.
         */
        return findfirst_in_directory("/", spec, fileinfo);
    } else if (rmslash != NULL) {
        /*
         * Since the rightmost slash isn't the first one, we're
         * looking for something located in a specific folder. In
         * order to open this folder, we split the folder path from
         * the specification part by overwriting the rightmost
         * forward slash.
         */
        size_t pathlen = strlen(filespec) +1;
        char* dirpath = alloca(pathlen);
        memcpy(dirpath, filespec, pathlen);
        dirpath[rmslash - filespec] = '\0';
        return findfirst_in_directory(dirpath, spec, fileinfo);
    } else {
        /*
         * Since the filespec doesn't contain any forward slash,
         * we're looking for something located in the current
         * directory.
         */
        return findfirst_in_directory(".", spec, fileinfo);
    }
}

/* Perfom a scan in the directory identified by dirpath. */
intptr_t findfirst_in_directory(const char* dirpath,
        const char* spec, struct _cfinddata_t* fileinfo) {
    DIR* dstream;
    fhandle_t* ffhandle;

    if (spec[0] == '\0') {
        errno = ENOENT;
        return INVALID_HANDLE;
    }

    if ((dstream = opendir(dirpath)) == NULL) {
        findfirst_set_errno();
        return INVALID_HANDLE;
    }

    if ((ffhandle = cmalloc(sizeof(fhandle_t))) == NULL) {
        closedir(dstream);
        errno = ENOMEM;
        return INVALID_HANDLE;
    }

    /* On Windows, *. returns only directories. */
    ffhandle->dironly = strcmp(spec, "*.") == 0 ? 1 : 0;
    ffhandle->dstream = dstream;
    ffhandle->spec = strdup(spec);

    if (_cfindnext((intptr_t) ffhandle, fileinfo) != 0) {
        _cfindclose((intptr_t) ffhandle);
        errno = ENOENT;
        return INVALID_HANDLE;
    }

    return (intptr_t) ffhandle;
}

/* On Windows, . and .. return canonicalized directory names. */
intptr_t findfirst_dotdot(const char* filespec,
        struct _cfinddata_t* fileinfo) {
    char* dirname;
    char* canonicalized;
    struct stat st;

    if (stat(filespec, &st) != 0) {
        findfirst_set_errno();
        return INVALID_HANDLE;
    }

    /* Resolve filespec to an absolute path. */
    if ((canonicalized = realpath(filespec, NULL)) == NULL) {
        findfirst_set_errno();
        return INVALID_HANDLE;
    }

    /* Retrieve the basename from it. */
    dirname = basename(canonicalized);

    /* Make sure that we actually have a basename. */
    if (dirname[0] == '\0') {
        free(canonicalized);
        errno = ENOENT;
        return INVALID_HANDLE;
    }

    /* Make sure that we won't overflow finddata_t::name. */
    if (strlen(dirname) > 259) {
        free(canonicalized);
        errno = ENOMEM;
        return INVALID_HANDLE;
    }

    fill_finddata(&st, dirname, fileinfo);

    free(canonicalized);

    /*
     * Return a special handle since we can't return
     * NULL. The findnext and findclose functions know
     * about this custom handle.
     */
    return DOTDOT_HANDLE;
}

/*
 * Windows implementation of _findfirst either returns EINVAL,
 * ENOENT or ENOMEM. This function makes sure that the above
 * implementation doesn't return anything else when an error
 * condition is encountered.
 */
void findfirst_set_errno() {
    if (errno != ENOENT &&
        errno != ENOMEM &&
        errno != EINVAL) {
        errno = EINVAL;
    }
}

void fill_finddata(struct stat* st, const char* name,
        struct _cfinddata_t* fileinfo) {
    fileinfo->attrib = S_ISDIR(st->st_mode) ? _A_SUBDIR : _A_NORMAL;
    fileinfo->size = st->st_size;
    fileinfo->time_create = st->st_ctime;
    fileinfo->time_access = st->st_atime;
    fileinfo->time_write = st->st_mtime;
    strcpy(fileinfo->name, name);
}

int _cfindnext(intptr_t fhandle, struct _cfinddata_t* fileinfo) {
    struct dirent entry, *result;
    struct fhandle_t* handle;
    struct stat st;

    if (fhandle == DOTDOT_HANDLE) {
        errno = ENOENT;
        return -1;
    }

    if (fhandle == INVALID_HANDLE || !fileinfo) {
        errno = EINVAL;
        return -1;
    }

    handle = (struct fhandle_t*) fhandle;

    while (readdir_r(handle->dstream, &entry, &result) == 0 && result != NULL) {
        if (!handle->dironly && !match_spec(handle->spec, entry.d_name)) {
            continue;
        }

        if (fstatat(dirfd(handle->dstream), entry.d_name, &st, 0) == -1) {
            return -1;
        }

        if (handle->dironly && !S_ISDIR(st.st_mode)) {
            continue;
        }

        fill_finddata(&st, entry.d_name, fileinfo);

        return 0;
    }

    errno = ENOENT;
    return -1;
}

int _cfindclose(intptr_t fhandle) {
    struct fhandle_t* handle;

    if (fhandle == DOTDOT_HANDLE) {
        return 0;
    }

    if (fhandle == INVALID_HANDLE) {
        errno = ENOENT;
        return -1;
    }

    handle = (struct fhandle_t*) fhandle;

    closedir(handle->dstream);
    free(handle->spec);
    free(handle);

    return 0;
}
#endif

