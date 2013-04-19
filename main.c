/*
 * Copyright 2005 OPENXTRA Limited
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at 
 *
 * http://www.apache.org/licenses/LICENSE-2.0 
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 */
/* main.c - Entry point for application
 *
 * Author: Jack Hughes 
 * Created: Tuesday, 19 February 2005
 */

//----------------------------------------------  //-- Headers

#include "buffer.h"
#include "entry.h"
#include "entry_test.h"
#include "error.h"
#include "filter.h"
#include "filter_test.h"
#include "minunit.h"

#include "config.h"

#include <getopt.h>
#include <glob.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------------------------  //--
//----------------------------------------------  //-- Constants

#ifndef _DEBUG
#define PRIVATE static
#else
#define PRIVATE
#endif

// Max number of file specification that can be passed on the command line
#define MAX_FILE_SPECS     (10)
#define INITIAL_BUFFER_LEN (100)

//----------------------------------------------  //--
//----------------------------------------------  //-- Macros

//----------------------------------------------  //--
//----------------------------------------------  //-- Types & Classes

//----------------------------------------------  //--
//----------------------------------------------  //-- Public variables

//----------------------------------------------  //--
//----------------------------------------------  //-- Private variables

// Criteria used to decide which entries should be filtered
PRIVATE struct log_entry_filter log_filter;

// List of file specs to be filtered
PRIVATE char* file_specs[MAX_FILE_SPECS];

//----------------------------------------------  //--
//----------------------------------------------  //-- Prototypes

// Filter the given file
PRIVATE void filter_file(FILE* log_file);
PRIVATE void parse_command_line(int argc, char** argv);
PRIVATE void execute_all_tests(void);
PRIVATE char* all_tests(void);
PRIVATE void read_file_specs_from_cl(int argc, char* argv[]);
PRIVATE void filter_files(glob_t* glob);
PRIVATE void free_file_specs(void);
PRIVATE void print_version(void);
PRIVATE void filter_file_specs(void);

//----------------------------------------------  //--
//----------------------------------------------  //-- Functions

int
main(int argc, char** argv)
{
     parse_command_line(argc, argv);
     if (file_specs[0] != NULL)
     {
	  // Filter the log files as specified on the command line
	  filter_file_specs();
	  free_file_specs();
     }
     else
     {
	  // As there are no files specified on the command line, assume
	  // the log file with arrive via stdin
	  filter_file(stdin);
     }
     filter_free(&log_filter);
     return EXIT_SUCCESS;
}

PRIVATE
void
filter_file(FILE* log_file)
{
     struct log_file_entry* entry;
     char* line = NULL;
     size_t length = INITIAL_BUFFER_LEN;

     line = buffer_allocate(line, INITIAL_BUFFER_LEN + 1);

     while (getline(&line, &length, log_file) != -1)
     {
	  // If getline doesn't return -1 the buffer can't be null
	  assert(line != NULL);

	  // Read the log file line into a log file entry structure
	  entry = parse_line(line);

	  // Ignore corrupted entries
	  if (entry)
	  {
	       if (filter_entry(&log_filter, entry))
	       {
		    fputs(line, stdout);
	       }
	       free_entry(entry);
	  }
     }
     free(line);
}

PRIVATE
void
free_file_specs(void)
{
     int counter = 0;

     while (file_specs[counter] != NULL)
     {
	  free(file_specs[counter]);
	  counter++;
     }
}

PRIVATE
void
filter_file_specs(void)
{
     int counter = 0;
     int flags = 0;
     int status;
     glob_t glob_buf;

     assert(file_specs[0] != NULL);

     while (file_specs[counter] != NULL)
     {
	  status = glob(file_specs[counter], flags, NULL, &glob_buf);
	  switch (status)
	  {
	  case GLOB_NOSPACE:
	       // Out of memory error
	       exit_with_diagnostic("Ran out of memory whilst globbing...\n");
	       break;

	  case GLOB_NOMATCH:
	       // The pattern didn't match any files
	       exit_with_diagnostic("No files match file spec\n");
	       break;

	  default:
	       // Everything went ok, just carry on...
	       break;
	  }
	  flags |= GLOB_APPEND;
	  counter++;
     }
     assert(glob_buf.gl_pathc > 0);
     filter_files(&glob_buf);
     globfree(&glob_buf);
}

