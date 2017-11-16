#include "mem.h"
#include "common.h"
#include <stdio.h>

mem_fit_function_t * fonction_recherche;

//Initialisation de l'allocateur : on cree le premier fb et on met a jour l'adresse presente en debut de memoire avec l'adresse du nouveau fb
void mem_init(char* mem, size_t taille){
        *(struct fb**) mem =  (void*)mem + sizeof(struct fb*);
        struct fb* first_free_zone = *(struct fb**) mem ;
        first_free_zone->size = taille-sizeof(struct fb*);
        first_free_zone->next = NULL;
        printf("Taille : %lu , Adresse de la premiere zone libre: %p\n",(size_t)first_free_zone->size,first_free_zone);
        mem_fit(&mem_fit_first);
}

//retourne l'adresse de la zone libre à droite la plus proche, retourne NULL si il n'y en a pas.
    struct fb* trouver_voisin_droite(void * zone){
        struct fb * parcours = *((struct fb**) get_memory_adr());
        while(parcours != NULL && (void *) parcours  < zone){
            parcours = parcours->next;
        }
        return parcours;
    };


    //Renvoi l'adresse de la zone libre à gauche la plus proche de la zone "zone", NULL si il n'y en a pas.
    struct fb* trouver_voisin_gauche(void * zone){
        struct fb * parcours = *((struct fb**) get_memory_adr());
        if(zone <= (void*)parcours)
                return NULL;
        struct fb * prec = parcours;
        while(parcours != NULL && (void *) parcours < zone){
            prec = parcours;
            parcours = parcours->next;
        }
        return prec;
    };


void* mem_alloc(size_t size){

        void* mem = get_memory_adr();
        //On aligne size sur un multiple de 4
        size_t alignement = (size+sizeof(size_t))%4;
        size += alignement;

        //Size doit avoir une taille minimum, sinon probleme a la liberation
        if(size < sizeof(struct fb)){
                size = sizeof(struct fb);
        }
        //recuperation du bloc ou l'on pourra allouer notre zone
        struct fb* fb_current = (void*) fonction_recherche(*(struct fb**) mem, size);
        if (fb_current != NULL){
                struct fb * voisin_gauche = trouver_voisin_gauche((void*)fb_current);

                //Modification de la taille de la zone si necessaire : si on ne peut pas allouer de fb dans l'espace restant
                if(fb_current->size - size - sizeof(size_t) <= sizeof(struct fb)){
                    size = fb_current->size - sizeof(size_t);
                }
                //Si le bloc contigu a  droite est occupé : on ne peut pas créer de fb
                if(size == fb_current->size-sizeof(size_t)){
                    //Si on a  un voisin gauche
                    if(voisin_gauche != NULL){
                        voisin_gauche->next = (void *)fb_current->next;
                    }
                    else{
                        *(struct fb**)mem = (void *)fb_current->next;
                    }
                }
                //Si on peut allouer un fb apres la nouvelle zone allouee
                else{
                    struct fb* suivant = (void*)fb_current->next;
                    struct fb* fb_new = (void *)fb_current + sizeof(size_t) +size;
                    //Si on a un voisin gauche
                    if(voisin_gauche != NULL){
                        voisin_gauche->next = (void *)fb_new;
                    }
                    else{
                        *(struct fb**)mem = (void *)fb_new;
                    }
                    fb_new->size = fb_current->size - (size + sizeof(size_t));
                    fb_new->next = (void*)suivant;
                }
                *(size_t*)((void*)fb_current) = size;
                //printf("Prochain bloc libre en %p",*(struct fb **)mem);
                return (void *) fb_current + sizeof(size_t);
        }
        else{
                return NULL;
        }
}
//Affiche la chaine des blocs libres
void affiche_chainage(){
    printf("Affichage du chainage des zones libres : ");
    struct fb* tete = *(struct fb**)get_memory_adr();
    while(tete != NULL){
        printf(" %p => ",(void*)tete);
        tete = tete->next;
    }
    printf("\n");
}

//Renvoi true si l'adresse zone est une adresse de debut de zone allouee
int est_liberable(void * zone){
        void* mem = get_memory_adr();
        struct fb* fb = *(struct fb**) mem;
        void* addr = mem + sizeof(struct fb*);
        size_t count = sizeof(struct fb**);
        int retour = 0;
        while(count < get_memory_size() && retour == 0){
                if(addr == (void *)fb){
                        addr = addr + fb->size;
                        count += fb->size;
                        fb = fb->next;
                } else {
                    if(addr+sizeof(size_t) == zone)
                        retour =1;
                    count += *(size_t*)addr + sizeof(size_t);
                    addr += *(size_t*)addr + sizeof(size_t);
                }
        }
        return retour;
}

