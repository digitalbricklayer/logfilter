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
/* filter_test.c - Implementation of filter unit tests
 *
 * Author: Jack Hughes
 * Created: Thursday, 30 April 2005
 */

//----------------------------------------------  //-- Headers

#include "filter_test.h"
#include "filter.h"
#include "minunit.h"
#include <stdlib.h>
#include <string.h>

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

// Test functions
PRIVATE char* simple_uri_test(void);
PRIVATE char* case_sensitive_compound_test(void);
PRIVATE char* case_insensitive_compound_test(void);
PRIVATE char* null_filter_test(void);
PRIVATE char* simple_time_test(void);
PRIVATE char* negative_status_test(void);

//----------------------------------------------  //--
//----------------------------------------------  //-- Functions

char*
filter_all_tests(void)
{
     mu_run_test(null_filter_test);
     mu_run_test(simple_uri_test);
     mu_run_test(simple_time_test);
     mu_run_test(negative_status_test);
     mu_run_test(case_sensitive_compound_test);
     mu_run_test(case_insensitive_compound_test);
     return 0;
}

PRIVATE
char*
simple_uri_test(void)
{
     char test_line[] = "192.168.2.1 - - [01/Apr/2005:20:25:53 +0100] \"GET /images/talkto.gif HTTP/1.1\" 200 4226 \"http://www.openxtra.co.uk/support/howto/ethereal-advanced-filter.php\" \"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; (R1 1.5); .NET CLR 1.1.4322)\"/r/n";
     char filter_arg_success[] = "/images/talkto.gif";
     char filter_arg_failure[] = "/articles/index.php";
     struct log_file_entry* entry;
     struct log_entry_filter filter;
     int result_success;
     int result_failure;

     memset(&filter, 0, sizeof(struct log_entry_filter));

     entry = parse_line(test_line);

     // Test the successfull scenario
     mu_assert("simple_uri_test test 1",
               save_uri_filter(&filter, filter_arg_success));
     result_success = filter_entry(&filter, entry);
     mu_assert("simple_uri_test test 2", result_success > 0);
     filter_free(&filter);

     // Test the unsuccessful scenario
     mu_assert("simple_uri_test test 3", 
               save_uri_filter(&filter, filter_arg_failure));
     result_failure = filter_entry(&filter, entry);
     mu_assert("simple_uri_test test 4", !result_failure);
     filter_free(&filter);
     free_entry(entry);
     free(entry);

     return 0;
}

PRIVATE
char*
simple_time_test(void)
{
     char test_line[] = "192.168.2.1 - - [01/Apr/2005:20:25:53 +0100] \"GET /images/talkto.gif HTTP/1.1\" 200 4226 \"http://www.openxtra.co.uk/support/howto/ethereal-advanced-filter.php\" \"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; (R1 1.5); .NET CLR 1.1.4322)\"/r/n";
     char filter_arg_success[] = "01/Apr/2005";
     char filter_arg_fail[] = "02/Apr/2005";
     struct log_file_entry* entry;
     struct log_entry_filter filter_success, filter_fail;
     int result_success;
     int result_fail;

     memset(&filter_success, 0, sizeof(struct log_entry_filter));
     memset(&filter_fail, 0, sizeof(struct log_entry_filter));

     entry = parse_line(test_line);
     mu_assert("simple_time_test test 1", entry);

     result_success = save_time_filter(&filter_success, filter_arg_success);
     result_fail = save_time_filter(&filter_fail, filter_arg_fail);

     mu_assert("simple_time_test test 2", result_success != 0);
     mu_assert("simple_time_test test 3", result_fail != 0);

     result_success = filter_entry(&filter_success, entry);

     mu_assert("simple_time_test test 4", result_success > 0);

     result_fail = filter_entry(&filter_fail, entry);

     mu_assert("simple_time_test test 5", !result_fail);

     filter_free(&filter_success);
     filter_free(&filter_fail);
     free_entry(entry);
     free(entry);

     return 0;
}

