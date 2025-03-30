all: ReconstitueReseau GrapheMain timeCal

ReconstitueReseau: ReconstitueReseau.c Chaine.o SVGwriter.o Reseau.o Hachage.o ArbreQuat.o
	gcc -Wall -o ReconstitueReseau $^ -lm

GrapheMain: GrapheMain.c Graphe.o Chaine.o SVGwriter.o Reseau.o Hachage.o ArbreQuat.o Struct_File.o TableH.o
	gcc -Wall -o GrapheMain $^ -lm

timeCal: timeCal.c Chaine.o SVGwriter.o Reseau.o Hachage.o ArbreQuat.o
	gcc -Wall -o timeCal $^ -lm


Chaine.o: Chaine.h SVGwriter.h Chaine.c SVGwriter.c
	gcc -Wall -c Chaine.c SVGwriter.c

SVGwriter.o: SVGwriter.h SVGwriter.c
	gcc -Wall -c SVGwriter.c

Reseau.o: Reseau.h Chaine.h SVGwriter.h Reseau.c Chaine.c SVGwriter.c
	gcc -Wall -c Reseau.c Chaine.c SVGwriter.c

Hachage.o: Hachage.h Reseau.h Chaine.h SVGwriter.h Hachage.c Reseau.c Chaine.c SVGwriter.c
	gcc -Wall -c Hachage.c Reseau.c Chaine.c SVGwriter.c

ArbreQuat.o: ArbreQuat.h Reseau.h Chaine.h SVGwriter.h ArbreQuat.c Reseau.c Chaine.c SVGwriter.c
	gcc -Wall -c ArbreQuat.c Reseau.c Chaine.c SVGwriter.c

Graphe.o: Graphe.h Reseau.h Chaine.h Struct_File.h TableH.h SVGwriter.h Graphe.c Reseau.c Chaine.c Struct_File.c TableH.c SVGwriter.c
	gcc -Wall -c Graphe.c Reseau.c Chaine.c Struct_File.c TableH.c SVGwriter.c

Struct_File.o: Struct_File.h Struct_File.c
	gcc -Wall -c Struct_File.c

TableH.o: TableH.h Graphe.h Reseau.h TableH.c Graphe.c Reseau.c
	gcc -Wall -c TableH.c Graphe.c Reseau.c -lm

clean:
	rm -f *.o ReconstitueReseau timeCal
