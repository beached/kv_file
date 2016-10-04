# KV File
A simple key value file parser.

Parses files like 
```
somekey=somevalue
"someother\"key"=someothervalue
"some=key2"=somethingelse
```
Each line is parsed into a struct with a string key and a string value member that is in a vector with each line being an element.  Blank lines are skipped
