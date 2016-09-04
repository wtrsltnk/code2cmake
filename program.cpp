#include <iostream>
#include <string>
#include <sstream>
#include "tokenizer.h"
#include "tinydir.h"

#include <system.io.fileinfo.h>
#include <system.io.path.h>

using namespace std;
using namespace System::IO;

void listCodeFiles(const string& root, const string& relativePath, vector<string>& files, vector<string>& directories)
{
    string path(Path::Combine(root, relativePath));

    tinydir_dir dir;
    tinydir_open(&dir, path.c_str());

    while (dir.has_next)
    {
        tinydir_file file;
        tinydir_readfile(&dir, &file);

        if (file.is_dir)
        {
            if (file.name[0] != '.')
            {
                directories.push_back(Path::Combine(path, file.name));
                listCodeFiles(root, Path::Combine(relativePath, file.name), files, directories);
            }
        }
        else
        {
            files.push_back(Path::Combine(path, file.name));
        }

        tinydir_next(&dir);
    }

    tinydir_close(&dir);
}

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        cout << "No parameters to work with..." << endl;
        return 0;
    }

    vector<string> files;
    vector<string> directories;
    listCodeFiles(argv[1], "", files, directories);

    vector<string> headerfiles, codefiles;
    for (auto itr = files.cbegin(); itr != files.cend(); ++itr)
    {
        auto filename = *itr;
        cout << filename << endl;
    }

    for (auto itr = directories.cbegin(); itr != directories.cend(); ++itr)
    {
        cout << *itr << endl;
    }

    return 0;
}
