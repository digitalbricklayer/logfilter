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
/* entry.c - Functions for manipulating a log entry
 *
 * Author: Jack Hughes
 * Created: Tuesday, 24 March 2005
 */

//----------------------------------------------  //-- Headers

#include "entry.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>
#include "buffer.h"

//----------------------------------------------  //--
//----------------------------------------------  //-- Constants

#ifndef _DEBUG
#define PRIVATE static
#else
#define PRIVATE
#endif

#define INITIAL_BUFFER_LEN		(100)
#define BUFFER_INCREMENT		(10)

//----------------------------------------------  //--
//----------------------------------------------  //-- Macros

//----------------------------------------------  //--
//----------------------------------------------  //-- Types & Classes

//----------------------------------------------  //--
//----------------------------------------------  //-- Public variables

//----------------------------------------------  //--
//----------------------------------------------  //-- Private variables

// Field seperator character
char field_seperator = ' ';

//----------------------------------------------  //--
//----------------------------------------------  //-- Prototypes

PRIVATE int parse_client_field(struct log_file_entry* record, const char* line, int* position);
PRIVATE int parse_identity_field(struct log_file_entry* record, const char* line, int* position);
PRIVATE int parse_user_id_field(struct log_file_entry* record, const char* line, int* position);
PRIVATE int parse_time_field(struct log_file_entry* record, const char* line, int* position);
PRIVATE int parse_request_field(struct log_file_entry* record, const char* line, int* position);
PRIVATE int parse_status_code_field(struct log_file_entry* record, const char* line, int* position);
PRIVATE int parse_size_field(struct log_file_entry* record, const char* line, int* position);
PRIVATE int parse_referer_field(struct log_file_entry* record, const char* line, int* position);
PRIVATE int parse_user_agent_field(struct log_file_entry* record, const char* line, int* position);

PRIVATE char* parse_field(const char* line, int* position, const char field_delimiter);
PRIVATE char* parse_field_with_delimeters(const char* line, int* line_position, const char field_delimiter_front, const char field_delimiter_end);

//----------------------------------------------  //--
//----------------------------------------------  //-- Functions

struct log_file_entry*
parse_line(const char* line)
{
     struct log_file_entry* entry;
     int current_position = 0;		// Position counter for the line

     // Create a new record for this line
     entry = entry_malloc();

     // Parse all of the fields
     if (!parse_client_field(entry, line, &current_position))
     {
	  // Bad entry, skip this one
	  free_entry(entry);
	  return NULL;
     }

     if (!parse_identity_field(entry, line, &current_position))
     {
	  // Bad entry, skip this one
	  free_entry(entry);
	  return NULL;
     }
	
     if (!parse_user_id_field(entry, line, &current_position))
     {
	  // Bad entry, skip this one
	  free_entry(entry);
	  return NULL;
     }
	
     if (!parse_time_field(entry, line, &current_position))
     {
	  // Bad entry, skip this one
	  free_entry(entry);
	  return NULL;
     }
	
     if (!parse_request_field(entry, line, &current_position))
     {
	  // Bad entry, skip this one
	  free_entry(entry);
	  return NULL;
     }
	
     if (!parse_status_code_field(entry, line, &current_position))
     {
	  // Bad entry, skip this one
	  free_entry(entry);
	  return NULL;
     }
	
     if (!parse_size_field(entry, line, &current_position))
     {
	  // Bad entry, skip this one
	  free_entry(entry);
	  return NULL;
     }
	
     if (!parse_referer_field(entry, line, &current_position))
     {
	  // Bad entry, skip this one
	  free_entry(entry);
	  return NULL;
     }
	
     if (!parse_user_agent_field(entry, line, &current_position))
     {
	  // Bad entry, skip this one
	  free_entry(entry);
	  return NULL;
     }

     // Return the newly created entry, it is the caller's responsibility to 
     // clean it up
     return entry;
}

struct log_file_entry*
entry_malloc(void)
{
     struct log_file_entry* entry;

     entry = malloc(sizeof(struct log_file_entry));

     if (!entry)
     {
	  // ERROR: unable to allocate buffer
	  fprintf(stderr, "Fatal error: malloc failed, out of memory");
	  exit(EXIT_FAILURE);
     }

     memset(entry, 0, sizeof(struct log_file_entry));

     return entry;
}

void
free_entry(struct log_file_entry* entry)
{
     assert(entry != NULL);
     // Free all of the log entry fields
     free(entry->client);
     free(entry->identity);
     free(entry->user_id);
     free(entry->time);
     free(entry->referer);
     free(entry->request_uri);
     free(entry->request_method);
     free(entry->request_protocol);
     free(entry->status_code);
     free(entry->size);
     free(entry->ua);
     // Free the log entry last
     free(entry);
}

PRIVATE
int
parse_client_field(struct log_file_entry* record, const char* line, int* position)
{
     char* client;

     assert(record != NULL);
     assert(line != NULL);
     assert(position != NULL);

     client = parse_field(line, position, field_seperator);

     if (client)
     {
	  record->client = client;
     }

     return (client)? 1: 0;
}

PRIVATE
int
parse_identity_field(struct log_file_entry* record, const char* line, int* position)
{
     char* identity;

     assert(record != NULL);
     assert(line != NULL);
     assert(position != NULL);

     identity = parse_field(line, position, field_seperator);

     if (identity)
     {
	  record->identity = identity;
     }

     return (identity)? 1: 0;
}

