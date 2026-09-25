# tr Localization File Format (.tloc)

tr provides a file format for localization files (henceforth referred to as 'tloc').

tloc is a textual file format for defining localization key-value pairs. Key-value pairs may additionally be scoped in namespaces, and both line and multiline comments are supported in C++ style.

Namespace may freely be opened multiple times within the same document, while keys are not allowed to be repeated within the same chunk.

tloc is parsed with `tr::parse_localization_to()` and related functions.

## Example

```
foo {
    /* Multiline
       comment   */
    bar {
        baz = "simple value" // Line comment
    }
    qux = "value with \"quotes\""
}
quux = "value with\\backslashes\\"
```

The above results in:

Key: `foo.bar.baz`, Value: `simple value`

Key: `foo.qux`, Value: `value with "quotes"`

Key: `quux`, Value: `value with \backslashes\`

## EBNF Grammar

```
character                   = ? Any individual Unicode codepoint encoded as UTF-8 ? ;

(* Whitespace *)
SP                          = ' ' ;
HT                          = ? U+0009 ? ;
LF                          = ? U+000A ? ;
VT                          = ? U+000B ? ;
FF                          = ? U+000C ? ;
CR                          = ? U+000D ? ;
whitespace character        = SP | LF | HT | VT | FF | CR ;

(* Line comment *)
line comment begin          = "//" ;
line comment character      = char - LF ;
line comment                = line comment begin , { line comment character } ;

(* Multiline comment *)
multiline comment begin     = "/*" ;
multiline comment character = ( character - '*' ) | ( '*' , ( character - '/' ) ) ;
multiline comment end       = "*/" ;
multiline comment           = multiline comment begin , { multiline comment character } , multiline comment end ;

(* Comments *)
comment                     = line comment | multiline comment ;
whitespace                  = { whitespace character | comment } ;

(* Symbol *)
reserved character          = '=' | '"' | '{' | '}' | '/' | '\' ;
symbol character            = character - ( whitespace character | reserved character ) ;
symbol                      = symbol character , { symbol character } ;

(* String *)
string delimiter            = '"' ;
escape character            = '\' ;
LF escape                   = 'n' ;
unescaped character         = character - ( string delimiter | escape character ) ;
escaped character           = escape character , ( escape character | string delimiter | LF escape ) ;
string character            = unescaped character | escaped character ;
string                      = string delimiter , { string character } , string delimiter ;

(* Key-value pair *)
kvpair separator            = '=' ;
kvpair pair                 = symbol , whitespace , kvpair separator , whitespace , string ;

(* Namespace *)
namespace begin             = '{' ;
namespace end               = '}' ;
namespace                   = symbol , whitespace , namespace begin , expression list , namespace end ;

(* Grammar *)
expression                  = kvpair | namespace ;
expression list             = whitespace , { expression , whitespace } ;
grammar                     = expression list ;
```