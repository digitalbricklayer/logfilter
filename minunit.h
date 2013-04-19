/* minunit.h - As taken from http://www.jera.com/techinfo/jtns/jtn002.html
 *
 * Author: John Brewer
 * Licence: You may use the code in this tech note for any purpose, with 
 * the understanding that it comes with NO WARRANTY. 
 */

#ifndef MINUNIT_H
#define MINUNIT_H   (1)

//----------------------------------------------  //-- Headers

//----------------------------------------------  //--
//----------------------------------------------  //-- Constants

//----------------------------------------------  //--
//----------------------------------------------  //-- Macros

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)

//----------------------------------------------  //--
//----------------------------------------------  //-- Typedefs

//----------------------------------------------  //--
//----------------------------------------------  //-- Public variables

extern int tests_run;

//----------------------------------------------  //--
//----------------------------------------------  //-- Prototypes

//----------------------------------------------  //--

#endif  /* MINUNIT_H */
