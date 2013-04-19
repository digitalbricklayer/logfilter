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
/* filter.h - Definitions for filtering log entries
 *
 * Author: Jack Hughes
 * Written: Thursday, 28 April 2005
 */

#ifndef FILTER_H
#define FILTER_H   (1)

//----------------------------------------------  //-- Headers

#include "entry.h"
#include <pcre/pcreposix.h>

//----------------------------------------------  //--
//----------------------------------------------  //-- Constants

// Max size of a filter
#define MAX_FILTER_LEN		(1024)

// Values for error_code flag
#define PARSE_OK	       	(0)
#define	PARSE_UNKNOWN_FILTER	(1)
#define PARSE_BAD_RE            (2)

//----------------------------------------------  //--
//----------------------------------------------  //-- Macros

//----------------------------------------------  //--
//----------------------------------------------  //-- Typedefs

struct log_entry_filter
{
     char*	client;
     regex_t    client_re;

     char*	identity;
     regex_t    identity_re;

     char*	user_id;
     regex_t    user_id_re;

     char*	time;
     regex_t    time_re;

     char*	referer;
     regex_t    referer_re;

     char*	request_uri;
     regex_t    request_uri_re;

     char*	request_method;
     regex_t    request_method_re;

     char*	request_protocol;
     regex_t    request_protocol_re;

     char*	status_code;
     regex_t    status_code_re;

     char*	size;
     regex_t    size_re;

     char*	ua;
     regex_t    ua_re;
};

//----------------------------------------------  //--
//----------------------------------------------  //-- Public variables

// If 1 match case when performing matches (on strings)
extern int case_sensitive;

// Error code set during filter argument parsing
extern int error_code;

//----------------------------------------------  //--
//----------------------------------------------  //-- Prototypes

// Save individual filter flags' arguments to the log_entry_filter
// structure.
extern int save_client_filter(struct log_entry_filter* filter, char* arg);
extern int save_identity_filter(struct log_entry_filter* filter, char* arg);
extern int save_user_id_filter(struct log_entry_filter* filter, char* arg);
extern int save_time_filter(struct log_entry_filter* filter, char* arg);
extern int save_uri_filter(struct log_entry_filter* filter, char* arg);
extern int save_referer_filter(struct log_entry_filter* filter, char* arg);
extern int save_method_filter(struct log_entry_filter* filter, char* arg);
extern int save_protocol_filter(struct log_entry_filter* filter, char* arg);
extern int save_status_filter(struct log_entry_filter* filter, char* arg);
extern int save_size_filter(struct log_entry_filter* filter, char* arg);
extern int save_ua_filter(struct log_entry_filter* filter, char* arg);

// Return 1 if the given record matches the search criteria, 0 otherwise
extern int filter_entry(struct log_entry_filter* filter, const struct log_file_entry* entry);

// Free the filter structure
extern void filter_free(struct log_entry_filter* filter);

//----------------------------------------------  //--

#endif  /* FILTER_H */