PRIVATE
char*
case_sensitive_compound_test(void)
{
     char test_line[] = "192.168.2.1 - - [01/Apr/2005:20:25:53 +0100] \"GET /images/talkto.gif HTTP/1.1\" 200 4226 \"http://www.openxtra.co.uk/support/howto/ethereal-advanced-filter.php\" \"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; (R1 1.5); .NET CLR 1.1.4322)\"/r/n";
     char filter_arg_ip[] = "192.168.2.1";
     char filter_arg_id[] = "-";
     char filter_arg_userid[] = "-";
     char filter_arg_time[] = "01/Apr/2005";
     char filter_arg_uri[] = "/images/talkto.gif";
     char filter_arg_referer[] = "ethereal-advanced-filter.php";
     char filter_arg_method[] = "get";
     char filter_arg_protocol[] = "1.1";
     char filter_arg_status[] = "200";
     char filter_arg_size[] = "4226";
     char filter_arg_ua[] = "MSIE 6.0";
     struct log_file_entry* entry;
     struct log_entry_filter filter;
     int result;

     memset(&filter, 0, sizeof(struct log_entry_filter));

     case_sensitive = 1;

     entry = parse_line(test_line);

     result = save_time_filter(&filter, filter_arg_time);
     mu_assert("case_sensitive_compound_test test 1", result != 0);

     result = save_client_filter(&filter, filter_arg_ip);
     mu_assert("case_sensitive_compound_test test 2", result != 0);

     result = save_identity_filter(&filter, filter_arg_id);
     mu_assert("case_sensitive_compound_test test 3", result != 0);

     result = save_time_filter(&filter, filter_arg_time);
     mu_assert("case_sensitive_compound_test test 4", result != 0);

     result = save_uri_filter(&filter, filter_arg_uri);
     mu_assert("case_sensitive_compound_test test 5", result != 0);

     result = save_referer_filter(&filter, filter_arg_referer);
     mu_assert("case_sensitive_compound_test test 6", result != 0);

     result = save_method_filter(&filter, filter_arg_method);
     mu_assert("case_sensitive_compound_test test 7", result != 0);

     result = save_protocol_filter(&filter, filter_arg_protocol);
     mu_assert("case_sensitive_compound_test test 8", result != 0);

     result = save_status_filter(&filter, filter_arg_status);
     mu_assert("case_sensitive_compound_test test 9", result != 0);

     result = save_size_filter(&filter, filter_arg_size);
     mu_assert("case_sensitive_compound_test test 10", result != 0);

     result = save_ua_filter(&filter, filter_arg_ua);
     mu_assert("case_sensitive_compound_test test 11", result != 0);

     // Match with case sensitive matching should NOT work
     result = filter_entry(&filter, entry);
     mu_assert("case_sensitive_compound_test test 12", !result);

     free_entry(entry);
     filter_free(&filter);

     return 0;
}