void mem_free(void* zone){
    if(!est_liberable(zone)){
        printf("L'adresse %p n'est pas une adresse de debut de zone allouee\n",zone);
        return;
    }
    void* mem = get_memory_adr();
    //Adresse ou est stockee la valeur de la zone a liberer
    void * ptr_taille_zone = zone - sizeof(size_t);
    size_t taille_zone = *(size_t*) ptr_taille_zone;
    size_t nouvelle_taille = taille_zone + sizeof(size_t);

    struct fb * voisin_gauche = trouver_voisin_gauche(zone);
    struct fb * voisin_droit = trouver_voisin_droite(zone);
    //Si il y a un voisin a gauche qui est colle a la zone qu'on veut liberer : fusionner avec lui
    if(voisin_gauche != NULL && (void*)voisin_gauche+voisin_gauche->size == ptr_taille_zone){
        nouvelle_taille += voisin_gauche->size;
        //Si la zone droite adjacente est libre : fusionner
        if((void *) voisin_droit == zone + taille_zone){
            nouvelle_taille+= voisin_droit->size;
            voisin_gauche->next = (void*)voisin_droit->next;
        }
        else{
                voisin_gauche->next = (void*)voisin_droit;
        }
        voisin_gauche->size = nouvelle_taille;
    }
    //Si on doit liberer le premier bloc disponible : il n'y a pas de voisin a gauche
    else if(voisin_gauche == NULL){
        struct fb* fb_new = (void*)ptr_taille_zone;
        //Si la zone droite adjacente est libre : fusionner
        if((void *) voisin_droit == zone + taille_zone){
                nouvelle_taille += voisin_droit->size;
                fb_new->next = (void*)voisin_droit->next;
        }
        else{
            fb_new->next = (void*)voisin_droit;
        }
        fb_new->size = nouvelle_taille;
        *(struct fb**) mem = (void *) fb_new;
    }
    //Si il y a un voisin a gauche mais pas colle a la zone a liberer
    else{
        struct fb* fb_new = ptr_taille_zone;
        //Si la zone droite adjacente est libre : fusionner
        if((void *) voisin_droit == zone + taille_zone){
            nouvelle_taille+= voisin_droit->size;
            fb_new->next = (void*)voisin_droit->next;
        }
        else{
            fb_new->next = (void*)voisin_droit;
        }
        fb_new->size = nouvelle_taille;
        voisin_gauche->next = (void*)fb_new;
    }
}

//Non implementee
size_t mem_get_size(void * zone){
        return *(size_t*) zone - sizeof(size_t);
}

/* Iterateur sur le contenu de l'allocateur */
void mem_show(void (*print)(void * zone, size_t size, int free)){
        //affiche_chainage();
        void* mem = get_memory_adr();
        struct fb* fb = *(struct fb**) mem;
        void* addr = mem + sizeof(struct fb*);
        size_t count = sizeof(struct fb**);
        while(count < get_memory_size()){
                if(addr == (void *)fb){
                        print(addr, fb->size-sizeof(size_t), 1);
                        addr = addr + fb->size;
                        count += fb->size;
                        fb = fb->next;
                } else {
                        print(addr, *(size_t*)addr, 0);
                        count += *(size_t*)addr + sizeof(size_t);
                        addr += *(size_t*)addr + sizeof(size_t);
                }
        }
}



void mem_fit(mem_fit_function_t* function){
    fonction_recherche = function;
}

//Renvoi l'adresse de la premiere zone ou l'on peut allouer
struct fb* mem_fit_first(struct fb* list, size_t size){
        while(list != NULL && list->size < size+sizeof(size_t)){
                list = list->next;
        }
        return list;
}

/* Si vous avez le temps */
struct fb* mem_fit_best(struct fb* list, size_t size){
        size_t best;
        struct fb * retour;
        if(list != NULL){
                best = get_memory_size();
                retour = NULL;
        }
        else{
                return NULL;
        }
        while(list != NULL){
                if(list->size >= size+sizeof(size_t) && best > list->size){
                        best = list->size;
                        retour = list;
                }
                list = list->next;
        }
        return retour;
}


struct fb* mem_fit_worst(struct fb* list, size_t size){
        size_t worst;
        struct fb * retour;
        if(list != NULL){
                worst = 0;
                retour = NULL;
        }
        else{
                return NULL;
        }
        while(list != NULL){
                if(list->size >= size+sizeof(size_t) && worst < list->size){
                        worst = list->size;
                        retour = list;
                }
                list = list->next;
        }
	printf("retour : %p\n",retour);
        return retour;
}