PRIVATE
int
parse_user_id_field(struct log_file_entry* record, const char* line, int* position)
{
     char* user_id;

     assert(record != NULL);
     assert(line != NULL);
     assert(position != NULL);

     user_id = parse_field(line, position, field_seperator);

     if (user_id)
     {
	  record->user_id = user_id;
     }

     return (user_id)? 1: 0;
}

PRIVATE
int
parse_time_field(struct log_file_entry* record, const char* line, int* position)
{
     char* time;

     assert(record != NULL);
     assert(line != NULL);
     assert(position != NULL);

     // Get the raw time from the raw line
     time = parse_field_with_delimeters(line, position, '[', ']');

     if (!time)
     {
	  // Bad field, unable to parse time field
	  return 0;
     }

     record->time = time;

     return 1;
}

PRIVATE
int
parse_request_field(struct log_file_entry* record, const char* line, int* position)
{
     char* request;
     char* method;
     char* uri;
     char* protocol;
     int field_position = 0;

     assert(record != NULL);
     assert(line != NULL);
     assert(position != NULL);

     // Get the raw request from the raw line
     request = parse_field_with_delimeters(line, position, '"', '"');

     if (!request)
     {
	  // ERROR: the field must be corrupt
	  return 0;
     }

     // Get the request method from the field
     method = parse_field(request, &field_position, ' ');

     if (!method)
     {
	  // ERROR: the method field is corrupt
	  return 0;
     }

     // Get the request URI from the field
     uri = parse_field(request, &field_position, ' ');

     if (!uri)
     {
	  // ERROR: the URI field is corrupt
	  free(method);
	  return 0;
     }

     // Get the request protocol from the field, the protocol is optional
     protocol = parse_field(request, &field_position, '\0');

     // Save the fields into the record
     record->request_method = method;
     record->request_uri = uri;
     record->request_protocol = protocol;

     return method && uri;
}

PRIVATE
int
parse_status_code_field(struct log_file_entry* record, const char* line, int* position)
{
     char* status_code;

     assert(record != NULL);
     assert(line != NULL);
     assert(position != NULL);

     status_code = parse_field(line, position, field_seperator);

     if (status_code)
     {
	  record->status_code = status_code;
     }

     return (status_code)? 1: 0;
}

PRIVATE
int
parse_size_field(struct log_file_entry* record, const char* line, int* position)
{
     char* size_code;

     assert(record != NULL);
     assert(line != NULL);
     assert(position != NULL);

     size_code = parse_field(line, position, field_seperator);

     if (size_code)
     {
	  record->size = size_code;
     }

     return 1;
}

PRIVATE
int
parse_referer_field(struct log_file_entry* record, const char* line, int* position)
{
     char* referer;

     assert(record != NULL);
     assert(line != NULL);
     assert(position != NULL);

     // Get the referer from the line
     referer = parse_field_with_delimeters(line, position, '"', '"');

     if (!referer)
     {
	  // ERROR: the field must be corrupt
	  return 0;
     }

     // Save the referer into the record
     record->referer = referer;

     return 1;
}

PRIVATE
int
parse_user_agent_field(struct log_file_entry* record, const char* line, int* position)
{
     char* user_agent;

     assert(record != NULL);
     assert(line != NULL);
     assert(position != NULL);

     // Get the user agent from the line
     user_agent = parse_field_with_delimeters(line, position, '"', '"');

     if (!user_agent)
     {
	  // ERROR: the field must be corrupt
	  return 0;
     }

     // Save the user agent into the record
     record->ua = user_agent;

     return 1;
}

PRIVATE
char*
parse_field(const char* line, int* line_position, const char field_delimiter)
{
     int buffer_len = INITIAL_BUFFER_LEN;// Length of buffer minus null terminator
     int line_counter = *line_position;	// counter for the line buffer		
     int field_counter = 0;				// counter for the field buffer
     char* field = NULL;

     // Allocate an initial field buffer
     field = buffer_allocate(field, buffer_len + 1);

     while (line[line_counter] != '\0')
     {
	  if (line[line_counter] == field_delimiter)
	  {
	       // We've reached the end of the field, eat the delimeter
	       line_counter++;
	       break;
	  }
	  if (field_counter + 1 == buffer_len)
	  {
	       // The field is longer than the current buffer, allocate some more
	       buffer_len += BUFFER_INCREMENT + 1;
	       field = buffer_allocate(field, buffer_len);
	  }
	  field[field_counter++] = line[line_counter++];
     }

     // Check to make sure that the field isn't empty
     if (field_counter == 0)
     {
	  return NULL;
     }

     // Append the null terminator to the field
     field[field_counter] = '\0';

     // Save the new line position
     *line_position = line_counter;

     return field;
}

PRIVATE
char*
parse_field_with_delimeters(const char* line, int* line_position, const char field_delimiter_front, const char field_delimiter_end)
{
     char* field;
     int line_counter = *line_position;

     // Make sure the front delimeter is in place & move past it
     if (line[line_counter++] != field_delimiter_front)
     {
	  // ERROR: missing front delimeter
	  return NULL;
     }

     // We are now past the front delimeter, read to the end delimeter
     field = parse_field(line, &line_counter, field_delimiter_end);

     if (!field)
     {
	  // ERROR: unable to parse the field
	  return NULL;
     }

     // A blank delimiter means that this field must be the last in the line, 
     // consequently, there is little point in aligning the counter to the
     // next field
     if (field_seperator != '\0')
     {
	  // Eat the field seperator ready for the next field
	  line_counter++;
     }

     // Save the line counter
     *line_position = line_counter;

     return field;
}
