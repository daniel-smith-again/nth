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