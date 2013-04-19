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
/* buffer.c - Functions for manipulating a buffer
 *
 * Author: Jack Hughes
 * Created: Thursday, 28 April 2005
 */

//----------------------------------------------  //-- Headers

#include "buffer.h"
#include <stdlib.h>
#ifdef _WIN32
#include <malloc.h>
#endif
#include "error.h"

//----------------------------------------------  //--
//----------------------------------------------  //-- Constants

#ifndef _DEBUG
#define PRIVATE static
#else
#define PRIVATE
#endif

//----------------------------------------------  //--
//----------------------------------------------  //-- Macros

//----------------------------------------------  //--
//----------------------------------------------  //-- Types & Classes

//----------------------------------------------  //--
//----------------------------------------------  //-- Public variables

//----------------------------------------------  //--
//----------------------------------------------  //-- Private variables

//----------------------------------------------  //--
//----------------------------------------------  //-- Prototypes

//----------------------------------------------  //--
//----------------------------------------------  //-- Functions

void*
buffer_allocate(void* existing_buffer, const size_t buffer_len)
{
     void* buffer;
#ifdef _WIN32
     // Suggested fix to the following bug in Microsoft's version of realloc
     // See http://support.microsoft.com/kb/q225099/
     _set_sbh_threshold(0);
#endif
     buffer = realloc(existing_buffer, buffer_len);
     if (!buffer)
     {
	  exit_with_diagnostic("Unable to allocate buffer");
     }
     return buffer;
}
