# supreme-compiler

# Autor

Paweł Rubin, 244963

# Zależności

- bison 3.0.4
- flex 2.6.4
- gcc 7.4.0

# Instalacja

```shell
$ make
```

# Sposób użycia

```shell
$ ./compiler <plik wejściowy> <plik wyjściowy>
```

# Pliki

- `typedefs.hpp` - pomocnicze definicje typów
- `parser.ypp` - główny plik, zawierający gramatykę oraz funkcję main
- `lexer.l` - plik leksera
- `data.hpp`, `data.cpp` - tablica symboli
- `code.hpp`, `code` - obsługa assemblera
- `types.hpp` - deklaracja klas reprezentujących gramatykę
- `program.cpp` - implementacja klasy `Program` reprezentującej wyprowadzenie **_program_**
- `command.cpp` - implementacja klasy `Command` reprezentującej wyprowadzenia **_command_** oraz **_commands_**
- `condition.cpp` - implementacja klasy `Condition` reprezentującej wyprowadzenie **_condition_**
- `expression.cpp` - implementacja klasy `Expression` reprezentującej wyprowadzenie **_expression_**
- `value.cpp` - implementacja klasy `Value` reprezentującej wyprowadzenie **_value_**
- `identifier.cpp` - implementacja klasy `Identifier` reprezentującej wyprowadzenie **_identifier_**
