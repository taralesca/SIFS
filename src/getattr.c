#include "../lib/getattr.h"

int sifs_getattr(const char* path, struct stat* sbuf, struct fuse_file_info* fi) {
  logger(DEBUG, "[getattr] Started on path: %s\n", path);

	struct fuse_context* context;
	context = fuse_get_context();

	struct node* root;
	root = (struct node*)context->private_data;

	struct node* n;
	n = get_node_from_path(root, path);
	if (n == NULL) {
	  logger(DEBUG, "[getattr] Ended\n");
		return -ENOENT;
	}
	logger(DEBUG, "[getattr] \tNode returned: %s\n", n->header->name);

	sbuf->st_dev 			= 0;
	sbuf->st_rdev 		= 0;
	sbuf->st_blocks 	= 0;
	sbuf->st_mode 		= ((strtoul(n->header->typeflag, NULL, 10) == 5) ? S_IFDIR : S_IFREG) | S_IRWXU  | S_IRWXG | S_IRWXO;
	sbuf->st_nlink		= n->children_size;
	sbuf->st_uid			= strtoul(n->header->uid, NULL, 10);
	sbuf->st_gid			= strtoul(n->header->gid, NULL, 10);
	sbuf->st_size			= strtol(n->header->size, NULL, 8);
	sbuf->st_blksize 	= 4096;
	sbuf->st_atime		= strtol(n->header->atime, NULL, 10);
	sbuf->st_mtime		= strtol(n->header->mtime, NULL, 10);
	sbuf->st_ctime		= strtol(n->header->ctime, NULL, 10);

  logger(DEBUG, "[getattr] Ended\n");
  return 0;
}

