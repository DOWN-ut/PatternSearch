# PatternSearch

Vous pouvez tester notre projet en lançant directement le fichier 
  "PatternSearch/bin/client" sous Linux
ou bien
  "PatternSearch/bin/PatternSearchClient.exe" sous Windows
 
Il vous suffit ensuite de suivre les instructions qui s'affichent dans le terminal.
Vous pouvez tester différents fichiers DIPWM et différentes séquences.

Pour compiler vous même le projet :

Windows :
  - Vous devez cloner ce répertoire Git directement avec Visual Studio
  - Il peut être nécessaire de lier les deux sous-projets (PatternSearchClient et PatternSearchLib), pour que le client (qui include la librairie) puis être compilé correctement
  - L'éxécutable se trouve dans le répertoire "Debug" ou bien "x64/Debug"

Linux :
  - Ouvrez un terminal dans le répertoire racine "PatternSearch"
  - Entrez la commande "make"
  - Entrez dans le répertoire "bin"
  - L'éxecutable est le fichier "client"
