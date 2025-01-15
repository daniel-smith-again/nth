/*******************************************************************************
--------------------------------------------------------------------------------
                        The nth Programming Environment                         
--------------------------------------------------------------------------------
                                       by                                      
                                  Daniel Smith                                  


--------------------------------------------------------------------------------
                                     About                                     
--------------------------------------------------------------------------------
                  This is a fully conforming implementation of                  
                  the Nth Programming Language as described in                  
                    "The nth Programming Language HyperDoc".                    


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


*******************************************************************************/
/* Edit me*/
#define __LINUX_AMD64__ //set this to one of 
                        // __LINUX_ARM64__
                        // __LINUX_i686__
                        // __LINUX_AMD64
                        // __WINDOWS_i686__
                        // __WINDOWS_AMD64__
                        // __MAC_AMD64__
                        // __MAC_ARM64__
#define Entry int main(int ArgCount, char** Args)
#define Iter(s) HalfWord i ; i < s ; i++
#define Array(T) T*
#define ArrayLoop(s, b) for(Iter(s)) { b }
#define Ref(T) T*

#ifdef __LINUX_ARM64__
#endif
#ifdef  __LINUX_i686__
#endif
#ifdef  __LINUX_AMD64__
#define Null void*
#define None void
#define Byte unsigned char
#define QuarterWord unsigned short
#define HalfWord unsigned int
#define Word unsigned long
#define Int int
#define LongInt long
#define CHARUNKNOWN 0xFFFD
typedef struct {
  HalfWord size;
  Array(HalfWord) contents;
} String;


Null claim(HalfWord size);
None drop(Null ref);

String utf8_to_charstr(Array(Byte) input, HalfWord size)
{
  String out;
  out.contents = claim(size);
  ArrayLoop(size,
    if (input[i - 1] & 0b10000000)
    {
      
    }
    else if (input[i - 1] & 0b11000000)
    {

    }
    else if (input[i - 1] & 0b11100000)
    {

    }
    else if (input[i - 1] & 0b11110000)
    {

    }
    else 
    {

    }
  )
}

#endif
#ifdef  __WINDOWS_i686__
#endif
#ifdef  __WINDOWS_AMD64__
#endif
#ifdef  __MAC_AMD64__
#endif
#ifdef  __MAC_ARM64__
#endif

Entry
{

}