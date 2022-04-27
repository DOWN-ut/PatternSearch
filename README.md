# PatternSearch

Le clonage de ce répertoire peut prendre quelques minutes : certains fichiers sont très lourds (i.e : séquences de chromosomes).

Vous pouvez tester notre projet en lançant directement le fichier 
  "PatternSearch/bin/client" sous Linux
ou bien
  "PatternSearch/bin/PatternSearchClient.exe" sous Windows
 
Il vous suffit ensuite de suivre les instructions qui s'affichent dans le terminal.
Vous pouvez tester différents fichiers DIPWM et différentes séquences.

Vous trouverez différentes DIPWM dans les dossiers "bin/human" et "bin/mouse".
Vous trouverez différentes séquences dans le dossier "bin/sequences".

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
Vous pouvez desinstaller la librairie SDSL de la façon suivant :

  ```sh
./uninstall.sh
```
  - Ouvrez un terminal dans le répertoire racine "PatternSearch"
  - Entrez la commande "make"
  - Entrez dans le répertoire "bin"
  - L'éxecutable est le fichier "client"

  
