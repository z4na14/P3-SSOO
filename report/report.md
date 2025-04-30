---
grado: Bachelor in Computer Science and Engineering
asignatura: Operating Systems
anio-academico: 2024-2025
grupo: 89
autores:
  - nombre: Jorge Adrian Saghin Dudulea
    nia: 100522257
  - nombre: Denis Loren Moldovan
    nia: 100522240
  - nombre: Antonio Nicolas Lemus Yeguas
    nia: 100522110
nombre-lab: Laboratory 3
titulo: Multi-thread programming. Control of fabrication
---
# Description of the code

## queue



## process_manager

## factory_manager

### main

After finishing processing each belt, we are going to
interchange the position of each thread from the thread 
pool by placing the unused one first.

This will be done in order to rely on the value of the
semaphore to begin processing the next belt. By using the value of the semaphore with the maximum number of
belts like:

$$
\text{Index} = \text{Max number of belts} - \text{Semaphore value} - 1
$$

Therefore, each time we process a belt, we increase the number
of the semaphore, reducing the index in the array, and using
as thread the last available one.

### tokenizar_linea
### parse_file


# Tests

# Conclusion