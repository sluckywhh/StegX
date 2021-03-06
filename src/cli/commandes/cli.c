/*
 * This file is part of the StegX project.
 * Copyright (C) 2018  StegX Team
 * 
 * StegX is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file cli.h
 * @brief Traitement du lancement en ligne de commande
 * @details Traitement du lancement en ligne de commande vérifiant les paramètres rentrer par l'utilisateur.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include "cli.h"
#include "stegx_common.h"

stegx_choices_s *init_stegx_info()
{
    stegx_choices_s *com = calloc(1, sizeof(stegx_choices_s));
    com->host_path = NULL;
    com->res_path = "stdout\0";
    com->insert_info = calloc(1, sizeof(stegx_info_insert_s));
    com->insert_info->hidden_path = "stdin\0";
    com->insert_info->algo = STEGX_ALGO_EOF;
    com->mode = -1;
    return com;
}

void we_are_stegx()
{
    printf("   _____ _            __   __\n");
    printf("  / ____| |           \\ \\ / /\n");
    printf(" | (___ | |_ ___  __ _ \\ V / \n");
    printf("  \\___ \\| __/ _ \\/ _` | > <  \n");
    printf("  ____) | ||  __/ (_| |/ . \\ \n");
    printf(" |_____/ \\__\\___|\\__, /_/ \\_\\\n");
    printf("                  __/ |      \n");
    printf("                 |___/   \n\n");
    printf("StegX - Steganographie & Steganalyse \n");
    printf("Université Versailles Saint Quentin en Yvelines - 2018 \n");
    printf("AYOUB Pierre - BASKEVITCH Claire - BESSAC Tristan - CAUMES Clément - ");
    printf("DELAUNAY Damien - DOUDOUH Yassin\n\n");
    printf
        ("Faire \033[01m\033[31mstegx --help\033[0m ou \033[01m\033[31mstegx -h \033[0m pour un rappel des commandes.\n\n\n");
    exit(0);
}

void help()
{
    printf("Manuel d'utilisation de stegx\n\n");
    printf
        ("\033[01m\033[32m-a, --about \033[0m\n\t A propos de l'application et de ses concepteurs\n\n"
         "\033[01m\033[32m-i, --insert \033[0m\n\t Option de dissimulation.\n\n"
         "\033[01m\033[32m-e, --extract \033[0m\n\t Option d'extraction.\n\n"
         "\033[01m\033[32m-o, --host [nom_fichier]\033[0m\n\t Le nom du fichier hote.\n\n"
         "\033[01m\033[32m-c, --hide [nom_fichier]\033[0m (si option de dissimulation sélectionnée)"
         "\n\t Le nom du fichier à cacher.\n\n"
         "\033[01m\033[32m-r, --result [nom_fichier]\033[0m\n\t Le nom du fichier où stocker le résultat (pour l'insertion) ou le dossier où stocker le fichier résultat (pour l'extraction).\n\n"
         "\033[01m\033[32m-p, --password [password]\033[0m (optionnel)\n\t Pour ajouter un mot de passe permettant "
         "de rajouter une protection à la dissimulation.\n\n"
         "Voici les différents algorithmes que propose l'application (si option de dissimulation sélectionnée) :\n"
         "\033[01m\033[32m--lsb\033[0m : least significant bit. Disponible pour BMP et PNG.\n"
         "\033[01m\033[32m--metadata\033[0m : Disponible pour BMP et PNG.\n"
         "\033[01m\033[32m--eof\033[0m : end of file. Disponible pour BMP, PNG, WAVE et FLV. (par défaut si aucun algorithme n'a été sélectionné.\n"
         "\033[01m\033[32m--eoc\033[0m : End of Chunk. "
         "Cache les données à la fin de chaque chunk. Disponible pour le format FLV.\n"
         "\033[01m\033[32m--junk\033[0m : Junk-chunk. "
         "Cache les données dans des chunks \"poubelles\". Disponible pour le format AVI.\n");
    printf("\n");
    exit(0);
}

void unvalid_line(char *error_info)
{
    printf("la ligne de commande est invalide : %s\n", error_info);
}

void fill_info(stegx_choices_s * com, const int argc, char *const *argv)
{
    if (argc == 1) {
        we_are_stegx();
    }
    int optch;
    extern int opterr;

    char short_option[] = "o:c:r:p:ieha";       // correspond au format taper en ligne de commande,  x: signifie qu'un argument est attendu après -x  
    //o: pour l'option -o suivi du fichier hote
    //c: pour l'option -c suivi du fichier à cacher 
    //r: pour l'option -r suivi du fichier resultat
    //p: pour l'option -p suivi du mot de passe 
    //d pour l'option -i pour signifier la dissimulation
    //e pour l'option -e pour signifier l'extraction
    //h pour l'option -h qui affiche l'aide
    const struct option long_option[] = {
        {"host", 1, NULL, 'o'},
        {"hide", 1, NULL, 'c'},
        {"result", 1, NULL, 'r'},
        {"insert", 0, NULL, 'i'},
        {"extract", 0, NULL, 'e'},
        {"lsb", 0, NULL, STEGX_ALGO_LSB},
        {"metadata", 0, NULL, STEGX_ALGO_METADATA},
        {"eof", 0, NULL, STEGX_ALGO_EOF},
        {"eoc", 0, NULL, STEGX_ALGO_EOC},
        {"junk", 0, NULL, STEGX_ALGO_JUNK_CHUNK},
        {"password", 1, NULL, 'p'},
        {"help", 0, NULL, 'h'},
        {"about", 0, NULL, 'a'},
        {NULL, 0, NULL, 0}
    };
    opterr = 1;
    int optindex = 0;

    while ((optch = getopt_long(argc, argv, short_option, long_option, &optindex)) != -1)
        switch (optch) {
            case 'i':
                com->mode = STEGX_MODE_INSERT;
                break;
            case 'e':
                com->mode = STEGX_MODE_EXTRACT;
                if (com->host_path == NULL)
                    com->host_path = "stdin\0";
                break;
            case 'o':
                com->host_path = optarg;
                break;
            case 'c':
                com->insert_info->hidden_path = optarg;
                break;
            case 'r':
                com->res_path = optarg;
                break;
            case 'p':
                com->passwd = optarg;
                break;
            case STEGX_ALGO_LSB:
                com->insert_info->algo = STEGX_ALGO_LSB;
                break;
            case STEGX_ALGO_METADATA:
                com->insert_info->algo = STEGX_ALGO_METADATA;
                break;
            case STEGX_ALGO_EOF:
                com->insert_info->algo = STEGX_ALGO_EOF;
                break;
            case STEGX_ALGO_EOC:
                com->insert_info->algo = STEGX_ALGO_EOC;
                break;
            case STEGX_ALGO_JUNK_CHUNK:
                com->insert_info->algo = STEGX_ALGO_JUNK_CHUNK;
                break;
            case 'h':
                help();
                break;
            case 'a':
                we_are_stegx();
                break;
            case '?':
                unvalid_line("Options inconnues\n");
                exit(-1);
                break;
        };
}

void check_info(stegx_choices_s * com)
{
    if (com->mode == STEGX_MODE_INSERT) {
        if ((com->host_path == NULL)) {
            unvalid_line("Fichier hôte ou algorithme non renseigné.\n");
            exit(-1);
        }
    } else if (com->mode == STEGX_MODE_EXTRACT) {
    } else {
        unvalid_line("");
        exit(-1);
    }
}

void dest_stegx_info(stegx_choices_s * com)
{
    if (com->insert_info) {
        free(com->insert_info);
    }
    free(com);
}
