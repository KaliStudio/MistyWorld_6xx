

#define LIBMPQ_CONF_FL_INCREMENT    512         /* i hope we did not need more :) */
#define LIBMPQ_CONF_EXT         ".conf"         /* listdb file seems to be valid with this extension */
#define LIBMPQ_CONF_HEADER      "LIBMPQ_VERSION"    /* listdb file must include this entry to be valid */
#define LIBMPQ_CONF_BUFSIZE     4096            /* maximum number of bytes a line in the file could contain */

#define LIBMPQ_CONF_TYPE_CHAR       1           /* value in config file is from type char */
#define LIBMPQ_CONF_TYPE_INT        2           /* value in config file is from type int */

#define LIBMPQ_CONF_EOPEN_DIR       -1          /* error on open directory */
#define LIBMPQ_CONF_EVALUE_NOT_FOUND    -2          /* value for the option was not found */

#if defined( __GNUC__ )
#include <sys/types.h>
#include <unistd.h>

#define _lseek  lseek
#define _read   read
#define _open   open
#define _write  write
#define _close  close
#define _strdup strdup

#ifndef O_BINARY
#define O_BINARY 0
#endif
#else
#include <io.h>
#endif

#ifdef O_LARGEFILE
#define MPQ_FILE_OPEN_FLAGS (O_RDONLY | O_BINARY | O_LARGEFILE)
#else
#define MPQ_FILE_OPEN_FLAGS (O_RDONLY | O_BINARY)
#endif

#ifndef min
#define min(a, b) ((a < b) ? a : b)
#endif

int libmpq_init_buffer(mpq_archive* mpq_a);
int libmpq_read_hashtable(mpq_archive* mpq_a);
int libmpq_read_blocktable(mpq_archive* mpq_a);
int libmpq_file_read_file(mpq_archive* mpq_a, mpq_file* mpq_f, unsigned int filepos, char* buffer, unsigned int toread);
int libmpq_read_listfile(mpq_archive* mpq_a, FILE* fp);

int libmpq_conf_get_value(FILE* fp, char* search_value, void* return_value, int type, int size);
char* libmpq_conf_delete_char(char* buf, char* chars);
int libmpq_conf_get_array(FILE* fp, char* search_value, char** *filelist, int* entries);
int libmpq_free_listfile(char** filelist);
int libmpq_read_listfile(mpq_archive* mpq_a, FILE* fp);

unsigned int libmpq_lseek(mpq_archive* mpq_a, unsigned int pos);
