#include "mem.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void affiche_zone(void *adresse, size_t taille, int free)
{
    if(free)
        printf("Zone %s, Adresse : %lx, Taille : %lu\n", free?"libre":"occupee",
         (unsigned long) adresse, (unsigned long) taille);
    else
        printf("Zone %s, Adresse : %lx, Taille : %lu, Pour la desallouer : %d\n", free?"libre":"occupee",
         (unsigned long) adresse, (unsigned long) taille,(int) (adresse-get_memory_adr()+sizeof(size_t)));

}

struct composite
{
        int un;
        int deux;
        int trois;
        double quatre;
        void * addr;
};

void test1()
{
	mem_init(get_memory_adr(),get_memory_size());
    printf("\n==================================================================\n");
    printf("Test numero 1 : Cas allocation 1 : Pas de blocs libre gauche et on a la place pour ecrire un nouveau fb\n\n");
    printf("\nAffichage de la memoire a l'etat initial\n");
    mem_show(affiche_zone);
    printf("\nOn alloue une premiere zone de taille 100\n\n");
    mem_alloc(100);
    mem_show(affiche_zone);
    printf("\nOn vient de prouver que l'on peut allouer dans une zone n'ayant pas de blocs libres a gauche et ou nous avons la place d'ecrire un nouveau fb apres avoir alloue la zone\n");
    printf("\n==================================================================\n");
}

void test2()
{
	mem_init(get_memory_adr(),get_memory_size());
	printf("\n==================================================================\n");
	printf("Test numero 2 : Cas allocation 2 : Pas de blocs libre a gauche et nous n'avons pas la place pour ecrire un nouveau fb\n\n");
    printf("\nAffichage de la memoire a l'etat initial\n");
    mem_show(affiche_zone);
    printf("\nOn alloue une premiere zone de taille 100\n\n");
    void * z1 = mem_alloc(100);
    mem_show(affiche_zone);
    printf("\nOn alloue une deuxieme zone de taille 200\n\n");
    mem_alloc(200);
    mem_show(affiche_zone);
    printf("\nOn libere la premiere zone\n\n");
    mem_free(z1);
    mem_show(affiche_zone);
    printf("\nOn alloue ensuite une zone de taille suffisante (100) qui va venir combler entierement le vide cree par la liberation de la zone 1 : nous n'aurons donc pas creation de nouveau fb\n\n");
    z1 = mem_alloc(100);
    mem_show(affiche_zone);
    printf("\nOn vient de prouver que l'on peut allouer dans une zone n'ayant pas de blocs libres a gauche et ou nous n'avons pas la place d'ecrire un nouveau fb apres avoir alloue la zone\n");
    printf("\n==================================================================\n");
}

void test3()
{
	mem_init(get_memory_adr(),get_memory_size());
	printf("\n==================================================================\n");
	printf("Test numero 3 : Cas allocation 3 : Bloc libre a gauche et nous avons la place pour ecrire un nouveau fb\n\n");
    printf("\nAffichage de la memoire a l'etat initial\n");
    mem_show(affiche_zone);
    printf("\nOn alloue une premiere zone de taille 100\n\n");
    void * z1 = mem_alloc(100);
    mem_show(affiche_zone);
    printf("\nOn alloue une deuxieme zone de taille 200\n\n");
    mem_alloc(200);
    mem_show(affiche_zone);
    printf("\nOn libere la premiere zone\n\n");
    mem_free(z1);
    mem_show(affiche_zone);
    printf("\nOn alloue ensuite une autre zone de taille 300\n\n");
    z1 = mem_alloc(300);
    mem_show(affiche_zone);
    printf("\nOn vient de prouver que l'on peut allouer dans une zone ayant un bloc libre a gauche et ou nous avons la place d'ecrire un nouveau fb apres avoir alloue la zone\n");
    printf("\n==================================================================\n");
}

void test4()
{
	mem_init(get_memory_adr(),get_memory_size());
	printf("\n==================================================================\n");
    printf("Test numero 4 : Cas allocation 4 : Bloc libre a gauche et nous n'avons pas la place pour ecrire un nouveau fb\n\n");
    printf("\nAffichage de la memoire a l'etat initial\n");
    mem_show(affiche_zone);
    printf("\nOn alloue une premiere zone de taille 100\n\n");
    void * z1 = mem_alloc(100);
    mem_show(affiche_zone);
    printf("\nOn alloue une deuxieme zone de taille 200\n\n");
    mem_alloc(200);
    mem_show(affiche_zone);
    printf("\nOn alloue une troisieme zone de taille 300\n\n");
    void * z3 = mem_alloc(300);
    mem_show(affiche_zone);
    printf("\nOn alloue une quatrieme zone de taille 400\n\n");
    mem_alloc(400);
    mem_show(affiche_zone);
    printf("\nOn libere la premiere zone\n\n");
    mem_free(z1);
    mem_show(affiche_zone);
    printf("\nOn libere la troisieme zone\n\n");
    mem_free(z3);
    mem_show(affiche_zone);
    printf("\nOn alloue ensuite une autre zone de taille 300 qui va prendre la place de l'ancienne troisieme zone et ainsi combler entierement le vide qu'a laisse la liberation de cette zone\n\n");
    z1 = mem_alloc(300);
    mem_show(affiche_zone);
    printf("\nOn remarque que la zone a ete entierement remplie et que, par consequent, nous n'avons pas cree de fb\n\n");
    printf("On vient de prouver que l'on peut allouer dans une zone ayant un bloc libre a gauche et ou nous n'avons pas la place d'ecrire un nouveau fb apres avoir alloue la zone\n");
    printf("\n==================================================================\n");
}

