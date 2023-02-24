#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

class FileScrubber {
protected:
    std::string Filename;
    std::vector<std::string> Filenames;
    double FileSize;
    std::string FileMemoryLocation;
    int Repetitions;
public:
    void setFilename() {
        std::cout << "Please enter a filepath" << std::endl;
        std::cin >> Filename;
    }

    std::string getFilename() {
        return Filename;
    }

    void setFilenames() {
        std::string filenames;
        int counter = 0;
        while (true) {
            std::cout << "Please enter filepath number [" << counter << "]" << std::endl;
            std::cin >> filenames;
            counter++;
            if (filenames == "q") {
                for (int i = 0; i < Filenames.size(); i++) {
                    std::cout << Filenames[i] << std::endl;
                }
                break;
            }
            Filenames.push_back(filenames);
        }
        counter = 0;
    }

    std::vector<std::string>& getFilenames() {
        return Filenames;
    }

    std::vector<int> getFileSize(std::vector<std::string> filenames) {
        std::vector<int> sizes;
        for (int i = 0; i < filenames.size(); i++)
        {
            try {
                int filesize = std::filesystem::file_size(filenames[i]);
                sizes.push_back(filesize);

            }
            catch (const std::filesystem::filesystem_error& ex) {
                std::cerr << "Error: " << ex.what() << std::endl;
                break;
            }
        }
        return sizes;
    }

    int setRepetitions() {
        std::cout << "Please enter the amount of times you wish to overwrite the file" << std::endl;
        std::cin >> Repetitions;
    }

    int getRepetitions() {
        return Repetitions;
    }

    int RewriteFileSectors(std::vector<std::string> filenames, int repetitions) {
        std::vector<int> filesizes = getFileSize(filenames);
        for (int i = 0; i < filenames.size(); i++)
        {
            std::cout << "opening file: " << filenames[i] << std::endl;
            std::ofstream outToFile(filenames[i], std::ios::binary);
            unsigned char BinaryZero = 0b00000000;
            if (!outToFile) {
                // error handling to log file would be best
                std::cout << filenames[i] << " could not be opened" << std::endl;
            }
            else {
                for (int j = 0; j < repetitions; j++) {
                    outToFile.write(reinterpret_cast<char*>(&BinaryZero), filesizes[i]);
                    std::cout << "The file " << filenames[i] << " has been destroyed" << std::endl;
                }
                outToFile.close();
                std::cout << "closing file: " << filenames[i] << std::endl;
            }

        }
        return 0;
    }


};

int main()
{
    FileScrubber filescrub;
    filescrub.setFilenames();

    std::vector<std::string>    filenames = filescrub.getFilenames();
    std::vector<int>            filesizes = filescrub.getFileSize(filenames);
    int                         repetitions = filescrub.setRepetitions();

    filescrub.RewriteFileSectors(filenames, repetitions);
}