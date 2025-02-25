/* Written by Théo Bruneau de la Salle
*
* ENSEIRB-MATMECA student, France
*
* 2024 
*/

#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>


class storageData
/////// Class to store the data in a text file //////
{
public:
    ///// Constructor/Destructor /////
    storageData();  // Constructor
    ~storageData(); // Destructor

    ///// Setters /////
    void setFileParameters(const char* fullPath);

    ///// Getters /////
    const char* getFullPath() const;

    /////// Function declaration //////

    void Open_Text_File(const char* fullPath);

    void Write_Text_File(const char* fullPath, char* data);

    void CloseFile(FILE* fid);

private:
    //// Attributs ////
    const char* m_fullPath;    // <! Full path of the file 
};