
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ARG_PRINT  0x1
#define ARG_SUMMARY   0x2
#define ADDR_LEN 4  // IPv4 address length in bytes

unsigned short cmd_line_flags = 0;
char * filename = NULL;

void usage (char *progname)
{
    fprintf (stderr,"%s [-p] [-s] -r filename]\n", progname);
    fprintf (stderr,"   -p    operate in print mode\n");
    fprintf (stderr,"   -s    operate in summary mode\n");
    fprintf (stderr,"   -r <filename>  require a filename \'X\'\n");
    exit (1);
}

void parseargs (int argc, char *argv [])
{
    int opt;

    while ((opt = getopt (argc, argv, "psr:")) != -1)
    {
        switch (opt)
        {
            case 'p':
              cmd_line_flags |= ARG_PRINT;
              break;
            case 's':
              cmd_line_flags |= ARG_SUMMARY;
              break;
            case 'r':
                filename = optarg;
                break;
            default:
                fprintf (stderr,"error: unknown option -%c\n",opt);
                usage (argv [0]);
        }
    }
    if (cmd_line_flags == 0)
    {
        fprintf (stderr,"error: Either -p or -s must be specified\n");
        usage (argv [0]);
    }
}

void printMode (char * filename)
{
    unsigned char buffer[ADDR_LEN]; 
    FILE * file = fopen(filename, "rb");

    if (file == NULL) {
        fprintf(stderr, "error: could not open file %s\n", filename);
        exit(1);
    }
    // Read and process the file in print mode
    while (fread(&buffer, sizeof(buffer), 1, file) == 1) {
        // Extract quads
        printf("%u.%u.%u.%u\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    }
    fclose(file);
}

// Summary mode reports the total number of IPv4 addresses in the input file, as well as the number of addresses from the
// 10.0.0.0/8 private address range. The output will consist of two lines, as follows:
// total IPs: X
// private IPs: Y
// The “X” is the total number of IPv4 addresses in the file. The “Y” is the number from the 10.0.0.0/8 address
// range—i.e., IPv4 addresses whose first quad is 10
void summaryMode (char * filename)
{
    unsigned char buffer[ADDR_LEN];
    FILE * file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "error: could not open file %s\n", filename);
        exit(1);
    }
    int total_ips = 0;
    int private_ips = 0;
    while (fread(&buffer, sizeof(buffer), 1, file) == 1) {
        total_ips++;
        if (buffer[0] == 10) {
            private_ips++;
        }
    }
    fclose(file);
    printf("total IPs: %d\n", total_ips);
    printf("private IPs: %d\n", private_ips);
}

int main (int argc, char *argv [])
{
    parseargs (argc,argv);

    // Check that a filename was provided
    if (filename == NULL)
    {
        fprintf (stderr,"error: no filename provided\n");
        usage (argv [0]);
    }

    // Prevent both options being set
    if (cmd_line_flags == (ARG_PRINT | ARG_SUMMARY))
    {
        fprintf (stderr,"error: either -p or -s must be specified but not both.\n");
        usage (argv [0]);
    }

    // Call the appropriate function
    if (cmd_line_flags & ARG_PRINT) {
        printMode (filename);
    }
    else if (cmd_line_flags & ARG_SUMMARY) {
        summaryMode (filename);
    } else {
        fprintf (stderr,"error: either -p or -s must be specified but not both. -r <filename> is required\n");
        exit (1);
    }   
    exit (0);
}

