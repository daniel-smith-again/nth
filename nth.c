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

#define BuffersMax 7
#define BufferSegmentSize 1024

/******************************* Shell Routines *******************************/

char* Buffer;
unsigned int Buffer_length = 0;
unsigned int Buffer_size = 0;
char** recent_buffers;
int recent_buffer_count;

void Shell();

/******************************* Image Routines *******************************/

/*********************************** Entry ************************************/
int main() 
{
  Shell();
  return 0;
}


#include <stdio.h>
void Shell()
{
  int running = 1;
  char c = 0;
  while(running)
  {
    c = getchar();
    switch(c)
    {
      case 27:                          // escape
        goto End;
        break;
      default:
        putchar(c);
    }
  }
  End:
  return;
}
