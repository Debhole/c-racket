# C-Racket

A pure-nonsense programming language derived from the Lisp variant Racket.
This was developed as a learning endeavor to greater understand the inner workings of programming languages,
and is not suitable for any real projects.

---

## Language Features

- Custom Variables and Functions
- Control Flow
- Function Scope
- Dynamically-typed Language
- Primitive data types (int, double, string, boolean)

---

```
(define (str-repeat str i)
    (if (= i 0)
        ""
        (string-append str (str-repeat str (- i 1)))))
```

---

## Building for GNU/MinGW

In the project root directory
```
cmake -S . -B build
(cd build && make)
```

## Running the C-Racket Interpreter

Run the generated executable (```c_racket.exe``` on Windows and ```c_racket``` on Linux).
C-Racket accepts 0 or more arguments. C-Racket will execute the files passed as arguments.
