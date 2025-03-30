# Reorganization-of-an-optical-fiber-network

This project aims to reorganize the optical fiber network of an urban area.

The project is divided into two main parts:
- **Part 1:** Network reconstruction.
- **Part 2:** Network reorganization.

The network reconstruction was carried out using various data structures, including:
- **Linked lists**
- **Hash tables**
- **Quadtree**

This allowed us to visualize and analyze the computational time associated with each structure, enabling us to draw meaningful conclusions.

## **Project Structure and Data Structures Used**

This project is implemented in **C** as the primary programming language.

The codebase is divided into six distinct parts:

- **Network chain manipulation**  
  *<u>Associated files</u>: `Chaine.h`, `Chaine.c`, `SVGwriter.h`, `SVGwriter.c`*

- **Network reconstruction and storage using linked lists**  
  *<u>Associated files</u>: `Reseau.h`, `Reseau.c`*

- **Network reconstruction and storage using hash tables**  
  *<u>Associated files</u>: `Hachage.h`, `Hachage.c`*

- **Network reconstruction and storage using a quadtree**  
  *<u>Associated files</u>: `ArbreQuat.h`, `ArbreQuat.c`*

- **Computation time analysis for each data structure**  
  *<u>Associated file</u>: `timeCal.c`*

- **Network reorganization using an undirected graph**  
  *<u>Associated files</u>: `Graphe.h`, `Graphe.c`, `Struct_File.h`, `Struct_File.c`*

Each file is generally divided into two sections. The second section contains the core functions required for the practical sessions (**TME**), while the first section is dedicated to auxiliary functions.
