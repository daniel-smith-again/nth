import { nth } from "./nth.js";
const Nth = new nth();
var area = document.getElementsByTagName('nth-area')[0]
var help = document.getElementsByTagName('nth-help')[0]
var editor = document.getElementById('editortemplate')
var helpbutton = document.getElementsByTagName('help-button')[0]
helpbutton.addEventListener('click', (e)=>help.style.display = 'block')
var closehelp = document.getElementsByTagName('close-help')[0]
closehelp.addEventListener('click', (e)=>help.style.display = 'none')
var unicodebutton = document.getElementsByTagName('nth-showhide')[0]
var unicodemenu = document.getElementsByTagName('nth-unicode')[0]
unicodebutton.onclick = (e) => {unicodemenu.getAttribute('displayhidden') == 'true' ? (unicodemenu.style.left = '0', unicodemenu.setAttribute('displayhidden', 'false')) : (unicodemenu.style.left = '-22ch', unicodemenu.setAttribute('displayhidden', 'true'));}

var unicodelisting = "\n+-⨯÷√=≠<≤>≥±≡≢≣∼∽∾∿≀≁\n∧∨¬∀∃∄∴∵⊢⊣⊤⊥\n←↑→↓↔↕↖↗↘↙\n∩∪∈∉∋∌∖∅⊂⊃⊄⊅⊆⊇⊈⊉⊊⊋⊌⊍⊎\nΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩαβγδεζηθικλμνξοπρςστυφχψω\n⊕⊖⊗⊘⊙⊚⊛⊜⊝\n⦁⦂⦃⦄⦅⦆⦇⦈⦉⦊⦋⦌⦍⦎⦏⦐⦑⦒⦓⦔⦕⦖⦗⦘⫷⫸⫹⫺⫶⦙⦚\n⌶⌷⌸⌹⌺⌻⌼⌽⌾⌿⍀⍁⍂⍃⍄⍅⍆⍇⍈⍉⍊⍋⍌⍍⍎⍏⍐⍑⍒⍓⍔⍕⍖⍗⍘⍙⍚⍛⍜⍝⍞⍟⍠⍡⍢⍣⍤⍥⍦⍧⍨⍩⍪⍫⍬⍭⍮⍯⍰⍱⍲⍳⍴⍵⍶⍷⍸⍹⍺"
unicodelisting.split('').map(c => {
  if (c == '\n') {unicodemenu.appendChild(document.createElement('div'))}
  else {
    unicodemenu.lastChild.appendChild(document.createElement('span'))
    unicodemenu.lastChild.onclick = (e)=>{if (active) insertCharacter(e.target.textContent[0])}
    unicodemenu.lastChild.lastChild.appendChild(document.createTextNode(c))
  }
})


var active = null;
document['clipboard'] = null;
area.onclick = (e)=> 
{
    if (area['clicklock']) {area['clicklock'] = false; return;}
    if (e.target.tagName != 'NTH-AREA') return;
    e.preventDefault()
    var snippet = createSnippet()
    area.appendChild(snippet)
    var r = snippet.getBoundingClientRect();
    snippet.style.top = String(Math.floor(e.clientY - (r.height / 2))) + 'px'
    snippet.style.left = String(Math.floor(e.clientX - (r.width / 2))) + 'px'
    
}
function createSnippet() 
{
  var snippet = editor.cloneNode(true)
  for (var x = 0; x < area.childElementCount; x++)
    area.children[x].removeAttribute('active')
  snippet.setAttribute('active', 'true')
  snippet.id = null
  snippet.style.display = 'grid'
  active = snippet, active.children[0].focus()
  snippet['caret'] = snippet.lastChild
  snippet.caret.symbol = null
  snippet.children[1].children[0].children[0].onclick = Run
  snippet.children[1].children[0].children[1].onclick = Rewrite
  snippet.children[1].children[0].children[2].children[0].children[0].onclick = Copy
  snippet.children[1].children[0].children[2].children[0].children[1].onclick = Cut
  snippet.children[1].children[0].children[2].children[0].children[2].onclick = Paste
  snippet.children[1].children[0].children[3].children[0].children[0].onclick = saveCode
  snippet.children[1].children[0].children[3].children[0].children[1].onclick = loadCode
  snippet.children[1].children[0].children[4].onclick = Close
  snippet.onpointerdown = dragCode
  return snippet
}
function Close(e)
{
  area.removeChild(active)
}

