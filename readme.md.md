**Custom C++ Math Calculator**

A desktop calculator built using C++ and the Qt 6 Framework. It features a native parsing engine that implements Dijkstra's Shunting-Yard Algorithm to handle operator precedence and nested parentheses from scratch, avoiding external script engines.



**Features**

Native Engine: Parses and evaluates math expressions directly in C++.



Precedence Rules: Respects standard algebraic order of operations.



Nested Bracket Support: Fully resolves complex expressions like (3 + 5) \* (2 / (4 - 2)).



Error Bounds: Catches syntax errors and division-by-zero exceptions safely.



**Mobile UI Layout:** Emulates a standard phone calculator layout using a grid engine.



**Technical Pipeline**

Tokenization: Splits input text into discrete numbers, operators, and brackets.



Shunting-Yard Transformation: Converts standard human-readable format (Infix) into Reverse Polish Notation (Postfix) via an operator stack.



Evaluation: Computes the Postfix expression using an operand stack to return the final value.



**Build \& Run**

**Prerequisites**

C++17 compiler, CMake (3.16+), and Qt 6 SDK (Widgets).



**Build Commands:**

git clone https://github.com/yourusername/custom-cpp-calculator.git

cd custom-cpp-calculator

cmake -B build\_release -S . -DCMAKE\_BUILD\_TYPE=Release

cmake --build build\_release --config Release

cd build\_release

windeployqt calculator.exe



**Concepts Demonstrated**

DSA: Practical use of Stacks, Vectors, and String Parsing.



OOP Architecture: Complete decoupling of math logic from the UI class.



Event-Driven GUI: Asynchronous routing via Qt Signals, Slots, and modern lambdas.



Release Tooling: Optimized binary compilation and dependency deployment.