PRIVATE
void
filter_files(glob_t* glob)
{
     int i;
     FILE* log_file;

     for (i = 0; i < glob->gl_pathc; ++i)
     {
	  log_file = fopen(glob->gl_pathv[i], "r");
	  if (!log_file)
	  {
	       exit_with_diagnostic("Unable to open log file\n");
	  }
	  filter_file(log_file);
	  fclose(log_file);
     }
}

PRIVATE
void
usage(void)
{
     exit_with_diagnostic(
"usage: " PACKAGE_NAME " [-hiTv] [-b browser] [-c client] [-f filter(s)]\n"
"      [-I identity] [-m method] [-p protocol] [-r referer] [-s status]\n"
"      [-u uri] [-U user] [-z size] logfile [logfile...]\n"
"\n"
"    -b browser   filter for user agent (browser) string\n"
"    -c client    filter for client address\n"
"    -h           get usage message\n"
"    -i           do case-insensitive string searches\n"
"    -I identity  ?? filter on second field of log file\n"
"    -m method    filter on request method (e.g. GET, POST...)\n"
"    -p protocol  filter on HTTP protocol version field (e.g. HTTP/1.1)\n"
"    -r referer   filter on document referer string\n"
"    -s status    filter on request status value (e.g. 200, 404...)\n"
"    -T           run internal test suite\n"
"    -u uri       filter on document URI\n"
"    -U user      filter on user name used in request, if any\n"
"    -v           show program's version number\n"
"    -z size      filter on document size\n"
"\n");
}

PRIVATE
void
parse_command_line(int argc, char** argv)
{
     int choice;

     // There is always one argument, the program path stored at argv[0]
     if (argc <= 1)
     {
          usage();
     }

     // Initialise the file specification array
     memset(file_specs, 0, MAX_FILE_SPECS * sizeof(char*));

     while (((choice = getopt(argc, argv, "b:c:hiTI:m:p:r:s:tu:U:vz:")) != -1)) 
     { 
	  switch (choice)
	  { 
          case 'b':
               save_ua_filter(&log_filter, optarg);
               break;

          case 'c':
               save_client_filter(&log_filter, optarg);
               break;

	  case 'h':
	       usage();
	       break;

	  case 'i':
	       // Perform case insensitive matches
	       case_sensitive = 0;
	       break;

          case 'I':
               save_identity_filter(&log_filter, optarg);
               break;

	  case 'm':
	       save_method_filter(&log_filter, optarg);
	       break;

	  case 'p':
	       save_protocol_filter(&log_filter, optarg);
	       break;

	  case 'r':
	       save_referer_filter(&log_filter, optarg);
	       break;

	  case 's':
	       save_status_filter(&log_filter, optarg);
	       break;

	  case 'T':
	       execute_all_tests();
	       break;

          case 'u':
               save_uri_filter(&log_filter, optarg);
               break;

          case 'U':
               save_user_id_filter(&log_filter, optarg);
               break;

	  case 'v':
	       print_version();
	       break;

          case 'z':
               save_size_filter(&log_filter, optarg);
               break;

	  default:
	       // An unknown option has been used...quit with an appropriate diagnostic
               usage();
	       exit_with_diagnostic("\nUnknown command line option");
	       break;
	  }
     }
     read_file_specs_from_cl(argc, argv);
}

PRIVATE
void
read_file_specs_from_cl(int argc, char* argv[])
{
     int cl_counter;
     int file_spec_counter = 0;
     char* file_spec;

     assert(file_specs[0] == NULL);

     for (cl_counter = optind; cl_counter < argc; ++cl_counter)
     {
	  file_spec = malloc(strlen(argv[cl_counter]) + 1);
	  if (!file_spec)
	  {
	       exit_with_diagnostic("Failed to allocate buffer for file spec");
	  }
	  strcpy(file_spec, argv[cl_counter]);
	  file_specs[file_spec_counter++] = file_spec;
     }
}

PRIVATE
void
print_version(void)
{
     printf("%s version %s\n", PACKAGE_NAME, VERSION);
     exit(EXIT_SUCCESS);
}

PRIVATE
char*
all_tests(void)
{
     mu_run_test(entry_all_tests);
     mu_run_test(filter_all_tests);
     return 0;
}

PRIVATE
void
execute_all_tests(void)
{
     int exit_code = EXIT_SUCCESS;
     char *result;
	
     result = all_tests();

     if (result != 0)
     {
	  printf("%s\n", result);
	  exit_code = EXIT_FAILURE;
     }
     else
     {
	  printf("ALL TESTS PASSED\n");
     }
     printf("Tests run: %d\n", tests_run);
     exit(exit_code);
}
