
Install c2rust
=============

Follow the instructions at
https://github.com/immunant/c2rust

Run c2rust
=============

A Makefile is prepared for the initial run of the c to rust conversion.

Outlook
=============

When feeding the crytal-facet-uml sources, the c2rust transpiler aborts with error.

Even when trying to find a function that can be transpiled, the generated rust code
is neither nice nor safe.

There are also more sound studies than my single-project evaluation, e.g.
https://csslab-ustc.github.io/publications/2023/rust-transpiler.pdf

Also a similar project, crust, aborts with error:
thread 'main' (7753) panicked at src/library/parser/parser.rs:655:33

For now, I'm trying to find a different approach.

