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
          stringportion,
          expression, 
          quote, 
          unquote, 
          requote 
        } Kind;
typedef struct nth__syntax
{
  Kind kind;
  Int size;
  union { Int content; struct nth__syntax **nodes; };
} Syntax;
Int InsideQuote;

struct
{
  char *content;
  Int size;
  Int i;
  int e;
  Syntax *top;
  Syntax **nodes;
  Int count;
} input;

char Char();
char Peek();
Syntax *CreateNode(Kind k);
void AppendNode(Syntax *a, Syntax *b);
void DropBuffer();
Syntax *Parse();
void Print(Syntax *s);

int main ()
{
  DropBuffer();
  for(;;)
  {
    input.top = Parse();
    if (input.top)
    {
      Print(input.top);
    }
    else
    {
      printf("Parse Error\n");
      for (Int i = 0; i < input.e; i++)
        putchar(input.content[i]);
    }
    DropBuffer();
    putchar('\n');
  }
  return 0;
}

char Char()
{
  if (input.i >= input.size)
  {
    for ( char c = 0; 
          1;
          c = getchar(),
          input.size++,
          input.content = realloc(input.content, sizeof(char) * input.size),
          input.content[input.size-1] = c)
      {if (c == '\n') break;}
  }
  char c = input.content[input.i];
  input.i++;
  return c;
}

char Peek()
{
  char c = Char();
  input.i--;
  return c;
}

Syntax *CreateNode(Kind k)
{
  Syntax *n = malloc(sizeof(Syntax));
  n->kind = k;
  n->size = 0;
  n->content = 0;
  n->nodes = 0;
  input.count++;
  input.nodes = realloc(input.nodes, sizeof(Syntax*) * input.count);
  input.nodes[input.count-1] = n;
  return n;
}

void AppendNode(Syntax *a, Syntax *b)
{
  switch(a->kind)
  {
    case quote:
    case unquote:
    case requote:
      if (a->size == 1) break;
    case expression:
      a->size++;
      a->nodes = realloc(a->nodes, sizeof(Syntax*) * a->size);
      a->nodes[a->size-1] = b;
  }
}

void DropBuffer()
{
  if (input.nodes) for (Int i = 0; i < input.count; i++) 
                        free(input.nodes[i]->nodes),
                        free(input.nodes[i]);
  if (input.content) free(input.content);
  input.nodes = malloc(0);
  input.content = malloc(0);
  input.count = 0;
  input.size = 0;
  input.i = 0;
  input.e = -1;
  InsideQuote = 0;
}

Syntax *Parse()
{
  Int alreadyquoted = 0;
  Syntax *t, *S;
  backtrack:
  switch(Peek())
  {
    case '(':
      Char();
      for (S = CreateNode(expression); Peek() != ')';)
      {
        t = Parse();
        if (!t) goto ParseError;
        else AppendNode(S, t);
      }
      Char();
      return S;
      break;
    case ' ':
    case '\n':
      Char();
      while (Peek() <= ' ') Char();
      goto backtrack;
      break;
    case ')': goto ParseError;
    case '\'':
      Char();
      S = CreateNode(quote);
      if (Peek() == '\'') { Char(); S->kind = unquote; }
      if (Peek() == '\'') { Char(); S->kind = requote; }
      if (S->kind != quote && !InsideQuote) goto ParseError;
      if (InsideQuote) alreadyquoted = 1;
      else InsideQuote = 1;
      S->size = 1;
      S->nodes = malloc(sizeof(Syntax*));
      t = Parse();
      if (t == 0) goto ParseError;
      else S->nodes[0] = t;
      if (!alreadyquoted) InsideQuote = 0;
      return S;
    default:
      for (S = CreateNode(symbol), S->content = input.i, S->size = 1, Char();; S->size++, Char())
      { if (Peek() <= ' ') { Char(); return S; }
        else if (Peek() == '(' || Peek() == ')') return S;
      }
      break;
  }
  ParseError:
  if (input.e == -1) input.e = input.i;
  free(S);
  return 0;
}

void Print(Syntax *s)
{
  switch(s->kind)
  {
    case expression:
      putchar('(');
      for (Int i = 0; i < s->size; i++) 
        Print(s->nodes[i]), i < s->size - 1 ? putchar(' ') : 0;
      putchar(')');
      break;
    case quote: printf("'"); goto PrintQuote; break;
    case unquote: printf("''"); goto PrintQuote; break;
    case requote: printf("'''"); goto PrintQuote; break;
      PrintQuote: Print(s->nodes[0]);
      break;
    case symbol:
      for (Int i = 0; i < s->size; i++)
        putchar(input.content[s->content+i]);
      break;
  }
}