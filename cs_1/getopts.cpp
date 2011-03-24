#include <stdio.h>
#include <string>

#include "types.h"

void print_help();

int getopts(opts_t * opts, int argc, char ** argv) {
	opts->mode = NONE;	

	if(1 == argc) {
		// there are no args, use default config
		opts->mode = HASH;
		opts->path = "C:\\Windows";
	} else {
		bool mode_specified = false;
		for(int i = 1; i < argc; i++) {
			if(strcmp(argv[i], "-h") == 0) {
				if(mode_specified) {
					printf("Error: mode is already specified\n");
					return -1;
				}

				#ifdef __DEBUG__
					printf("Flag -h find, hash mode\n");
				#endif
				
				mode_specified = true;
				opts->mode = HASH;
			} else if(strcmp(argv[i], "-s") == 0) {
				if(mode_specified) {
					printf("Error: mode is already specified\n");
					return -1;
				}

				#ifdef __DEBUG__
					printf("Flag -h find, hash mode");
				#endif

				mode_specified = true;
				opts->mode = SIGNATURE;
			} else if(strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
				printf("PATH!!!!");
			} else {
				printf("Error: Unknown flag %s\n", argv[i]);
				print_help();
				return -1;
			}
		} /* argv iter */
	} /* if argc != 1 */
	
	// check configuration
	if(opts->mode == NONE) {
		printf("Mode isn't specified\n");
		print_help();
		return -1;
	}

	if(opts->path.length() == 0) {
		printf("Path isn't specified\n");
		print_help();
		return -1;
	}

	printf("mode: %s\n", opts->mode == HASH ? "hash" : (opts->mode == SIGNATURE ? "signature" : "unknown"));
	printf("path: %s\n", opts->path.c_str());

	return 0;
} /* getopts */

void print_help() {
	printf("Use flags:\n\t-h\thash mode\n\t-s\tsignature mode\n\t-p PATH\tpath\n\n");
}
