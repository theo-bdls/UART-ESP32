/* Written by Théo Bruneau de la Salle
*
* ENSEIRB-MATMECA student, France
*
* 2024 
*/


#include "Storage.h"

using namespace std;

// Constructor
storageData::storageData(){
}

// Destructor
storageData::~storageData()
{
}

/////// Function of the class //////
/**
 * Open a text file to store the data
 * 
 * IN : 
 * 
 * fullpath : full address of the file
 * 
 * OUT : 
 * 
 * Text file created on your computer
 */
void storageData::Open_Text_File(const char* fullPath){
    FILE* textFile = std::fopen(fullPath, "w");

    if (!textFile)
    {
        std::cerr << "Error opening file for writing: " << fullPath << std::endl;
        return;
    }

    // Close the file
    std::fclose(textFile);
}

/**
 * Write the data received in the text file
 * 
 * IN :
 * 
 * fullPath : full path for the file: Ex : '/Users/dossier/test/Data.txt'
 * data : received by using socketUDP class
 * 
 * OUT : 
 * Text File on your computer/OS
 */
void storageData::Write_Text_File(const char* fullPath, char* data){
    // Open the file in append mode
    FILE* textFile = std::fopen(fullPath, "a");

    if (!textFile)
    {
        std::cerr << "Error opening file for writing: " << fullPath << std::endl;
    }

    // Write the data to the file
    std::fprintf(textFile, "%s", data);
    CloseFile(textFile);
}

/**
 * Close the text file
 * 
 * IN : 
 * 
 * fid : file descriptor 
 */
void storageData::CloseFile(FILE* fid){
    std::fclose(fid);
}

/////// Setters ///////
/**
 * Set the private attribute fullpath
 * 
 * IN : 
 * 
 * fullPath : Full path of your file 
 */
void storageData::setFileParameters(const char* fullPath)
{
    m_fullPath = fullPath;
}

/////// Getters ///////
/**
* Get the fullpath of the file
*
* OUT :
* Full Path of the file
*/
const char* storageData::getFullPath() const
{
    return m_fullPath;
}