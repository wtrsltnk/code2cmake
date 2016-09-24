#include <iostream>
#include <string>
#include <sstream>
#include "tokenizer.h"

#include <system.io.fileinfo.h>
#include <system.io.directoryinfo.h>
#include <system.io.path.h>

using namespace std;
using namespace System::IO;

void listCodeFiles(const DirectoryInfo& root, const string& relativePath, vector<FileInfo>& files, vector<DirectoryInfo>& directories)
{
    auto current = DirectoryInfo(Path::Combine(root.FullName(), relativePath));

    auto f = current.GetFiles();
    for (auto itr = f.begin(); itr != f.end(); ++itr)
    {
        auto file = FileInfo(*itr);

        if (file.Name()[0] == '.') continue;

        if (file.Extension() == ".cpp"
                || file.Extension() == ".c"
                || file.Extension() == ".hpp"
                || file.Extension() == ".h")
        {
            files.push_back(file);
        }
    }
    auto d = current.GetDirectories();
    for (auto itr = d.begin(); itr != d.end(); ++itr)
    {
        auto directory = DirectoryInfo(*itr);

        if (directory.Name()[0] == '.') continue;

        directories.push_back(directory);
        listCodeFiles(root, Path::Combine(relativePath, directory.Name()), files, directories);
    }
}

// test comment

/* Multi line comment
 * tweede line
 */
int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        cout << "No parameters to work with..." << endl;
        return 0;
    }

    auto root = DirectoryInfo(argv[1]);
    vector<FileInfo> files;
    vector<DirectoryInfo> directories;
    listCodeFiles(root, "", files, directories);

    vector<string> headerfiles, codefiles;
    for (auto itr = files.cbegin(); itr != files.cend(); ++itr)
    {
        auto filename = *itr;
        cout << filename.FullName() << endl;
    }

    for (auto itr = directories.cbegin(); itr != directories.cend(); ++itr)
    {
        auto directory = *itr;
        cout << directory.FullName() << endl;
    }

    Tokenizer t("..\\code2cmake\\program.cpp");
    auto tokens = t.AllTokens();
    for (auto itr = tokens.begin(); itr != tokens.end(); ++itr)
    {
        auto token = *itr;
        cout << "[" << token.type << "] " << token.token << endl;
    }

    return 0;
}
