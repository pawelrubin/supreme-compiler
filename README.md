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

```
$ ./compiler <plik wejsciowy> <plik wyjsciowy>
```

# Pliki

- `typedefs.hpp` - pomocnicze definicje typów
- `parser.ypp` - główny plik, zawierający gramatykę oraz funkcję main.
- `lexer.l` - plik leksera
- `data.hpp`, `data.cpp` - tablica symboli
- `code.hpp`, `code` - obsługa assemblera
- `types.hpp` - deklaracja klas reprezentujących gramatykę
- `program.cpp` - imlementacja klasy `Program` reprezentującej wyprowadzenie **_program_**
- `command.cpp` - imlementacja klasy `Command` reprezentującej wyprowadzenia **_command_** oraz **_commands_**
- `condition.cpp` - imlementacja klasy `Condition` reprezentującej wyprowadzenie **_condition_**
- `expression.cpp` - imlementacja klasy `Expression` reprezentującej wyprowadzenie **_expression_**
- `value.cpp` - imlementacja klasy `Value` reprezentującej wyprowadzenie **_value_**
- `identifier.cpp` - imlementacja klasy `Identifier` reprezentującej wyprowadzenie **_identifier_**
