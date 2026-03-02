# Introduction

Welcome to the GLaDOS documentation!

## What is GLaDOS?

GLaDOS is a simple programming language with a compiler and virtual machine implementation written in Haskell. This project (Part 2) consists of:

- **A Compiler** that translates GLaDOS source code into bytecode
- **A Virtual Machine (VM)** that executes the compiled bytecode

## Project Overview

GLaDOS is designed as an educational project to demonstrate the fundamental concepts of:

- **Language Design**: Simple, C-like syntax with functions and control flow
- **Compilation**: Transforming high-level code into low-level bytecode
- **Virtual Machine Architecture**: Stack-based execution model
- **Type System**: Support for integers and booleans with type safety

## Key Features

- **Simple Syntax**: Clean, readable syntax inspired by C and JavaScript
- **Functions**: First-class functions with parameters and recursion support
- **Type System**: Integer and boolean types with type-safe operations
- **Control Flow**: If-else conditionals for decision making
- **Bytecode Compilation**: Efficient compilation to stack-based bytecode
- **Virtual Machine**: Fast bytecode interpreter with stack-based execution

## Architecture

The GLaDOS system follows a classic compiler architecture:

```text
Source Code (.gld)
       ↓
   [Parser] → AST (Abstract Syntax Tree)
       ↓
  [Compiler] → Bytecode
       ↓
[Virtual Machine] → Execution Result
```

## Use Cases

GLaDOS is perfect for:

- Learning about compiler and VM design
- Understanding bytecode compilation
- Exploring language implementation concepts
- Educational purposes in programming language theory

## Getting Started

To get started with GLaDOS, check out the [Quick Start](./getting-started/quick-start.md) guide!
