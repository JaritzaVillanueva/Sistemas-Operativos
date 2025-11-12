Queremos que el asignador mida los bloques en unidades del tipo Align (que equivale a un long), y no en unidades de tipo Header.

Así:
- Antes: p->s.size contaba cuántos Headers ocupaba el bloque.
- Ahora: p->s.size contará cuántos Aligns ocupa el bloque.

Esto reduce la fragmentación interna, porque Align es más pequeño que Header.