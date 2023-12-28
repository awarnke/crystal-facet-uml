
# Test cases of u8stream

This folder contains test cases for the u8stream component.

Test cases are designed to execute fast, to be reproducible and to cover the provided functionality.

## Test categories

Test cases are stored based on their category:

- unit
  - test scope is the inner logic of a unit
  - trigger and observation points are at the interfaces of the unit
- integration
  - test scope is the interface between two or more units
  - trigger and observation points are at the interfaces of the units under test
- qualification (stored at [../../build/qualification_test](../../build/qualification_test) )
  - test scope is the integrated application
  - trigger and observation points are at the interfaces of the application

In case of contradicting criteria, the test scope precedes.

## Test goal

- unit tests shall cover 100% lines of source code

