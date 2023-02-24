#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <cmath>

class FileScrubber {
protected:
    std::string Filename;
    std::vector<std::string> Filenames;
    double FileSize;
    std::string FileMemoryLocation;

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

    /* 
    Maybe make a function which returns size to user?
     if (filesize < 1024) {
                    //std::cout << "The size of: [" << filenames[i] << "] is: " << filesize << " kB\n";
                    return filesize;
                }
                else if (filesize >= 1024) {
                    double filesizeKB = (filesize / 1024);
                    //std::cout << "The size of: [" << filenames[i] << "] is: " << filesizeKB  << " kB\n";
                    return filesizeKB;
                }
                else if (filesize >= 1048576) {
                    double filesizeMB = (filesize / (1024^2) );
                    //std::cout << "The size of: [" << filenames[i] << "] is: " << filesizeMB  << " MB\n";
                    return filesizeMB;
                }
                else if (filesize >= 1073741824) {
                    double filesizeGB = (filesize / (1024 ^ 3));
                    //std::cout << "The size of: [" << filenames[i] << "] is: " << filesizeGB  << " GB\n";
                    return filesizeGB;
                }

    */
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
                    outToFile.write(reinterpret_cast<char*>(&BinaryZero), filesizes[i]); // This is broken. does not write the same amount of bytes!!
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

    filescrub.RewriteFileSectors(filenames, 2);
}

// to do
// 1. Add another class which handles the Filescrubber - It should ask the following
    // a. Filenames
    // b. How many repetitions
    // c. 1's or 0's or both
    // d. Whether to delete the file afterwards. (some people may want to review the file before manually deleting)