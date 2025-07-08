function Show(e) { e.currentTarget.scrollIntoView(); }
function Search(f) 
{
    const results = document.getElementById('results');
    while (results.hasChildNodes()) { results.removeChild(results.lastChild); }
    const search = document.getElementById('searchbar').value.toLowerCase();
    if (search == "") { return false; }
    const pattern = new RegExp(search.split("").reduce((a,b) => { return a + b; }), "i");
    const dict = Object.keys(SearchTable);
    var topcount = 0;
    for (var i in dict)
    {
        if (pattern.test(dict[i]))
        {
            var l = document.createElement("li");
            l.style.listStyle = "none";
            var a = document.createElement("a");
            if (f)
            {
                window.location = SearchTable[dict[i]][1];
                return false;
            }
            a.href = SearchTable[dict[i]][1];
            a.appendChild(document.createTextNode(SearchTable[dict[i]][0]));
            l.appendChild(a);
            if (dict[i].startsWith(search))
            {
                results.insertBefore(l, results.childNodes[topcount]);
                topcount = topcount + 1;
            }
            else 
            {
                results.appendChild(l);
            }
        }
    }
}
const SearchTable = 
{
    "arithmetic":["Arithmetic","arithmetic.html"],
    "collections":["Collections","collections.html"],
    "data":["Data","data.html"],
    "defaults":["Defaults","defaults.html"],
    "foreword":["Foreword","foreword.html"],
    "evaluation":["Evaluation","evaluation.html"],
    "functions":["Functions","functions.html"],
    "images":["Images","images.html"],
    "overview":["Overview","overview.html"],
    "rationale":["Rationale","rationale.html"],
    "reference":["Reference","reference.html"],
    "syntax":["Syntax","syntax.html"],
    "table of contents":["Table of Contents","contents.html"],
    "terms":["Terms","terms.html"],
    "types":["Types","types.html"],
    "application":['"Application"', "terms.html#Application"],
    "binding":['"Binding"',"terms.html#Binding"],
    "blank":['"Blank"', "terms.html#Blank"],
    "character":['"Character"', "terms.html#Character"],
    "collection":['"Collection"', "terms.html#Collection"],
    "computer":['"Computer"', "terms.html#Computer"],
    "default":['"Default"',"terms.html#Default"],
    "digit":['"Digit"',"terms.html#Digit"],
    "environment":['"Environment"',"terms.html#Environment"],
    "error":['"Error"',"terms.html#Error"],
    "escape Sequence":['"Escape Sequence"',"terms.html#Escape-Sequence"],
    "execute":['"Execute"',"terms.html#Execute"],
    "evaluate":['"Evaluate"',"terms.html#Evaluate"],
    "expression":['"Expression"',"terms.html#Expression"],
    "field":['"Field"',"terms.html#Field"],
    "function":['"Function"',"terms.html#Function"],
    "glyph":['"Glyph"',"terms.html#Glyph"],
    "implementation":['"Implementation"',"terms.html#Implementation"],
    "member":['"Member"',"terms.html#Member"],
    "operation":['"Operation"',"terms.html#Operation"],
    "program":['"Program"',"terms.html#Program"],
    "program model":['"Program Model"',"terms.html#Program-Model"],
    "quote":['"Quote"',"terms.html#Quote"],
    "resource":['"Resource"',"terms.html#Resource"],
    "semantics":['"Semantics"',"terms.html#Semantics"],
    "source":['"Source"',"terms.html#Source"],
    "storage":['"Storage"',"terms.html#Storage"],
    "string":['"String"',"terms.html#String"],
    "symbol":['"Symbol"',"terms.html#Symbol"],
    "type":['"Type"',"terms.html#Type"],
}

