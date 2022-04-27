#include "sdsl-source/include/sdsl/suffix_arrays.hpp"
#include "SDSL.h"
// fileSource = fichier a parcourir 
// fileList = fichier liste
// fileOut = nom du fichier en sortie 

using namespace sdsl;
namespace PatternSearch
{
 void SDSL::fm_index(string fileSource, string fileList , string fileOut)
{
    size_t max_locations = 5;
    size_t post_context = 10;
    size_t pre_context = 10;

    
    ofstream fichier(fileOut);


    string index_suffix = ".fm9";
    string index_file = fileSource + index_suffix;
    csa_wt<wt_huff<rrr_vector<127>>, 512, 1024> fm_index; // creation de l'index methode huffman, forme rrr_vector.

    if (!load_from_file(fm_index, index_file))
    {
        ifstream in(fileSource);
        if (!in)
        {
            cout << "ERROR: File " << fileSource << " does not exist. Exit." << endl;
        }
        cout << "No index " << index_file << " located. Building index now." << endl;
        construct(fm_index, fileSource, 1);     // generate index
        store_to_file(fm_index, index_file); // save it
    }
    cout << "Index construction complete, index requires " << size_in_mega_bytes(fm_index) << " MiB." << endl;

    cout << "Input search terms and press Ctrl-D to exit." << endl;
    string prompt = "\e[0;32m>\e[0m ";
    cout << prompt;

    ifstream source(fileList);

    string nom_du_fichier;
    getline(source, nom_du_fichier, ' '); // recup du nom du fichier

    char taille[100]; // recupération de la taille des mots
    source.getline(taille, 1000, ' ');
    int tailles = atoi(taille);


    char inter_nb[100]; // recupération du nombre de mots dans la liste
    source.getline(inter_nb, 1000, ' ');
    int nb = atoi(inter_nb);

    char inter_seuil[100];// récupération score min
    source.getline(inter_seuil, 10000, '\n'); 
    float seuil = atof(inter_seuil);


    for (int num_mot = 0; num_mot < nb; num_mot++)
    {
        string query;
        getline(source, query, '>');
        

        size_t m = query.size();
        size_t occs = sdsl::count(fm_index, query.begin(), query.end());
        if (occs > 0)
        {
            char inter_score[100];
            source.getline(inter_score,1000,'\n');
            float score = atof(inter_score);
            cout << "# of occurrences: " << occs << endl;
            
                cout << "Location and context of first occurrences: " << endl;
                auto locations = locate(fm_index, query.begin(), query.begin() + m);
                sort(locations.begin(), locations.end());
                for (size_t i = 0, pre_extract = pre_context, post_extract = post_context; i < min(occs, max_locations); ++i)
                {
                    cout << setw(8) << locations[i] << ": ";
                    fichier << locations[i]<<"-"<<locations[i] + tailles <<"-";
                    if (pre_extract > locations[i])
                    {
                        pre_extract = locations[i];
                    }
                    if (locations[i] + m + post_extract > fm_index.size())
                    {
                        post_extract = fm_index.size() - locations[i] - m;
                    }
                    auto s = extract(fm_index, locations[i] - pre_extract, locations[i] + m + post_extract - 1);
                    string pre = s.substr(0, pre_extract);
                    s = s.substr(pre_extract);
                    if (pre.find_last_of('\n') != string::npos)
                    {
                        pre = pre.substr(pre.find_last_of('\n') + 1);
                    }
                    cout << pre;
                    cout << "\e[1;31m"; // rouge
                    cout << s.substr(0, m);
                    cout << "\e[0m"; // blanc
                    string context = s.substr(m);
                    cout << context.substr(0, context.find_first_of('\n')) << endl;

                    fichier<<s.substr(0,m) << "-"<< score<<"\n";
                }
            

            cout << endl;
        }
        else{source.ignore(1000, '\n');}

    }

    fichier.close();
}
}
