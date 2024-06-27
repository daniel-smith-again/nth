/*******************************************************************************
--------------------------------------------------------------------------------
                        The nth Programming Environment                         
--------------------------------------------------------------------------------
                                       by                                       
                                  Daniel Smith                                  


--------------------------------------------------------------------------------
                                  Terms of Use                                  
--------------------------------------------------------------------------------

                  The author(s) grants the holder of this work                  
             full right of access, without limitation, irrevocably,             
                 in perpetuity, for any purpose, by any means.                  

           Authorship of this work is reserved only for contributions           
       to this work which are made under the terms given in this section.       
             Contribution to this work under terms other than those             
                given in this section constitutes authorship of                 
                    a distinct and separate work, for which                     
                the status of authorship of this particular work                
                                is not granted.                                 

            In plain terms, if you, the rights holder of this work,             
           create any derivation, modification, or copy of this work,           
           you cannot claim that it is a form, version, or variation            
              of the work titled "The nth Programming Environment"              
         without providing it under the terms set out in this section.          

               The purpose of these terms is to provide the most                
               permissive rights and allowances while preventing                
                any individual or party from claiming authorship                
                          with less permissive rights                           
                and thus tainting the availability of the work.                 


--------------------------------------------------------------------------------
                             STATEMENT OF WARRANTY                              
--------------------------------------------------------------------------------

        THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        
                EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO                
      THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,      
                              AND NONINFRINGEMENT.                              

        IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES,        
   OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE,   
     ARISING FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE, OR THE USE,     
                       OR OTHER DEALINGS IN THE SOFTWARE.                       


--------------------------------------------------------------------------------
                                     About                                      
--------------------------------------------------------------------------------

     This is a fully working implementation of the nth Programming Language     
     as described in "The nth Programming Language HyperDoc". It is a fully     
          conforming implementation as described in the "Conformance"           
                      section of the chapter, "Foreword".                       

*******************************************************************************/

typedef unsigned long long int Nat;
typedef signed long long int Int;
typedef void Nil;
typedef void* Any;

/*                              String Routines                               */

typedef struct                          //----------------String----------------
{
  Nat length;
  unsigned char* contents;              // assumes ASCII but will transparently 
                                        // handle utf-8 encoding as long as
                                        // your system also handles it.
                                        // i.e. ASCII->ASCII or UTF-8 -> UTF-8
} String;

String* join (String* a, String* b);    //Creates a new string

Nil drop_string(String* s);              //deletes a string

/****************************** Number Routines *******************************/

typedef struct                          //----------------Number----------------
{
  Nat size;                             // the number of bytes
  unsigned char* digits;                // array of bytes
} Number;

/******************************* Array Routines *******************************/

typedef struct                          //----------------Array-----------------
{
  Nat cardinality;                      // number of fields
  Any fields;                           // pointers to field data
} Array;

/**************************** Collection Routines *****************************/

typedef struct                          //--------------Collection--------------
{
  String* bindings;                      // pointers to field names
  Array members;                        // A Collection is an Array supertype
} Collection;

/***************************** Function Routines ******************************/

typedef struct
{
  Collection parameters;
  String definition;
} Function;

/******************************* Type Routines ********************************/

typedef struct
{
  enum
  {
    discrete,
    continuous
  } kind;
  union
  {
    Collection c;
    Function f;
  }
} Type;

/******************************* Unit Routines ********************************/

typedef struct
{
  enum 
  {
    string, 
    number, 
    array, 
    collection
  } instance;
  union 
  {

  }
} Unit;

/************************* Memory Allocation Routines *************************/

Any take(Nat size);                     //---------------Allocate---------------
                                        // just a wrapper for malloc for now

Nil drop(Any data);                     //--------------Deallocate--------------
                                        // just a wrapper for free for now

/******************************* Shell Routines *******************************/

String* Read();
Any Compile();
Any Compute(Any program);

/******************************* Image Routines *******************************/

/*********************************** Entry ************************************/
int main() 
{
  return 0;
}

#include <stdlib.h>
Any take(Nat size) { return malloc(size); }
Nil drop(Any data) { if (data != 0 && data != NULL) free(data); }

String* join (String* a, String* b)
{
  String *s = take(sizeof(String));
  s->length = a->length + b->length, s->contents = take(s->length);
  for (int i = 0; i < a->length; i++)
    s->contents[i] = a->contents[i];
  for (int i = 0; i < b->length; i++)
    s->contents[a->length + i] = b->contents[i];
  return s;
}

Nil drop_string(String* s)
{
  drop(s->contents), drop(s);
}

#include <stdio.h>

String* Read() 
{
  char c = getchar();
  switch (c) {
    case '(':
    break;
    case ')': 
    break;
    case '\'': 
    break;
    case '"':
    break;
  }
}
