#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "getopt.h"
#include "cli.h"

#include "libsteg.h"


void we_are_stegx(){
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
  printf("Faire \033[01m\033[31mstegx --help\033[0m ou \033[01m\033[31mstegx -h \033[0m pour un rappel des commandes.\n\n\n");
}

void help(){
    printf("Manuel d'utilisation de stegx\n\n");
    printf("\033[01m\033[32m-d, --dissimule \033[0m\n\t Option de dissimultation.\n\n"
		    "\033[01m\033[32m-e, --extraire \033[0m\n\t Option d'extraction.\n\n"
		    "\033[01m\033[32m-o, --hote [nom_fichier]\033[0m\n\t Le nom du fichier hote.\n\n"
		    "\033[01m\033[32m-c, --cache [nom_fichier]\033[0m (si option de dissimulation sélectionnée)"
		    "\n\t Le nom du fichier à cacher.\n\n"
		    "\033[01m\033[32m-r, --resultat [nom_fichier]\033[0m\n\t Le nom du fichier où stocker le résultat.\n\n"
		    "\033[01m\033[32m-p, --password [password]\033[0m (optionnel)\n\t Pour ajouter un mot de passe permettant "
		    "de rajouter une protection à la dissimulation.\n\n"
		    "Voici les différents algorithmes que propose l'application :\n"
		    "\033[01m\033[32m--lsb\033[0m : least significant bit. \n"
		    "\033[01m\033[32m--metadata\033[0m \n"
		    "\033[01m\033[32m--EOF\033[0m : end of file.\n");

   printf("\n");
   exit(0); 
}

void unvalid_line(){
  printf("la ligne de commande est invalide\n\n\n");
  help();
}






void fill_info(stegx_info_t* com,const int argc,char* const* argv){

    int optch;
    extern int opterr;

    char short_option[] = "o:c:r:p:deh"; // correspond au format taper en ligne de commande,  x: signifie qu'un argument est attendu après -x  
                  //o: pour l'option -o suivi du fichier hote
                  //c: pour l'option -c suivi du fichier à cacher 
                  //r: pour l'option -r suivi du fichier resultat
                  //p: pour l'option -p suivi du mot de passe 
                  //d pour l'option -d pour signifier la dissimulation
                  //e pour l'option -e pour signifier l'extraction
                  //h pour l'option -h qui affiche l'aide
     const struct option long_option[] = {
        {"hote", 1, NULL, 'o'},
        {"cache", 1, NULL, 'c'},
        {"resultat", 1, NULL, 'r'},
        {"dissimule", 0, NULL, 'd'},
        {"extraire", 0, NULL, 'e'}, 
        {"lsb",0, NULL,STEGX_ALGO_LSB},
        {"metadata",0,NULL,STEGX_ALGO_METADATA},
        {"EOF",0,NULL, STEGX_ALGO_EOF},
        {"password", 1, NULL, 'p'},
        {"help",0,NULL,'h'},
        {NULL, 0, NULL, 0}
};
    opterr = 1;
    int optindex = 0;

    while ((optch = getopt_long(argc, argv, short_option, long_option, &optindex)) != -1)
    switch (optch) {
		case 'd':
            com->mode=STEGX_MODE_INSERT;
            com->ins_info = malloc(sizeof(stegx_info_ins_t));
            break;
        case 'e':
             com->mode=STEGX_MODE_EXTRACT;
            break;
        case 'o':
            com->host_path=optarg;
            break;
        case 'c':
            com->ins_info->hidden_path=optarg;
            break; 
        case 'r':
            com->res_path=optarg;
            break;
        case 'p':
             com->passwd=optarg;
        case STEGX_ALGO_LSB:
             com->ins_info->algo=STEGX_ALGO_LSB;
             break;
        case STEGX_ALGO_METADATA:
             com->ins_info->algo=STEGX_ALGO_METADATA;
             break;           
        case STEGX_ALGO_EOF:
             com->ins_info->algo=STEGX_ALGO_EOF;
             break;
        case 'h':
             help();
             break;
        case '?':
              unvalid_line();
              break;
    };
 
}

void check_info(stegx_info_t* com){
  if (com->mode==STEGX_MODE_INSERT){
    if((com->ins_info->hidden_path!="\0") && (com->ins_info->algo!=-1)){
      printf("stegx_check_compatibility_dissimulation(com);");
     
    }
  }
  else if (com->mode==STEGX_MODE_EXTRACT){
      printf("stegx_check_compatibility_extraction(com);");
  }
  else {unvalid_line();}
  }
//test 

