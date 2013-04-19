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
/* entry.h - Definitions for log file entries
 *
 * Author: Jack Hughes
 * Written: Monday, 24 March 2005
 */

#ifndef ENTRY_H
#define ENTRY_H   (1)

//----------------------------------------------  //-- Headers

//----------------------------------------------  //--
//----------------------------------------------  //-- Constants

//----------------------------------------------  //--
//----------------------------------------------  //-- Macros

//----------------------------------------------  //--
//----------------------------------------------  //-- Typedefs

struct log_file_entry
{
     char*	client;
     char*	identity;
     char*	user_id;
     char*	time;
     char*	referer;
     char*	request_uri;
     char*	request_method;
     char*	request_protocol;
     char*	status_code;
     char*	size;
     char*	ua;
};

//----------------------------------------------  //--
//----------------------------------------------  //-- Public variables

// Field seperator character
extern char field_seperator;

//----------------------------------------------  //--
//----------------------------------------------  //-- Prototypes

// Parse a single record from an apache log into a log file entry record
extern struct log_file_entry* parse_line(const char* line);

// Allocate a new entry record
extern struct log_file_entry* entry_malloc(void);

// Free the buffers used by the given apache log record
extern void free_entry(struct log_file_entry* entry);

//----------------------------------------------  //--

#endif  /* ENTRY_H */
