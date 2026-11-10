# StrPP - Safer Strings in C
StrPP (Strings Plus Protection) is a C library aimed at providing programmers a safer, more efficient way of handling strings.
Rather than the standard `char*` "string" in C, StrPP implements its own memory layout designed for memory safety and speed.
Who needs Rust now?
A visualisation of the StrPP memory layout:
```
[ LENGTH ] [ DATA ]
```

Not only does StrPP provide a safer string type, but it also provides its own string utilities, like `strpp_strcat()`, eliminating the need for constant type conversions, because of this, you'll find that these utilities tend to be faster than libc!