const colors = 
[
  { 'bg_0': '#181818',
    'bg_1': '#252525',
    'bg_2': '#3b3b3b',
    'dim_0': '#777777',
    'fg_0': '#b9b9b9',
    'fg_1': '#dedede',
    'red': '#ed4a46',
    'green': '#70b433',
    'yellow': '#d1a416',
    'blue': '#368aeb',
    'magenta': '#3b6eb7',
    'cyan': '#3fc5b7',
    'orange': '#e67f43',
    'violet': '#a580e2',
    'br_red': '#ff5e56',
    'br_green': '#83c746',
    'br_yellow': '#efc541',
    'br_blue': '#4f9cfe',
    'br_magenta': '#ff81ca',
    'br_cyan': '#56d8c9',
    'br_orange': '#fa9153',
    'br_violet': '#b891f5'
  },
  { 'bg_0': '#103c48',
    'bg_1': '#184956',
    'bg_2': '#2d5b69',
    'dim_0': '#72898f',
    'fg_0': '#adbcbc',
    'fg_1': '#cad8d9',
    'red': '#fa5750',
    'green': '#75b938',
    'yellow': '#dbb32d',
    'blue': '#4695f7',
    'magenta': '#f275be',
    'cyan': '#41c7b9',
    'orange': '#ed8649',
    'violet': '#af88eb',
    'br_red': '#ff665c',
    'br_green': '#84c747',
    'br_yellow': '#ebc13d',
    'br_blue': '#58a3ff',
    'br_magenta': '#ff84cd',
    'br_cyan': '#53d6c7',
    'br_orange': '#fd9456',
    'br_violet': '#bd96fa'
  },
  { 'bg_0': '#fbf3db',
    'bg_1': '#ece3cc',
    'bg_2': '#d5cdb6',
    'dim_0': '#909995',
    'fg_0': '#53676d',
    'fg_1': '#3a4d53',
    'red': '#d2212d',
    'green': '#489100',
    'yellow': '#ad8900',
    'blue': '#0072d4',
    'magenta': '#ca4898',
    'cyan': '#009c8f',
    'orange': '#c25d1e',
    'violet': '#8762c6',
    'br_red': '#cc1729',
    'br_green': '#428b00',
    'br_yellow': '#a78300',
    'br_blue': '#006dce',
    'br_magenta': '#c44392',
    'br_cyan': '#00978a',
    'br_orange': '#bc5819',
    'br_violet': '#825dc0'
  },
  { 'bg_0': '#ffffff',
    'bg_1': '#ebebeb',
    'bg_2': '#cdcdcd',
    'dim_0': '#878787',
    'fg_0': '#474747',
    'fg_1': '#282828',
    'red': '#d6000c',
    'green': '#1d9700',
    'yellow': '#c49700',
    'blue': '#0064e4',
    'magenta': '#dd0f9d',
    'cyan': '#00ad9c',
    'orange': '#d04a00',
    'violet': '#7f51d6',
    'br_red': '#bf0000',
    'br_green': '#008400',
    'br_yellow': '#af8500',
    'br_blue': '#0054cf',
    'br_magenta': '#c7008b',
    'br_cyan': '#009a8a',
    'br_orange': '#ba3700',
    'br_violet': '#6b40c3'
  },
]
var Theme = 3
function setTheme(index)
{
  index = Theme
  var c = colors[index % 4]
  var r = document.querySelector(':root')
  r.style.setProperty('--background', c.bg_0)
  r.style.setProperty('--backgroundsecond', c.bg_1)
  r.style.setProperty('--backgroundthird', c.bg_2)
  r.style.setProperty('--diminished', c.dim_0)
  r.style.setProperty('--foreground', c.fg_0)
  r.style.setProperty('--foregroundsecond', c.fg_1)
  r.style.setProperty('--red', c.red)
  r.style.setProperty('--green', c.green)
  r.style.setProperty('--yellow', c.yellow)
  r.style.setProperty('--blue', c.blue)
  r.style.setProperty('--magenta', c.magenta)
  r.style.setProperty('--cyan', c.cyan)
  r.style.setProperty('--orange', c.orange)
  r.style.setProperty('--violet', c.violet)
  r.style.setProperty('--brightred', r.br_red)
  r.style.setProperty('--brightgreen', c.br_green)
  r.style.setProperty('--brightyellow', c.br_yellow)
  r.style.setProperty('--brightblue', c.br_blue)
  r.style.setProperty('--brightmagenta', c.br_magenta)
  r.style.setProperty('--brightcyan', c.br_cyan)
  r.style.setProperty('--brightorange', c.br_orange)
  r.style.setProperty('--brightviolet', c.br_violet)
  r.style.setProperty('--hgl', c.dim_0 + '40')
  Theme = Theme + 1
  if (Theme >= 4) 
  {
    Theme = 0
  }
}
setTheme(Theme);