function dragCode(e) 
{
  e.preventDefault()
  area['clicklock'] = true;
  var snippet = e.currentTarget
  for (var x = 0; x < area.childElementCount; x++)
    area.children[x].removeAttribute('active')
  snippet.setAttribute('active', 'true')
  active = snippet, active.children[0].focus()
  const offsetY = snippet.offsetTop - (e.type == 'touchstart' ? e.touches[0].clientY : e.clientY)
  const offsetX = snippet.offsetLeft - (e.type == 'touchstart' ? e.touches[0].clientX : e.clientX)
  var move = function(e) 
             {
                e.preventDefault()
                var x = (e.type == 'touchmove' ? e.touches[0].clientX : e.clientX) + offsetX
                var y = (e.type == 'touchmove' ? e.touches[0].clientY : e.clientY) + offsetY
                if ((x > 0) && (x + snippet.offsetWidth < window.innerWidth))
                  snippet.style.left = String((e.type == 'touchmove' ? e.touches[0].clientX : e.clientX) + offsetX) + 'px'
                if ((y > 0) && (y + snippet.offsetHeight < window.innerHeight))
                  snippet.style.top = String((e.type == 'touchmove' ? e.touches[0].clientY : e.clientY) + offsetY) + 'px'
             }
  var stopmove = function(e)
                 {
                    e.preventDefault()
                    document.onpointermove = null
                    document.onpointerup = null
                 }
  document.onpointermove = move
  document.onpointerup = stopmove
}
document.onkeydown = (e) => {
  if (document.activeElement.tagName == 'NTH-STRING') 
  {
    if (e.key == 'Escape') 
    {
      document.activeElement.blur()
    }
    return
  }
  if (e.ctrlKey)
  {
    switch(e.key)
    {
      case 'c': Copy(e); break
      case 'x': Cut(e); break
      case 'v': Paste(e); break
      default: return
    }
    return
  }
  if (e.key.length == 1) {
    e.preventDefault()
    switch(e.key) {
      case '(': insertExpression(); break
      case ')': outwards(); break
      case ' ': insertSymbol(); break
      case '"': insertString(); break
      default: insertCharacter(e.key)
    }
  }
  else {
    e.preventDefault();
    switch(e.key) {
      case 'Enter':
        if (e.shiftKey) Run()
        else insertBreak()
      break
      case 'Tab': growTab(); break
      case 'Backspace': backspace(); break
      case 'Delete': removeExpression(); break
      case 'ArrowRight': forwards(); break
      case 'ArrowLeft': backwards(); break
      case 'ArrowUp': outwards(); break
      case 'ArrowDown': inwards(); break
      case 'Escape': if (active != null) 
                        active.removeAttribute('active'),active.children[0].blur(), active = null; break
    }
  }
}
function insertCharacter(c) 
{
  var caret = active.caret;
  if (caret.symbol == null || 
      caret.symbol.tagName == 'BR' || 
      caret.symbol.tagName == 'NTH-EXPRESSION' || 
      caret.symbol.tagName == 'NTH-STRING' || 
      caret.symbol.getAttribute('tab') != null)
  {
    insertSymbol()
  }
  if (caret.symbol.tagName != 'NTH-SYMBOL') return
  caret.symbol.textContent = caret.symbol.textContent + c
  checkQuote(caret.symbol)
  reduceShortcuts(caret.symbol)
}
function insertSymbol() 
{
  var caret = active.caret;
  if (caret.tagName == 'NTH-CODE' && caret.childElementCount > 0) return
  var s = document.createElement('nth-symbol')
  s.appendChild(document.createTextNode(''))
  s.setAttribute('active', 'true')
  s.onclick = highlightSymbol
  s.ontouchstart = highlightSymbol
  
  if (caret.symbol) caret.symbol.removeAttribute('active');
  if (caret.childElementCount == 0) 
  {
    caret.appendChild(s)
    caret.symbol = caret.firstChild
  }
  else if (caret.symbol.nextSibling)
  {
    caret.insertBefore(s, caret.symbol.nextSibling)
    caret.symbol = caret.symbol.nextSibling
  }
  else
  {
    caret.appendChild(s)
    caret.symbol = caret.lastChild
  } 
}
function insertExpression() 
{
  var caret = active.caret
  if (caret.tagName == 'NTH-CODE' && caret.childElementCount > 0) return
  var e = document.createElement('nth-expression')
  e['symbol'] = null;
  e.setAttribute('active', 'true')
  caret.removeAttribute('active');
  if (caret.symbol != null) caret.symbol.removeAttribute('active')
  if (caret.childElementCount == 0)
  {
    caret.appendChild(e)
    active.caret = caret.firstChild;
  }
  else if (caret.symbol.nextSibling)
  {
    caret.insertBefore(e, caret.symbol.nextSibling)
    active.caret = caret.symbol.nextSibling
  }
  else
  {
    caret.appendChild(e)
    active.caret = caret.lastChild
  }
}
function insertBreak() 
{
  var caret = active.caret
  if (caret.tagName == 'NTH-CODE') return
  var b = document.createElement('br')
  if (caret.childElementCount == 0) return
  else
  {
    caret.symbol.removeAttribute('active')
    if (caret.symbol.nextSibling)
    {
      caret.insertBefore(b, caret.symbol.nextSibling)
    }
    else 
    {
      caret.appendChild(b)
    }
    active.caret.symbol = caret.symbol.nextSibling
  }
}
function growTab() 
{
  var caret = active.caret
  if (caret.symbol && caret.symbol.tagName == 'NTH-SYMBOL')
  {
    if (caret.symbol.textContent == '')
    {
      caret.symbol.textContent = caret.symbol.textContent + ' '
      caret.symbol.setAttribute('tab', 'true')
    }
    else if (caret.symbol.getAttribute('tab') != null)
      caret.symbol.textContent = caret.symbol.textContent + ' '

  }
  else
  {
    insertSymbol()
  }
}
function backspace() 
{
  var caret = active.caret
  if (caret.childElementCount > 0)
  {
    if (caret.symbol.tagName !== 'NTH-EXPRESSION' && caret.symbol.textContent.length > 0 && caret.symbol.tagName != 'NTH-STRING')
    {
      caret.symbol.textContent = caret.symbol.textContent.slice(0, -1)
      checkQuote(caret.symbol)
      if (caret.symbol.textContent.length == 0 && caret.symbol.getAttribute('tab') != null)
        caret.symbol.removeAttribute('tab')
    }
    else 
    {
      if (caret.symbol.previousSibling)
      {
        var temp = caret.symbol
        active.caret.symbol = caret.symbol.previousSibling
        active.caret.symbol.setAttribute('active', 'true')
        caret.removeChild(temp)
      }
      else
      {
        if (caret.symbol.nextSibling)
        {
          var temp = caret.symbol
          active.caret.symbol = caret.symbol.nextSibling
          active.caret.symbol.setAttribute('active', 'true')
          caret.removeChild(temp)
        }
        else
        {
          caret.replaceChildren()
          active.caret.symbol = null;
        }
      }
    }
  }
  else removeExpression()
}
function removeExpression() 
{
  var caret = active.caret
  var temp = caret
  if (caret.parentElement.tagName == 'NTH-EXPRESSION' || caret.parentElement.tagName == 'NTH-CODE')
    active.caret = caret.parentElement
  else 
  {
    active.caret.replaceChildren()
    active.caret.symbol = null
    return
  }
  if (temp.previousSibling) active.caret.symbol = temp.previousSibling
  else if (temp.nextSibling) active.caret.symbol = temp.nextSibling
  else active.caret.symbol = null
  temp.parentElement.removeChild(temp)
  active.caret.setAttribute('active', 'true')
  if (active.caret.symbol) active.caret.symbol.setAttribute('active', 'true');
}
function forwards()
{
  var caret = active.caret
  if (caret.childElementCount == 0) return
  if (caret.symbol && caret.symbol.nextSibling)
  {
    caret.symbol.removeAttribute('active')
    caret.symbol = caret.symbol.nextSibling
    caret.symbol.setAttribute('active', 'true')
  }
}
function backwards()
{
  var caret = active.caret
  if (caret.childElementCount == 0) return
  if (caret.symbol && caret.symbol.previousSibling)
  {
    caret.symbol.removeAttribute('active')
    active.caret.symbol = caret.symbol.previousSibling
    active.caret.symbol.setAttribute('active', 'true')
  }
}
function inwards() 
{
 var caret = active.caret
 if (caret.symbol == null) return
 if (caret.symbol.tagName == 'NTH-EXPRESSION')
 {
  caret.removeAttribute('active')
  active.caret = caret.symbol
  active.caret.setAttribute('active', 'true')
  if (active.caret.symbol != null)
    active.caret.symbol.setAttribute('active', 'true')
  else if (active.caret.childElementCount > 0)
  {
    active.caret.symbol = active.caret.children[0]
    active.caret.symbol.setAttribute('active', 'true')
  }
 }
 else if (caret.symbol.tagName == 'NTH-STRING')
  caret.symbol.click()
 
}
function outwards() 
{
  var caret = active.caret
  var temp = caret
  if (caret.parentElement && (caret.parentElement.tagName == 'NTH-EXPRESSION' || caret.parentElement.tagName == 'NTH-CODE'))
  {
    caret.removeAttribute('active')
    if (caret.symbol != null) caret.symbol.removeAttribute('active')
    active.caret = caret.parentElement
    active.caret.setAttribute('active', 'true')
    active.caret.symbol = temp
    active.caret.symbol.setAttribute('active', 'true')
  }
}
function highlightSymbol(e) 
{
  e.preventDefault()
  var p = e.target
  while (p.parentElement.tagName != 'NTH-EDITOR')
    p = p.parentElement
  p = p.parentElement
  if (active != null && active != p) active.caret.removeAttribute('active')
  active = p, active.children[0].focus()
  if (active.caret != null) active.caret.removeAttribute('active')
  if (active.caret.symbol) active.caret.symbol.removeAttribute('active')
  active.caret = e.target.parentElement;
  active.caret.setAttribute('active', 'true')
  active.caret.symbol = e.target
  active.caret.symbol.setAttribute('active', 'true')
}
function checkQuote(s) 
{
  if (s.textContent == "'" ||
      s.textContent == "''" ||
      s.textContent == "'''")
    s.setAttribute('quote', 'true')
  else if (s.getAttribute('quote') != null)
    s.removeAttribute('quote')
}
function reduceShortcuts(s)
{
  if (s.textContent.includes('\\\\'))
  {
    s.textContent = s.textContent.replace(/\\\\->/g, '→')
    s.textContent = s.textContent.replace(/\\\\\*/g, '⨯')
    s.textContent = s.textContent.replace(/\\\\\//g, '÷')
    s.textContent = s.textContent.replace(/\\\\and/g, '∧')
    s.textContent = s.textContent.replace(/\\\\or/g, '∨')
    s.textContent = s.textContent.replace(/\\\\not/g, '¬')
    s.textContent = s.textContent.replace(/\\\\\/=/g, '≠')
    s.textContent = s.textContent.replace(/\\\\<=/g, '≤')
    s.textContent = s.textContent.replace(/\\\\>=/g, '≥')
  }
}
function Cut(e)
{
  e.stopImmediatePropagation();
  if (active.caret == null) return;
  if (active.caret.symbol == null) return;
  document.clipboard = null;
  document.clipboard = active.caret.symbol
  document.clipboard.removeAttribute('active')
  if (active.caret.childElementCount == 1) 
    active.caret.symbol = null
  else if (document.clipboard.previousSibling) 
    active.caret.symbol = active.caret.previousSibling
  else if (document.clipboard.nextSibling)
    active.caret.symbol = active.caret.nextSibling
  else
    active.caret.symbol = null
  active.caret.removeChild(document.clipboard);
}
function Copy(e)
{
  e.stopImmediatePropagation();
  if (active.caret == null) return;
  if (active.caret.symbol == null) return;
  document.clipboard = null;
  document.clipboard = active.caret.symbol.cloneNode(true)
  if (document.clipboard.tagName == 'NTH-SYMBOL')
    document.clipboard.onclick = highlightSymbol
    document.clipboard.ontouchstart = highlightSymbol
}
function Paste(e)
{
  e.stopImmediatePropagation();
  if (document.clipboard == null) return;
  if (active.caret.tagName == 'NTH-CODE' && active.caret.childElementCount == 1) return;
  if (active.caret == null)
  {
    active.lastChild.appendChild(document.clipboard)
  }
  else 
  {
    if (active.caret.symbol && active.caret.symbol.getAttribute('active'))
      active.caret.symbol.removeAttribute('active');
    if (active.caret.symbol && active.caret.symbol.nextSibling)
    {
      active.caret.insertBefore(document.clipboard, active.caret.symbol.nextSibling)
      active.caret.symbol = active.caret.symbol.nextSibling
      active.caret.symbol.setAttribute('active', 'true')
    }
    else
    {
      active.caret.appendChild(document.clipboard)
      active.caret.symbol = active.caret.lastChild
      active.caret.symbol.setAttribute('active', 'true')
    }
    document.clipboard = null;
  }
}
function insertString()
{
  var caret = active.caret;
  if (caret.tagName == 'NTH-CODE' && caret.childElementCount > 0) return
  var s = document.createElement('nth-string')
  s.contentEditable = true
  s.autocapitalize = false
  s.spellcheck = false
  s.onclick = (e) => {highlightSymbol(e); e.target.focus()}
  s.ontouchstart = (e) => {highlightSymbol(e); e.target.focus()}
  s.setAttribute('active', 'true')
  if (caret.symbol) caret.symbol.removeAttribute('active')
  if (caret.childElementCount == 0) 
  {
    caret.appendChild(s)
    caret.symbol = caret.firstChild
  }
  else if (caret.symbol.nextSibling)
  {
    caret.insertBefore(s, caret.symbol.nextSibling)
    caret.symbol = caret.symbol.nextSibling
  }
  else
  {
    caret.appendChild(s)
    caret.symbol = caret.lastChild
  }
}
function saveCode(e)
{
  var p = structureToProgram(active.lastChild.lastChild)
  var download = document.createElement('a')
  download.setAttribute('href', 'data:text/plain;charset=utf8,' + encodeURIComponent(p))
  download.setAttribute('download', 'nth-source.txt')
  download.style.display = 'none'
  document.body.appendChild(download)
  download.click()
  document.body.removeChild(download)
}
function loadCode(e)
{
  var input = document.createElement('input')
  input.type = 'file';
  input.style.display = 'none'
  input.onchange = async (e) => 
  {
    var file = input.files[0]
    var program = await file.text()
    var e = programToStructure(program)
    active.lastChild.replaceChildren(e)
    active.caret = active.lastChild
    active.caret.setAttribute('active', 'true')
    if (active.caret.childElementCount > 0)
    {
      active.caret.symbol = active.caret.children[0]
      active.caret.symbol.setAttribute('active', 'true')
    }
  }
  document.body.appendChild(input)
  input.click()
}
function Run(e)
{
  if (active.lastChild.childElementCount == 0) return
  var result = Eval(active.lastChild.lastChild)
  var r = document.createElement('nth-symbol')
  r.appendChild(document.createTextNode('_'))
  active.lastChild.replaceChildren(result)
  active.caret = active.lastChild
  active.caret.symbol = r
  active.caret.symbol.setAttribute('active', 'true')
}
function Rewrite(e) 
{
}
function structureToProgram(s) 
{
  switch(s.tagName) {
    case 'NTH-EXPRESSION':
      var p = '(' 
      for (var x = 0; x < s.childElementCount; x++)
      {
        p += structureToProgram(s.children[x])
        if (x < s.childElementCount - 1) 
        {
          if (s.children[x].tagName != 'NTH-TAB' &&
              s.children[x].getAttribute('quote') == null &&
              s.children[x].getAttribute('tab') == null &&
              s.children[x].tagName != 'BR' &&
              s.children[x + 1].tagName != 'BR')
          {
            p += ' '
          }
        }
      }
      p += ')'
      return p
    break
    case 'NTH-SYMBOL': 
      return s.innerText
    break
    case 'NTH-STRING': 
      return '"' + s.innerText + '" '
    break
    case 'BR': 
      return '\n'
    break
  }
}
function programToStructure(p)
{
  var i = 0;
  var getChar = () => {var char = (i >= p.length ? null : p[i]); i++; return char;}
  var peekChar = () => {return i >= p.length ? null : p[i]; }
  function parse()  
  {
    var expression
    var c = peekChar()
    switch(c)
    {
      case '(':
        expression = document.createElement('nth-expression')
        c = getChar()
        while (c = peekChar(), c != ')')
        {
          var temp = parse()
          if (temp) expression.appendChild(temp)
        }
        getChar()
        return expression
      break;
      case '"':
        c = getChar()
        expression = document.createElement('nth-string')
        expression.contentEditable = true
        expression.autocapitalize = false
        expression.spellcheck = false
        expression.onclick = (e) => {highlightSymbol(e); e.target.focus()}
        expression.ontouchstart = (e) => {highlightSymbol(e); e.target.focus()}
        var stringcontent = ''
        while (c = peekChar(), c != '"')
        {
          if (c == '\t' || c == '\r') continue
          if (c == '\\')
            stringcontent += c, getChar(), c = peekChar()
          if (c == '\n')
          {
            expression.appendChild(document.createTextNode(stringcontent))
            expression.appendChild(document.createElement('br'))
            stringcontent = ''
          }
          stringcontent += c, getChar()
        }
        getChar()
        expression.appendChild(document.createTextNode(stringcontent))
        return expression
      break;
      case '\n':
        c = getChar(), expression = document.createElement('br')
        return expression
      break;
      case ' ':
      case '\u00a0':
        c = getChar()
        expression = document.createElement('nth-symbol')
        expression.setAttribute('tab', 'true')
        expression.textContent = '\u00a0';
        while (c = peekChar(), c == ' ' || c == '\u00a0')
          expression.textContent += '\u00a0', getChar()
        if (expression.textContent.length > 1) return expression
        else return null
      break;
      case ')':
        throw "Unexpected closing brace!"
      break;
      default:
        c = getChar()
        expression = document.createElement('nth-symbol')
        expression.onclick = highlightSymbol
        expression.ontouchstart = highlightSymbol
        expression.textContent = c
        while (c = peekChar(), c != '(' && c != ')' && c != ' ' && c != '\n' && c != null)
          expression.textContent += getChar()
        var t = expression.textContent
        checkQuote(expression)
        return expression
    }
  }
  var top
  try {
    top = parse ()
  }
  catch(e) {console.log(e);top = document.createElement('nth-symbol').appendChild(document.createTextNode('_'))}
  return top
}
function Eval(program)
{
  if (program.tagName == 'NTH-CODE')
  { program = program.lastChild; }
  var src = structureToProgram(program).replaceAll(/[^\S]/gi, ' ');
  var result = Nth.eval(src)
  return programToStructure(result)
}