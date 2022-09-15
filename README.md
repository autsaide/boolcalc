[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-c66648af7eb3fe8bc4f294546bfd86ef473780cde1dea487d3c4ff354943c9ae.svg)](https://classroom.github.com/online_ide?assignment_repo_id=7966256&assignment_repo_type=AssignmentRepo)
# Boolean calculator

Variables: `xN`, `N` is natural numbers

Operations:
* `&` And
* `v` Or
* `~` Not
* `>` Implication (x → y)
* `<` Reverse implication (x ← y)
* `+` XOR
* `=` Equal
* `|` штрих Шеффера
* `^` Pierce arrow


The input file contains a system of functions

* `boolcalc -calc input output` calculate functions inside of `input`, put results into `output`
* `boolcalc -cnf input output` build conjunctive normal forms for functions inside of `input` put results into `output` 
* `boolcalc -dnf input output` build disjunctive normal forms for functions inside of `input` put results into `output`
* `boolcalc -zh input output`  build Zhegalkin polynomials for functions inside of `input` put results into `output`
* `boolcalc -table input` print truth tables for functions inside of `input`
* `boolcalc -isfull input output` check the system of function inside `input` for completeness 
* `boolcalc -h` or `boolcalc ?` help