void test5()
{
	mem_init(get_memory_adr(),get_memory_size());
	printf("\n==================================================================\n");
    printf("Test numero 5 : Cas desallocation 1 : Pas de blocs libre a gauche et fusion avec la zone libre a droite\n\n");
    printf("\nAffichage de la memoire a l'etat initial\n");
    mem_show(affiche_zone);
    printf("\nOn alloue une premiere zone de taille 100\n\n");
    void * z1 = mem_alloc(100);
    mem_show(affiche_zone);
    printf("\nOn libere cette zone\n\n");
    mem_free(z1);
    mem_show(affiche_zone);
    printf("\nOn remarque que nous sommes retourne dans l'etat initial : nous avons fusionne avec la zone libre a droite\n\n");
    printf("On vient de prouver que l'on peut desallouer une zone n'ayant pas de blocs libre a gauche et avec fusion avec la zone libre de droite\n");
    printf("\n==================================================================\n");
}

void test6()
{
	mem_init(get_memory_adr(),get_memory_size());
	printf("\n==================================================================\n");
	printf("Test numero 6 : Cas desallocation 2 : Pas de blocs libre a gauche et pas de blocs libre a droite\n\n");
    printf("\nAffichage de la memoire a l'etat initial\n");
    mem_show(affiche_zone);
    printf("\nOn alloue une premiere zone de taille 100\n\n");
    mem_alloc(100);
    printf("\nOn alloue une deuxieme zone de taille 100\n\n");
    void * z2 = mem_alloc(100);
    mem_show(affiche_zone);
    printf("\nOn libere cette zone\n\n");
    mem_free(z2);
    mem_show(affiche_zone);
    printf("On vient de prouver que l'on peut desallouer une zone n'ayant pas de blocs libre a gauche et a droite\n");
    printf("\n==================================================================\n");
}

void test7()
{
	mem_init(get_memory_adr(),get_memory_size());
	printf("\n==================================================================\n");
	printf("Test numero 7 : Cas desallocation 3 : Fusion avec un blocs libre a gauche et pas de blocs libre a droite\n\n");
    printf("\nAffichage de la memoire a l'etat initial\n");
    mem_show(affiche_zone);
    printf("\nOn alloue une premiere zone de taille 100\n\n");
    void * z1 = mem_alloc(100);
    printf("\nOn alloue une deuxieme zone de taille 100\n\n");
    void * z2 = mem_alloc(100);
    printf("\nOn alloue une troisieme zone de taille 100\n\n");
    mem_alloc(100);
    mem_show(affiche_zone);
    printf("\nOn libere la premiere zone\n\n");
    mem_free(z1);
    mem_show(affiche_zone);
    printf("\nOn libere la deuxieme zone\n\n");
    mem_free(z2);
    mem_show(affiche_zone);
    printf("On vient de prouver que l'on peut desallouer une zone ayant un bloc libre a gauche et pas de blocs libre a droite\n");
    printf("\n==================================================================\n");
}

void test8()
{
	mem_init(get_memory_adr(),get_memory_size());
	printf("\n==================================================================\n");
	printf("Test numero 8 : Cas desallocation 4 : Fusion avec un blocs libre a gauche et a droite\n\n");
    printf("\nAffichage de la memoire a l'etat initial\n");
    mem_show(affiche_zone);
    printf("\nOn alloue une premiere zone de taille 100\n\n");
    mem_alloc(100);
    printf("\nOn alloue une deuxieme zone de taille 100\n\n");
    void * z2 = mem_alloc(100);
    printf("\nOn alloue une troisieme zone de taille 100\n\n");
    void * z3 = mem_alloc(100);
    mem_show(affiche_zone);
    printf("\nOn libere la deuxieme zone\n\n");
    mem_free(z2);
    mem_show(affiche_zone);
    printf("\nOn libere la troisieme zone\n\n");
    mem_free(z3);
    mem_show(affiche_zone);
    printf("On vient de prouver que l'on peut desallouer une zone ayant un bloc libre a gauche et a droite\n");

    printf("\n==================================================================\n");
}