PRIVATE
char*
case_insensitive_compound_test(void)
{
     char test_line[] = "192.168.2.1 - - [01/Apr/2005:20:25:53 +0100] \"GET /images/talkto.gif HTTP/1.1\" 200 4226 \"http://www.openxtra.co.uk/support/howto/ethereal-advanced-filter.php\" \"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; (R1 1.5); .NET CLR 1.1.4322)\"/r/n";
     char filter_arg_ip[] = "192.168.2.1";
     char filter_arg_id[] = "-";
     char filter_arg_userid[] = "-";
     char filter_arg_time[] = "01/Apr/2005";
     char filter_arg_uri[] = "/images/talkto.gif";
     char filter_arg_referer[] = "ethereal-advanced-filter.php";
     char filter_arg_method[] = "get";
     char filter_arg_protocol[] = "1.1";
     char filter_arg_status[] = "200";
     char filter_arg_size[] = "4226";
     char filter_arg_ua[] = "MSIE 6.0";
     struct log_file_entry* entry;
     struct log_entry_filter filter;
     int result;

     memset(&filter, 0, sizeof(struct log_entry_filter));

     case_sensitive = 0;

     entry = parse_line(test_line);

     result = save_time_filter(&filter, filter_arg_time);
     mu_assert("case_insensitive_compound_test test 1", result != 0);

     result = save_client_filter(&filter, filter_arg_ip);
     mu_assert("case_insensitive_compound_test test 2", result != 0);

     result = save_identity_filter(&filter, filter_arg_id);
     mu_assert("case_insensitive_compound_test test 3", result != 0);

     result = save_time_filter(&filter, filter_arg_time);
     mu_assert("case_insensitive_compound_test test 4", result != 0);

     result = save_uri_filter(&filter, filter_arg_uri);
     mu_assert("case_insensitive_compound_test test 5", result != 0);

     result = save_referer_filter(&filter, filter_arg_referer);
     mu_assert("case_insensitive_compound_test test 6", result != 0);

     result = save_method_filter(&filter, filter_arg_method);
     mu_assert("case_insensitive_compound_test test 7", result != 0);

     result = save_protocol_filter(&filter, filter_arg_protocol);
     mu_assert("case_insensitive_compound_test test 8", result != 0);

     result = save_status_filter(&filter, filter_arg_status);
     mu_assert("case_insensitive_compound_test test 9", result != 0);

     result = save_size_filter(&filter, filter_arg_size);
     mu_assert("case_insensitive_compound_test test 10", result != 0);

     result = save_ua_filter(&filter, filter_arg_ua);
     mu_assert("case_insensitive_compound_test test 11", result != 0);

     // Match with case insensitive matching should work
     result = filter_entry(&filter, entry);
     mu_assert("case_insensitive_compound_test test 12", result);

     free_entry(entry);
     filter_free(&filter);

     return 0;
}

PRIVATE
char*
null_filter_test(void)
{
     char test_line[] = "192.168.2.1 - - [01/Apr/2005:20:25:53 +0100] \"GET /images/talkto.gif HTTP/1.1\" 200 4226 \"http://www.openxtra.co.uk/support/howto/ethereal-advanced-filter.php\" \"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; (R1 1.5); .NET CLR 1.1.4322)\"/r/n";
     struct log_file_entry* entry;
     int result;

     // A null filter means that the all lines pass (except invalid lines)
     entry = parse_line(test_line);
     result = filter_entry(NULL, entry);
     mu_assert("null_filter_test test 1", result);
     free_entry(entry);

     return 0;
}

PRIVATE
char*
negative_status_test(void)
{
     char test_line[] = "192.168.2.1 - - [01/Apr/2005:20:25:53 +0100] \"GET /images/talkto.gif HTTP/1.1\" 200 4226 \"http://www.openxtra.co.uk/support/howto/ethereal-advanced-filter.php\" \"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; (R1 1.5); .NET CLR 1.1.4322)\"/r/n";
     char filter_arg_success[] = "^200$";
     char filter_arg_failure[] = "!404";
     struct log_file_entry* entry;
     struct log_entry_filter filter;
     int result_success;
     int result_failure;

     memset(&filter, 0, sizeof(struct log_entry_filter));

     entry = parse_line(test_line);

     // Test the successfull scenario
     result_success = save_status_filter(&filter, filter_arg_success);
     mu_assert("negative_status_test test 1", result_success != 0);

     result_success = filter_entry(&filter, entry);
     mu_assert("negative_status_test test 2", result_success > 0);

     filter_free(&filter);

     // Test the unsuccessful scenario
     result_failure = save_status_filter(&filter, filter_arg_failure);
     mu_assert("negative_status_test test 3", result_failure != 0);

     result_failure = filter_entry(&filter, entry);
     mu_assert("negative_status_test test 4", !result_failure);

     filter_free(&filter);
     free_entry(entry);

     return 0;
}
