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
#include <stdlib.h>

void Shell();

struct nth__program {
  enum {symbol, string, number, expression, quote, unquote, requote} Type;
  unsigned int Size;
  union
  {
    unsigned int String;
    unsigned int Symbol;
    unsigned int Number;
    unsigned int *Expression;
  };
};
typedef struct nth__program Program;

struct nth__buffer
{
  unsigned int l;
  unsigned int i;
  char*b;
  unsigned int s;
  Program *p;
};
struct nth__buffer Buffer;

int main() 
{
  Buffer.l = 0;
  Buffer.i = 0;
  Buffer.s = 0;
  Buffer.b = malloc(0);
  Buffer.p = malloc(0);
  Shell();
  return 0;
}

void ResetBuffer()
{
  Buffer.l = 0;
  Buffer.i = 0;
  Buffer.s = 0;
  Buffer.b != 0 ? (free(Buffer.b), Buffer.b = 0) : (Buffer.b = 0);
  for (unsigned int i = 0; i < Buffer.s; i++)
    if (Buffer.p[i].Type == expression)
      free(Buffer.p[i].Expression);
  Buffer.p != 0 ? (free(Buffer.p), Buffer.p = 0) : (Buffer.p = 0);
}

char Char()
{
  if (Buffer.i >= Buffer.l)
  {
    for(char c = 0; 
        1; 
        c = getchar(),
        Buffer.l++,
        Buffer.b = realloc(Buffer.b, sizeof(char) * Buffer.l),
        Buffer.b[Buffer.l-1] = c) 
        {if (c == '\n') break;}
  }
  Buffer.i++;
  return Buffer.b[Buffer.i-1];
}

char Peek()
{
  char c = Char();
  Buffer.i--;
  return c;
}

int IsSymbChar(char c)
{
  if (c <= ' '
  ||  c == '('
  ||  c == ')'
  ||  c == '"')
    return 0;
  else
    return 1;
}

unsigned int NewProgram()
{
  Buffer.s++;
  Buffer.p = realloc(Buffer.p, sizeof(Program) * Buffer.s);
  return Buffer.s - 1;
}
Program *FindProgram(unsigned int i)
{
  return &Buffer.p[i];
}
void PushAtom(unsigned int i, unsigned int atom)
{
  Program *p = FindProgram(i);
  if (p->Type != expression 
  &&  p->Type != quote
  &&  p->Type != unquote
  &&  p->Type != requote)
  return;
  if (p->Type != expression && p->Size == 1) return;
  if (atom == 0) return;
  p->Size++;
  p->Expression = realloc(p->Expression, sizeof(unsigned int*) * p->Size);
  p->Expression[p->Size-1] = atom;
}

unsigned int Read()
{
  Program *p;
  char tmp;
  unsigned int tmp_size, i, j;
  switch(Peek())
  {
    case '(':
      Char();
      i = NewProgram();
      p = FindProgram(i);
      p->Type = expression, p->Size = 0, p->Expression = malloc(0);
      for (;Peek() != ')';)
      {
        j = Read();
        if (j != -1) {
          PushAtom(i, j);
        }
      }
      return i;
      break;
    default:
      if (Peek() <= ' ') {Char(); return -1;}
      i = NewProgram();
      p = FindProgram(i);
      p->Type = symbol, p->Size = 1, p->Symbol = Buffer.i-1;
      for (;;)
      {
        if (IsSymbChar(Peek()))
          p->Size++, Char();
        else break;
      }
      return i;
      break;
  }
}

void Print(Program *p)
{
  switch(p->Type)
  {
    case expression:
     // printf("%i\n", p->Size);
      putchar('(');
      for (int i = 0; i < p->Size; i++)
        Print(FindProgram(p->Expression[i])), 0;
        //i < p->Size - 1 ? putchar(' ') : 0;
      putchar(')');
      break;
    case symbol:
      for (int i = 0; i < p->Size; i++)
        putchar(Buffer.b[p->Symbol + i]);
      break;
  }
}

void Shell()
{
  int running = 1;
  Program *p = FindProgram(Read());
  0;
  if (p == (Program *)-1) ResetBuffer();
  else Print(p), putchar('\n');
  End:
  return;
}
