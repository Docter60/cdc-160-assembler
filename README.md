# CDC 160 Assembler

## Table of Contents
- [CDC 160 Assembler](#cdc-160-assembler)
  - [Table of Contents](#table-of-contents)
  - [Description](#description)
  - [Requirements to build](#requirements-to-build)
  - [Usage](#usage)
  - [Syntax](#syntax)
    - [Overview](#overview)
    - [Whitespace](#whitespace)
    - [Comments](#comments)
    - [Setting the memory address](#setting-the-memory-address)
    - [Machine Code Literals](#machine-code-literals)
    - [CDC 160-A Instructions](#cdc-160-a-instructions)
  - [References](#references)

## Description

The CDC 160 Assembler defines an assembly syntax for the CDC 160-A instruction set and can parse an assembly file that generates CDC 160-A machine code.

## Requirements to build

- CMake 3.4 minimum
- C++ compiler with 17 standard

## Usage

```
160asm input.asm [-o output.160]
```

 - **160asm** - program name
 - **input.asm** - CDC 160-A assembly file
 - **output.160** - CDC 160-A machine code file

## Syntax

### Overview

```
//

@ <address> [//]

<XXXX> [//]

<instruction> [//]

<instruction> X [//]

<instruction> XX [//]

<instruction> XXXX [//]

<instruction> X YYYY [//]

<instruction> XX YYYY [//]
```

### Whitespace

Empty lines are ignored.  Tabs and multiple spaces in syntax are currently unsupported.

### Comments

```
//

<syntax> [//]
```

Comments come in two variants: line and post.  Line comments are lines beginning with // and are completely ignored by the assembler.  Post comments follow the end of an instruction on the same line.  Note that for line comments, the // must be followed immediately by a space, and for post comments, the // must be delimited by spaces on either side.

**Example**
```
// Simple program to multiply A by 10
LDN 03 // Load A with 0003
MUT // Do multiplication
```

### Setting the memory address

```
@ <address>
```

Setting the location to load memory is done using the @ symbol followed by a four-digit octal address and delimited by a space.  The following lines of data will be stored starting at the respective address. 

**Example**

```
@ 1234
5670
@ 7770
6502
4004
8008
@ 0100
7700
```

The assembly example above is only using data literals for simplicity and is understood as:
1. Start by setting the next address to 1234.
2. Store 5670 at address 1234.
3. Set the next address to 7770.
4. Store 6502 at address 7770.
5. Store 4004 at address 7771.
6. Store 8008 at address 7772.
7. Set the next address to 0100.
8. End with 7700 stored at 0100.

### Machine Code Literals

```
<XXXX>
```

The assembler accepts any lines with a 4-digit octal number as direct data to be written in the machine code.

**Example**\
Please review the machine code literals used in the [Setting the memory address](#setting-the-memory-address) example.

### CDC 160-A Instructions

```
<instruction>

<instruction> X

<instruction> XX

<instruction> XXXX

<instruction> X YYYY

<instruction> XX YYYY
```

Each CDC 160-A instruction has a three-letter identifier and follows one of the above syntax rules.  X and Y signify an octal digit (X one digit, XX two digits, etc).  Each instruction only occupies a single line.

## References

[CDC 160-A Computer Reference Manual](https://bitsavers.org/pdf/cdc/160/60014500G_160-A_ref_Mar65.pdf)\
[CDC 160-A Peripheral Equipment Codes](https://bitsavers.org/pdf/cdc/160/60024900_160A_Peripheral_Equipment_Codes_Jan65.pdf)\
[CDC 160-A Assembly System](https://bitsavers.org/pdf/cdc/160/60050700C_OSAS-A_160A_Assember_Apr64.pdf)