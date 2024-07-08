/******************************************************************************
-------------------------------------------------------------------------------
                        The nth Programming Environment                         
-------------------------------------------------------------------------------
                                       by                                      
                                  Daniel Smith                                  

-------------------------------------------------------------------------------
                                     About                                     
-------------------------------------------------------------------------------

     This is a fully working implementation of the nth Programming Language    
     as described in "The nth Programming Language HyperDoc". It is a fully    
          conforming implementation as described in the "Conformance"          
                      section of the chapter, "Foreword".                      


-------------------------------------------------------------------------------
                                  Terms of Use                                 
-------------------------------------------------------------------------------

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


-------------------------------------------------------------------------------
                             STATEMENT OF WARRANTY                             
-------------------------------------------------------------------------------

        THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       
                EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO               
      THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,     
                              AND NONINFRINGEMENT.                             

        IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES,        
   OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE,   
     ARISING FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE, OR THE USE,     
                       OR OTHER DEALINGS IN THE SOFTWARE.                      


*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

typedef unsigned int Int;
typedef enum 
        { 
          symbol, 
          number, 
          string, 
          expression, 
          quote, 
          unquote, 
          requote 
        } Kind;
typedef struct nth__syntax
{
  Kind kind;
  Int size;
  union { Int content; struct nth__syntax *nodes; };
} Syntax;

struct nth__input
{
  char *content;
  Int size;
  Int i;
  Syntax *nodes;
  Int length;
};
struct nth__input Input;

char Char();
char Peek();
Syntax *CreateNode(Kind k);
void AppendNode(Syntax *a, Syntax *b);
void DropBuffer();
void Parse();

int main ()
{
  Input.content = malloc(0);
  Input.size = 0;
  Input.i = 0;
  Input.nodes = malloc(0);
  Int length = 0;
  return 0;
}

char Char()
{
  if (Input.i >= Input.size)
  {
    for ( char c = 0; 1;
          c = getchar(),
          Input.size++,
          Input.content = realloc(Input.content, sizeof(char) * Input.size),
          Input.content[Input.size-1] = c)
      {if (c == '\n') break;}
  }
  char c = Input.content[Input.i];
  Input.i++;
  return c;
}