void test_best_fit()
{
	mem_init(get_memory_adr(),get_memory_size());
	mem_fit(&mem_fit_best);
	printf("\n==================================================================\n");
	printf("\nTest de la fonction best_fit\n");
	printf("\nAffichage de la memoire a l'etat initial\n");
	mem_show(affiche_zone);
	printf("\nOn alloue une premiere zone de taille 300\n\n");
	void * z1 = mem_alloc(300);
	printf("\nOn alloue une deuxieme zone de taille 300\n\n");
	mem_alloc(300);
	printf("\nOn alloue une troisieme zone de taille 100\n\n");
	void * z3 = mem_alloc(100);
	printf("\nOn alloue une quatrieme zone de taille 100\n\n");
	mem_alloc(100);
	printf("\nOn libere la premiere zone\n\n");
	mem_free(z1);
	mem_show(affiche_zone);
	printf("\nOn libere la troisieme zone\n\n");
	mem_free(z3);
	mem_show(affiche_zone);

	printf("\nOn va allouer une zone de taille 50, best fit va se charger de l'allouer dans un bloc ou le residu sera minimal i.e dans l'ancienne zone de taille 100 que l'on a liberee\n\n");
	mem_alloc(50);
	mem_show(affiche_zone);
	printf("\nOn remarque qu'on a bien eu le comportement decrit plus haut : la politique best_fit est donc fonctionelle");

    printf("\n==================================================================\n");
}

void test_worst_fit()
{
	mem_init(get_memory_adr(),get_memory_size());
	mem_fit(&mem_fit_worst);
	printf("\n==================================================================\n");
	printf("\nTest de la fonction worst_fit\n");
	printf("\nAffichage de la memoire a l'etat initial\n");
	mem_show(affiche_zone);
	printf("\nOn alloue une premiere zone de taille 300\n\n");
	void * z1 = mem_alloc(300);
	printf("\nOn alloue une deuxieme zone de taille 300\n\n");
	mem_alloc(300);
	printf("\nOn alloue une troisieme zone de taille 100\n\n");
	void * z3 = mem_alloc(100);
	printf("\nOn alloue une quatrieme zone de taille 100\n\n");
	mem_alloc(100);
	printf("\nOn libere la premiere zone\n\n");
	mem_free(z1);
	mem_show(affiche_zone);
	printf("\nOn libere la troisieme zone\n\n");
	mem_free(z3);
	mem_show(affiche_zone);

	printf("\nOn va allouer une zone de taille 50, worst fit va se charger de l'allouer dans un bloc ou le residu sera maximal i.e dans le reste de la memoire\n\n");
	mem_alloc(50);
	mem_show(affiche_zone);
	printf("\nOn remarque qu'on a bien eu le comportement decrit plus haut : la politique worst_fit est donc fonctionelle");

    printf("\n==================================================================\n");
}

void test_special(){

	mem_init(get_memory_adr(),get_memory_size());
	printf("\n==================================================================\n");
	printf("Test special : on simule l'execution d'un programme\n\n");

        //On ecrit un double en memoire.
	printf("On alloue un premier double de valeur 80 dans notre allocateur\n");
        double* double1 = (double *) mem_alloc(sizeof(double));
        if (double1 == NULL)
                printf("Echec de l'allocation\n");
        else{
                *double1 = 80.0;
                printf("Affichage du double  : %f\n", *double1);
        }
	printf("Affichage de la memoire avec le double alloue :\n\n");
        mem_show(affiche_zone);

        //On ecrit un second double en memoire.
	printf("On alloue un deuxieme double de valeur -10000 dans notre allocateur\n");
        double * double2 = (double *) mem_alloc(sizeof(double));
        if (double2 == NULL)
                printf("Echec de l'allocation\n");
        else{
                *double2 = -10000.0;
                printf("\nAffichage du double : %f\n", *double2);
        }
	printf("Affichage de la memoire avec les doubles alloues :\n\n");
        mem_show(affiche_zone);



	printf("\nOn alloue une phrase \"Une phrase assez longue\"dans notre allocateur\n");
        char * phrase = (char *)  mem_alloc(sizeof("Une phrase assez longue\0"));
        if(phrase != NULL){
                strcpy(phrase,"Une phrase assez longue\0");
                printf("Affichage de la phrase : %s\n\n",phrase);
        }

	printf("Affichage de la memoire avec les doubles alloues et la phrase alloue:\n\n");
        mem_show(affiche_zone);

        //On ecrit une structure en memoire
	printf("\nOn alloue une structure complexe(int,int,int,double,void*) dans notre allocateur\n");

        struct composite * comp = (struct composite *) mem_alloc(sizeof(struct composite));
        if(comp != NULL){
                comp->un = 1;
                comp->deux = 2;
                comp->trois = 3;
                comp->quatre = 4.0;
                comp->addr = (void*)comp-sizeof(size_t);
        }
        printf("\nInformations dans notre structure : %d et %d et %d et %f et %p\n", comp->un,comp->deux,comp->trois,comp->quatre,comp->addr);
	printf("\nAffichage de la memoire avec les doubles alloues, la phrase alloue et la structure composite:\n\n");
        mem_show(affiche_zone);
	
	printf("\nLiberation de tout ce que l'on a alloue : on retourne dans l'etat initial\n\n");
        mem_free(double1);
        mem_free(double2);
        mem_free(phrase);
        mem_free(comp);
        mem_show(affiche_zone);


	printf("\n==================================================================\n");

}

int main()
{
        //Cas d'allocations
        test1();
        test2();
        test3();
        test4();
        //Cas de desallocations
        test5();
        test6();
        test7();
        test8();
	test_best_fit();
	test_worst_fit();
	test_special();

}
