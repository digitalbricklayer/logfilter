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
/* buffer.h - Definitions for buffer api
 *
 * Author: Jack Hughes
 * Written: Thursday, 28 April 2005
 */

#ifndef BUFFER_H
#define BUFFER_H   (1)

//----------------------------------------------  //-- Headers

#include <memory.h>

//----------------------------------------------  //--
//----------------------------------------------  //-- Constants

//----------------------------------------------  //--
//----------------------------------------------  //-- Macros

//----------------------------------------------  //--
//----------------------------------------------  //-- Typedefs

//----------------------------------------------  //--
//----------------------------------------------  //-- Public variables

//----------------------------------------------  //--
//----------------------------------------------  //-- Prototypes

// Allocate a buffer of buffer_len size. If an error occurs, terminate
// with an appropriate diagnostic message
extern void* buffer_allocate(void* existing_buffer, const size_t buffer_len);

//----------------------------------------------  //--

#endif  /* BUFFER_H */
