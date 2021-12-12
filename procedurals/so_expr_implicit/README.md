# so_expr_implicit

Renders an implicit surface defined by a simple expression.

Parameters are the same as `implicit`, but `.field` should be set to its default: `"field"`. `min` and `max` must also be set and defines the domain.

The expression supports three variables, `x`, `y` and `z`, which are world space coordinates.