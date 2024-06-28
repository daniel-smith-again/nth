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

#include <stdio.h>

#define BuffersMax 7
#define BufferSegmentSize 1024

char* Buffer;
unsigned int Buffer_length = 0;
unsigned int Buffer_size = 0;
char** recent_buffers;
int recent_buffer_count;

void Shell();

typdef struct {
  enum {symbol, string, number, expression, quote, unquote, requote} kind;
  unsigned int size;
  union
  {
    struct String {char *contents;};
    struct Symbol {char *contents;};
    struct Number {unsigned char* contents;};
    struct Expression {void **fields;};
    struct Quote {void *contents;};
    struct Unquote {void *contents;};
    struct Requote {void *contents;};
  }
} Program;

/*********************************** Entry ************************************/
int main() 
{
  Shell();
  return 0;
}


void Shell()
{
  int running = 1;
  char c = 0;
  Program top, *p;
  p = &top;
  p->type = expression;
  p->size = 0;
  while(running)
  {
    c = getchar()
    switch(p->type)
    {
      case symbol:
      break;
      case string:
      break;
      case number:
      break;
      case expression:
      break;
      case quote:
      break;
      case unquote:
      break;
      case requote:
      break;
    }
  }
  End:
  return;
}
