
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ARG_PRINT  0x1
#define ARG_SUMMARY   0x2

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
            case '?': // TODO why is this needed?
            default:
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
}

void summaryMode (char * filename)
{
}

int main (int argc, char *argv [])
{
    parseargs (argc,argv);

    // Check that a filename was provided
    if (filename == NULL || filename == "")
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

