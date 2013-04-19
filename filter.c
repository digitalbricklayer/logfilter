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
/* filter.c - Functions for filtering log entries
 *
 * Author: Jack Hughes
 * Created: Thursday, 28 April 2005
 */

//----------------------------------------------  //-- Headers

#include "filter.h"

#include "buffer.h"
#include "error.h"

#include <assert.h>
#include <ctype.h>
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

// Return code from save_<filter name>__filter()
#define PARSE_FAILURE		(0)
#define PARSE_SUCCESS		(1)

//----------------------------------------------  //--
//----------------------------------------------  //-- Macros

//----------------------------------------------  //--
//----------------------------------------------  //-- Types & Classes

//----------------------------------------------  //--
//----------------------------------------------  //-- Public variables

// Set to 1 to perform case sensitive matches, 0 for case insensitive 
// matches. By default case sensitive matches are performed.
int case_sensitive = 1;

//----------------------------------------------  //--
//----------------------------------------------  //-- Private variables

//----------------------------------------------  //--
//----------------------------------------------  //-- Prototypes

PRIVATE int filter_match(regex_t* filter, const char* field);
PRIVATE int compile_re(regex_t* re, const char* filter_str);

//----------------------------------------------  //--
//----------------------------------------------  //-- Functions

int
filter_entry(struct log_entry_filter* filter, const struct log_file_entry* entry)
{
     if (filter)
     {
	  if (filter->client)
	  {
	       if (!filter_match(&filter->client_re, entry->client))
	       {
		    return 0;
	       }
	  }
	  if (filter->identity)
	  {
	       if (!filter_match(&filter->identity_re, entry->identity))
	       {
		    return 0;
	       }
	  }
	  if (filter->time)
	  {
	       if (!filter_match(&filter->time_re, entry->time))
	       {
		    return 0;
	       }
	  }
	  if (filter->user_id)
	  {
	       if (!filter_match(&filter->user_id_re, entry->user_id))
	       {
		    return 0;
	       }
	  }
	  if (filter->referer)
	  {
	       if (!filter_match(&filter->referer_re, entry->referer))
	       {
		    return 0;
	       }
	  }
	  if (filter->request_uri)
	  {
	       if (!filter_match(&filter->request_uri_re, entry->request_uri))
	       {
		    return 0;
	       }
	  }
	  if (filter->request_method)
	  {
	       if (!filter_match(&filter->request_method_re, entry->request_method))
	       {
		    return 0;
	       }
	  }
	  if (filter->request_protocol)
	  {
	       if (!filter_match(&filter->request_protocol_re, entry->request_protocol))
	       {
		    return 0;
	       }
	  }
	  if (filter->status_code)
	  {
	       if (!filter_match(&filter->status_code_re, entry->status_code))
	       {
		    return 0;
	       }
	  }
	  if (filter->size)
	  {
	       if (!filter_match(&filter->size_re, entry->size))
	       {
		    return 0;
	       }
	  }
	  if (filter->ua)
	  {
	       if (!filter_match(&filter->ua_re, entry->ua))
	       {
		    return 0;
	       }
	  }
     }
     return 1;
}

void
filter_free(struct log_entry_filter* filter)
{
     assert(filter);

     if (filter->client)
     {
	  regfree(&filter->client_re);
	  free(filter->client);
     }
     
     if (filter->identity)
     {
	  regfree(&filter->identity_re);
	  free(filter->identity);
     }

     if (filter->user_id)
     {
	  regfree(&filter->user_id_re);
	  free(filter->user_id);
     }

     if (filter->time)
     {
	  regfree(&filter->time_re);
	  free(filter->time);
     }

     if (filter->referer)
     {
	  regfree(&filter->referer_re);
	  free(filter->referer);
     }

     if (filter->request_uri)
     {
	  regfree(&filter->request_uri_re);
	  free(filter->request_uri);
     }

     if (filter->request_method)
     {
	  regfree(&filter->request_method_re);
	  free(filter->request_method);
     }

     if (filter->request_protocol)
     {
	  regfree(&filter->request_protocol_re);
	  free(filter->request_protocol);
     }

     if (filter->status_code)
     {
	  regfree(&filter->status_code_re);
	  free(filter->status_code);
     }

     if (filter->size)
     {
	  regfree(&filter->size_re);
	  free(filter->size);
     }

     if (filter->ua)
     {
	  regfree(&filter->ua_re);
	  free(filter->ua);
     }
}

int
save_client_filter(struct log_entry_filter* filter, char* arg)
{
     filter->client = strdup(arg);
     return compile_re(&filter->client_re, filter->client);
}

int
save_identity_filter(struct log_entry_filter* filter, char* arg)
{
     filter->identity = strdup(arg);
     return compile_re(&filter->identity_re, filter->identity);
}

int
save_user_id_filter(struct log_entry_filter* filter, char* arg)
{
     filter->user_id = strdup(arg);
     return compile_re(&filter->user_id_re, filter->user_id);
}

int
save_time_filter(struct log_entry_filter* filter, char* arg)
{
     filter->time = strdup(arg);
     return compile_re(&filter->time_re, filter->time);
}

int
save_uri_filter(struct log_entry_filter* filter, char* arg)
{
     filter->request_uri = strdup(arg);
     return compile_re(&filter->request_uri_re, filter->request_uri);
}

int
save_referer_filter(struct log_entry_filter* filter, char* arg)
{
     filter->referer = strdup(arg);
     return compile_re(&filter->referer_re, filter->referer);
}

int
save_method_filter(struct log_entry_filter* filter, char* arg)
{
     filter->request_method = strdup(arg);
     return compile_re(&filter->request_method_re, filter->request_method);
}

int
save_protocol_filter(struct log_entry_filter* filter, char* arg)
{
     filter->request_protocol = strdup(arg);
     return compile_re(&filter->request_protocol_re,
		       filter->request_protocol);
}

int
save_status_filter(struct log_entry_filter* filter, char* arg)
{
     filter->status_code = strdup(arg);
     return compile_re(&filter->status_code_re, filter->status_code);
}

int
save_size_filter(struct log_entry_filter* filter, char* arg)
{
     filter->size = strdup(arg);
     return compile_re(&filter->size_re, filter->size);
}

int
save_ua_filter(struct log_entry_filter* filter, char* arg)
{
     filter->ua = strdup(arg);
     return compile_re(&filter->ua_re, filter->ua);
}

PRIVATE
int
filter_match(regex_t* filter, const char* field)
{
     int result = 1;

     if (filter && field)
     {
	  // regexec returns 0 when a match is made
	  if (regexec(filter, field, (size_t) 0, NULL, 0))
	  {
	       result = 0;
	  }
     }
     return result;
}

PRIVATE
int
compile_re(regex_t* re, const char* filter_str)
{
     int status = -1;

     assert(filter_str != NULL);

     if (strlen(filter_str) > 0)
     {
	  // REG_NOSUB: Don't try to substitute, should be faster
	  status = regcomp(re, filter_str, REG_NOSUB | (!case_sensitive)? REG_ICASE: 0);
     }

     return (status == 0) ? PARSE_SUCCESS: PARSE_FAILURE;
}
