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
          stringsegment,
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
void KillBuffer();
Syntax *Parse();
void nth_print_syntax(Syntax *s);

int main ()
{
  DropBuffer();
  for(;;)
  {
    input.top = Parse();
    if (input.top)
    {
      //nth_print_syntax(input.top);
      putchar('\n');
    }
    else if (input.e > -1)
    {
      printf("\x1b[30;107m?\x1b[0m\n");
      KillBuffer();
    }
    else DropBuffer();
  }
  return 0;
}

char Char()
{
  if (input.i >= input.size)
  {
    for ( char c = 0;;
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
    case string:
    case expression:
      a->size++;
      a->nodes = realloc(a->nodes, sizeof(Syntax*) * a->size);
      a->nodes[a->size-1] = b;
  }
}

void DropBuffer()
{
  if (input.nodes) 
    for (Int i = 0; i < input.count; i++)
    {
        if (  input.nodes[i]->kind != symbol && 
              input.nodes[i]->kind != stringsegment &&
              input.nodes[i])
        {
          free(input.nodes[i]->nodes);
        }
        free(input.nodes[i]);
    }
  if (input.i < input.size)
  {
    char *t = input.content;
    input.content = malloc(sizeof(char) * input.size - input.i);
    for (Int i = 0; i < input.size - input.i; i++)
      input.content[i] = t[input.i + i];
    free(t);
    input.size -= input.i;
    input.i = 0;
  }
  else 
  {
    if (input.content) free(input.content);
    input.content = malloc(0);
    input.size = 0;
  }
  input.nodes = malloc(0);
  input.count = 0;
  input.e = -1;
  InsideQuote = 0;
}

void KillBuffer()
{
  DropBuffer();
  free(input.content);
  input.content = malloc(0);
  input.size = 0;
  input.i = 0;
}

Syntax *Parse()
{
  Int alreadyquoted = 0;
  Syntax *t, *S;
  switch(Peek())
  {
    case '(':
      Char();
      for (S = CreateNode(expression); Peek() != ')';)
      {
        t = Parse();
        if (input.e > -1) goto ParseError;
        if (t) AppendNode(S, t);
      }
      Char();
      return S;
      break;
    case ' ':
    case '\n':
      while (Peek() <= ' ') Char();
      return 0;
      break;
    case ')': Char(); goto ParseError;
    case '"':
      S = CreateNode(string);
      Char();
      for(;Peek() != '"';) 
      {
        switch(Peek())
        {
          case '\\': 
            Char();
            if (Peek() == '(')
            {
              t = Parse();
              if (input.e > -1) goto ParseError;
              if (t) AppendNode(S, t);
              continue;
            } 
            AppendNode(S, CreateNode(stringsegment));
            S->nodes[S->size - 1]->content = input.i;
          default:
            if (!S->nodes || S->nodes[S->size - 1]->kind != stringsegment)
              AppendNode(S, CreateNode(stringsegment)),
              S->nodes[S->size - 1]->content = input.i;
            S->nodes[S->size - 1]->size++;
            Char();
        }
      }
      Char();
      return S;
      break;
    case '\'':
      Char();
      S = CreateNode(quote);
      if (Peek() == '\'') { Char(); S->kind = unquote; }
      if (Peek() == '\'') { Char(); S->kind = requote; }
      if (S->kind != quote && !InsideQuote) goto ParseError;
      if (InsideQuote) alreadyquoted = 1;
      else InsideQuote = 1;
      t = Parse();
      if (input.e > -1) goto ParseError;
      if (t) AppendNode(S, t);
      if (!alreadyquoted) InsideQuote = 0;
      return S;
    default:
      for (S = CreateNode(symbol), S->content = input.i, S->size = 1, Char();; S->size++, Char())
      { if (Peek() <= ' ' || Peek() == '(' || Peek() == ')') return S; }
      break;
  }
  ParseError:
  if (input.e == -1) input.e = input.i;
  return 0;
}

void nth_print_syntax(Syntax *s)
{
  switch(s->kind)
  {
    case string:
      putchar('"');
      for (Int i = 0; i < s->size; i++)
        nth_print_syntax(s->nodes[i]);
      putchar('"');
      break;
    case expression:
      putchar('(');
      for (Int i = 0; i < s->size; i++) 
        nth_print_syntax(s->nodes[i]), i < s->size - 1 ? putchar(' ') : 0;
      putchar(')');
      break;
    case quote: printf("'"); goto nth_print_syntaxQuote; break;
    case unquote: printf("''"); goto nth_print_syntaxQuote; break;
    case requote: printf("'''"); goto nth_print_syntaxQuote; break;
      nth_print_syntaxQuote: 
      nth_print_syntax(s->nodes[0]);
      break;
    case stringsegment:
    case symbol:
      for (Int i = 0; i < s->size; i++)
        putchar(input.content[s->content+i]);
      break;
  }
}