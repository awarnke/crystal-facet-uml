
Install c2rust
=============

Follow the instructions at
https://github.com/immunant/c2rust

Run c2rust
=============

A Makefile is prepared for the initial run of the c to rust conversion.

Outlook
=============

The c2rust transpiler does not create valid rust code, instead it aborts with error.

Even when trying to find a function that can be transpiled, the generated rust code
is neiter nice nor safe. One does not want to continue working with this generated code, 
even if the code does the same.

There are also studies, e.g.
https://csslab-ustc.github.io/publications/2023/rust-transpiler.pdf

I'm trying to find a different approach.
