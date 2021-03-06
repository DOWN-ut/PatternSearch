# PatternSearch

Le clonage de ce répertoire peut prendre quelques minutes : certains fichiers sont très lourds (i.e : séquences de chromosomes).

Vous pouvez tester notre projet en lançant directement le fichier 
  "PatternSearch/bin/client" sous Linux
ou bien
  "PatternSearch/bin/PatternSearchClient.exe" sous Windows
 
Il vous suffit ensuite de suivre les instructions qui s'affichent dans le terminal.
 
Vous trouverez différentes DIPWM dans les dossiers "bin/human" et "bin/mouse".

Vous trouverez différentes séquences dans le dossier "bin/sequences" (les fichiers .gz sont des séquences compressées, qu'il est nécessaire de décompresser avant utilisation)

Vous pouvez tester différents fichiers DIPWM et différentes séquences. Par exemple :
  - Une DIPWM humaine quelconque (i.e : "human/ESR2_HUMAN.H11DI.0.A.dpwm") avec la séquence du chromosome 15 humain ("sequences/chr15.fa").
  - Une DIPWM de souris quelconque avec la séquence du chromosome 1 de la souris.
  - Une DIPWM de souris avec la séquence d'un chromosome humain.
  - ect ...
  
La recherche peut être très longue à s'effectuer (surtout avec beaucoups de mots / sur une longue séquence). N'hésitez donc pas à CTRL-c pour interrompre l'éxecution du programme.

Pour compiler vous même le projet :

Windows :
-----------
  - Vous devez cloner ce répertoire Git directement avec Visual Studio
  - Il peut être nécessaire de lier les deux sous-projets (PatternSearchClient et PatternSearchLib), pour que le client (qui include la librairie) puis être compilé correctement
  - L'éxécutable se trouve dans le répertoire "Debug" ou bien "x64/Debug"
  - Vous pouvez aussi utiliser le MakeFile (mais vous devez avoir "cmake" ou un équivalent installé sur votre machine.

Linux :
-----------
  Avant toute chose, il faut installer la librairie SDSL si vous souhaitez l'utiliser 
  (Cette librairie n'est fonctionnelle que sous linux)

  - Ouvrez un terminal dans le répertoire "/PatternSearchLib/sdsl-source"
  - Lancer l'installation de la SDSL :

```sh
./install.sh /usr/local/
```

La librairie est désormais installée. 
Vous pouvez desinstaller la librairie SDSL de la façon suivante :

  ```sh
./uninstall.sh
```
  - Ouvrez un terminal dans le répertoire racine "PatternSearch"
  - Entrez la commande "make"
  - Entrez dans le répertoire "bin"
  - L'éxecutable est le fichier "client"

  
