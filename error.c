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
/* error.c - Functions for handling errors
 *
 * Author: Jack Hughes
 * Created: Thursday, 28 April 2005
 */

//----------------------------------------------  //-- Headers

#include "error.h"
#include <stdio.h>
#include <stdlib.h>

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

void
exit_with_diagnostic(char* diagnostic_message)
{
     fprintf(stderr, diagnostic_message);
     exit(EXIT_FAILURE);
}
