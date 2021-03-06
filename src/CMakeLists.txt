# This file is part of the StegX project.
# Copyright (C) 2018  StegX Team
# 
# StegX is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

# Auteur : Pierre AYOUB
# ==============================================================================

# Modules externes =============================================================

# Informations sur la compilation.
include (info)
# Génération des tags des fichiers sources.
include (tags)

# Communs ======================================================================

# Noms des différents composants :
# Bibliothèque StegX.
set (SRC_LIB_COMP "library_stegx")
# Interface en ligne de commande.
set (SRC_CLI_COMP "interface_command_line")
# Interface graphique.
set (SRC_GUI_COMP "interface_graphical")

# Installation =================================================================

# Dossiers standards GNU pour l'installation.
include (GNUInstallDirs)

# Assigne RPATH au dossier d'installation de la bibliothèque.
if (UNIX)
    set (CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR};
    ./${CMAKE_INSTALL_LIBDIR};../${CMAKE_INSTALL_LIBDIR})
endif()

# Communs ======================================================================

# Lecture des sous-répertoires.
add_subdirectory (${SRC_LIB_DIR})
add_subdirectory (${SRC_CLI_DIR})
if (NOT MSVC)
    add_subdirectory (${SRC_GUI_DIR})
endif ()

# Configuration pour toutes les cibles.
foreach (target ${SRC_ALL_TARGETS})
    if (TARGET ${target})
        # Génération des tags.
        if (TAG_GEN)
            add_dependencies (${target} tags)
        endif()
        # Informations de compilation.
        add_dependencies (${target} info)
        # Dossier de sortie sur Windows.
        if (WIN32)
            set_target_properties (${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${SRC_RUNTIME_OUTPUT_PATH})
        endif()
    endif ()
endforeach ()

# Package ======================================================================

# Configuration par système d'exploitation :
# Unix.
if (UNIX)
    # Liste des générateurs.
    set (CPACK_GENERATOR TGZ;DEB;STGZ)

    # Configuration pour DEB :
    # Auteurs du programme.
    set (CPACK_DEBIAN_PACKAGE_MAINTAINER "AYOUB Pierre, BASKEVITCH Claire, BESSAC Tristan, \n \
        CAUMES Clément, DELAUNAY Damien, DOUDOUH Yassin")
    # Site du logiciel.
    set (CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/Heisenberk/StegX")
    # Résolution des dépendances.
    set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    # Liste des dépendances.
    set (CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libgcc1, libgtk-3-0, \
        libgtk-3-common, libgtk-3-bin")

# Windows.
elseif (WIN32)
    # Cherche si le générateur NSIS est installé.
    find_program (NSIS_EXECUTABLE makensisw HINTS "C:\\Program Files (x86)\\NSIS")
    if (NSIS_EXECUTABLE)
        set (NSIS_GEN NSIS)
    endif ()

    # Liste des générateurs.
    set (CPACK_GENERATOR ZIP;${NSIS_GEN})

    # Configuration pour NSIS :
    # Icônes de l'installateur/désinstallateur.
    set (CPACK_NSIS_MUI_ICON "${DESKTOP_PATH}/${PROJECT_NAME_LC}-64x64.ico")
    set (CPACK_NSIS_MUI_UNIICON "${DESKTOP_PATH}/${PROJECT_NAME_LC}-64x64.ico")
    set (CPACK_PACKAGE_ICON "${DESKTOP_PATH}\\\\${PROJECT_NAME_LC}-128x320.bmp")
    set (CPACK_NSIS_INSTALLED_ICON_NAME Uninstall.exe)
    # Permet d'ajouter le répertoire contenant les binaires dans la variable
    # d'environment PATH.
    set (CPACK_NSIS_MODIFY_PATH ON)
    if (TARGET ${SRC_GUI_TARGET})
        # Créé/supprime les raccourcis du menu démarrer et du bureau vers l'interface graphique.
        set (CPACK_NSIS_CREATE_ICONS
            "CreateShortCut '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk' '\$INSTDIR\\\\${CMAKE_INSTALL_BINDIR}\\\\${SRC_GUI_TARGET}.exe' \
                '' '\$INSTDIR\\\\${PROJECT_NAME_LC}.ico' \n \
             CreateShortCut '$DESKTOP\\\\${PROJECT_NAME}.lnk' '\$INSTDIR\\\\${CMAKE_INSTALL_BINDIR}\\\\${SRC_GUI_TARGET}.exe' \
                '' '\$INSTDIR\\\\${PROJECT_NAME_LC}.ico' \
            ")
        set (CPACK_NSIS_DELETE_ICONS
            "Delete '\$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\${PROJECT_NAME}.lnk' \n \
             Delete '$DESKTOP\\\\${PROJECT_NAME}.lnk' \
            ")
        # Permet de lancer le programme à la fin de l'installation.
        set (CPACK_NSIS_EXECUTABLES_DIRECTORY "${CMAKE_INSTALL_BINDIR}")
        set (CPACK_NSIS_MUI_FINISHPAGE_RUN "${SRC_GUI_TARGET}.exe")
    endif ()
    # Désinstalle une version précédente si présente.
    set (CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    # Nom de l'installateur et du dossier d'installation
    set (CPACK_NSIS_DISPLAY_NAME ${PROJECT_NAME})
    set (CPACK_NSIS_PACKAGE_NAME ${PROJECT_NAME})
    set (CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
    # Préfixe d'installation.
    set (CPACK_NSIS_INSTALL_ROOT "${CMAKE_INSTALL_PREFIX}")
    get_filename_component (CPACK_NSIS_INSTALL_ROOT "${CPACK_NSIS_INSTALL_ROOT}" DIRECTORY)
    string (REPLACE "/" "\\\\" CPACK_NSIS_INSTALL_ROOT "${CPACK_NSIS_INSTALL_ROOT}")
endif ()

# Configuration générale puis lancement de CPack.
if (UNIX OR WIN32)
    # Répertoire de destination.
    set (PACKAGE_TARGET dist)
    set (CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/${PACKAGE_TARGET})

    # Configuration :
    # License.
    set (PROJECT_FILE_LICENSE COPYING)
    set (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/${PROJECT_FILE_LICENSE})
    # Éditeur.
    set (CPACK_PACKAGE_VENDOR "StegX Team")
    # Description :
    set (CPACK_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})
    set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "\t \n \
        StegX est un logiciel de steganographie, \n \
        compose de deux interfaces ainsi que d'une bibliotheque. \n \
        Le logiciel prend en charge de multiples formats de fichier \n \
        de type image, son et video.")

    # Lance la configuration de CPack.
    include (CPack)
    
    # Composants :
    # Bibliothèque StegX.
    cpack_add_component (${SRC_LIB_COMP}
        DISPLAY_NAME "Bibliotheque StegX"
        DESCRIPTION "Bibliotheque StegX de Steganographie."
        REQUIRED
        )
    # Interface en ligne de commande.
    cpack_add_component (${SRC_CLI_COMP}
        DISPLAY_NAME "Interface en ligne de commande"
        DESCRIPTION "Interface en ligne de commande permettant d'utiliser la bibliotheque StegX."
        )
    # Interface graphique.
    if (TARGET ${SRC_GUI_TARGET})
        cpack_add_component (${SRC_GUI_COMP}
            DISPLAY_NAME "Interface graphique"
            DESCRIPTION "Interface graphique permettant d'utiliser la bibliotheque StegX."
            )
    endif ()
    
    # Cible personnalisée pour créer les packages (pour mettre les cibles en
    # dépendances et lancer une désinstallation complète contre les fichiers
    # résidus).
    add_custom_target (${PACKAGE_TARGET}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target package
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target dist-cleanup
        DEPENDS ${SRC_ALL_TARGETS}
        )

    # Nettoyage.
    set_property (DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
        ${CPACK_PACKAGE_DIRECTORY}
        )
endif ()

# Désinstallation ==============================================================

# Cible de désinstallation.
include (uninstall-target)
