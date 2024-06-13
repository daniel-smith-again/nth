/*******************************************************************************
--------------------------------------------------------------------------------
                        The Nth Programming Environment                         
--------------------------------------------------------------------------------
                                       by                                       
                                  Daniel Smith                                  

--------------------------------------------------------------------------------
                                  Terms of Use                                  
--------------------------------------------------------------------------------

        The author grants the holder of this work full right of access,          
       without limitation, in perpetuity, for any purpose, by any means.        

           Authorship of this work is reserved only for contributions           
       to this work which are made under the terms given in this section.       

  Contribution to this work under terms other than those given in this section  
       constitutes authorship of a distinct and separate work, for which         
                the status of authorship of this particular work                
                                is not granted.                                 


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

This is a fully working implementation of the nth programming language.
However, it is not intended to be efficient or performant.
It's only purpose is to facilitate
the magical, mystical, bootstrapping stage
for the actual compiler, written in nth,
which will compile itself to an actually useful tool.



*******************************************************************************/

typedef unsigned long long int Nat;
typedef signed long long int Int;
typedef void Nil;
typedef void* Any;

/****************************** String Routines *******************************/

typedef struct {                        //----------------String----------------
  Nat length;
  unsigned char* contents;              // assumes ASCII but will transparently 
                                        // handle utf-8 encoding as long as
                                        // your system also handles it.
                                        // i.e. ASCII->ASCII or UTF-8 -> UTF-8
} String;

String* join (String* a, String* b);    //Creates a new string

Nil drop_string(String* s);              //deletes a string

/****************************** Number Routines *******************************/

typedef struct {                        //----------------Number----------------
  Nat size;                             // the number of bytes
  unsigned char* digits;                // array of bytes
} Number;

/**************************** Collection Routines *****************************/

typedef struct {                        //--------------Collection--------------
  Nat cardinality;                      // number of fields
  char** bindings;                      // pointers to field names
  Any fields;                           // pointers to field data
} Collection;

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
int main() {
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
String* Read_nth_symbol()
{

}
String* Read_nth_expression()
{

}
String* Read_nth_quote()
{

}

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
