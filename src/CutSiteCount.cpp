#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include "CutSiteCount.h"
#include "RcoutRcerr.h"
using namespace std;


CutSiteCount::CutSiteCount(string readsfile, string motiffile, string matrixfile, int motif_len, int strand_len)
{
    this -> readsfile = readsfile;
    this -> motiffile = motiffile;
    this -> matrixfile = matrixfile;
    this -> motif_len = motif_len;
    this -> strand_len = strand_len;
}

int CutSiteCount::DoCutSiteCount()
{
    // These two files must be sorted
    string readsifile = this -> readsfile;
    string motififile = this -> motiffile;
    int motif_len = this -> motif_len;
    int strand_len = this -> strand_len;
    // Output file
    string matrixfile = this -> matrixfile;

    // matrix using to save cut site infomation
    int matrix_len = motif_len + strand_len * 2;
    vector<int> matrix (matrix_len, 0);


    ifstream readsfile(readsifile.c_str(), ios::in);
    ifstream motiffile(motififile.c_str(), ios::in);

    ofstream matrixOutput(matrixfile.c_str(), ios::out);

    char lineR[100000] = {0};
    char lineM[100000] = {0};
    const char *sep = "\t ";

    // read motif file, no value --> exit
    if(!motiffile.getline(lineM, sizeof(lineM)))
    {
        return 0;
    }
    // cout << lineM << endl;
    string M_chr(strtok(lineM, sep));
    int M_start = atoi(strtok(NULL, sep));
    int M_end = atoi(strtok(NULL, sep));
    string M_strand(strtok(NULL, sep));
    int M_s = M_start - strand_len;
    int M_e = M_end + strand_len;

    // read reads file, no value --> exit
    if(!readsfile.getline(lineR, sizeof(lineR)))
    {
        return 0;

    }
    int R_start = atoi(strtok(lineR, sep));

    while(1)
    {
        if (R_start < M_s) // in this situation, R file read the next line
        {
            //cout << "step1" << endl;
            // read reads file, no value --> exit
            if(readsfile.getline(lineR, sizeof(lineR)))
            {
                R_start = atoi(strtok(lineR, sep));
            }
            else // read the end, read file finished
            {
                // reads file end, write matrix
                if(M_strand == "+") // motif in 5'-->3'
                {
                    for(int i = 0; i < matrix_len; i++) // write matrix to output file
                    {
                        matrixOutput << matrix[i] << "\t";
                    }
                    matrixOutput << "\n";
                    fill(matrix.begin(), matrix.end(), 0); // set matrix to 0
                }
                else  // motif in 3'-->5'
                {
                    reverse(matrix.begin(), matrix.end());
                    for(int i = 0; i < matrix_len; i++) // write matrix to output file
                    {
                        matrixOutput << matrix[i] << "\t";
                    }
                    matrixOutput << "\n";
                    fill(matrix.begin(), matrix.end(), 0); // set matrix to 0
                }

                // for the rest motif, because of reads file end, write 0 matrix
                while(motiffile.getline(lineM, sizeof(lineM)))
                {
                    if(M_strand == "+") // motif in 5'-->3'
                    {
                        for(int i = 0; i < matrix_len; i++) // write matrix to output file
                        {
                            matrixOutput << matrix[i] << "\t";
                        }
                        matrixOutput << "\n";
                        fill(matrix.begin(), matrix.end(), 0); // set matrix to 0
                    }
                    else  // motif in 3'-->5'
                    {
                        reverse(matrix.begin(), matrix.end());
                        for(int i = 0; i < matrix_len; i++) // write matrix to output file
                        {
                            matrixOutput << matrix[i] << "\t";
                        }
                        matrixOutput << "\n";
                        fill(matrix.begin(), matrix.end(), 0); // set matrix to 0
                    }
                }
                // cout << "loop1 end" << endl;
                // cout << R_start << endl;
                // cout << M_s << endl;
                // two file end
                readsfile.close();
                motiffile.close();
                matrixOutput.close();
                return 0;
            }
        }
        else if((R_start >= M_s) && (R_start <= M_e)) // modify matrix and read next reads
        {
            //cout << "step2" << endl;
            matrix[R_start - M_s] += 1;// modify matrix value
            if(readsfile.getline(lineR, sizeof(lineR)))
            {
                R_start = atoi(strtok(lineR, sep));
            }
            else
            {
                // reads file end, write matrix
                if(M_strand == "+") // motif in 5'-->3'
                {
                    for(int i = 0; i < matrix_len; i++) // write matrix to output file
                    {
                        matrixOutput << matrix[i] << "\t";
                    }
                    matrixOutput << "\n";
                    fill(matrix.begin(), matrix.end(), 0); // set matrix to 0
                }
                else  // motif in 3'-->5'
                {
                    reverse(matrix.begin(), matrix.end());
                    for(int i = 0; i < matrix_len; i++) // write matrix to output file
                    {
                        matrixOutput << matrix[i] << "\t";
                    }
                    matrixOutput << "\n";
                    fill(matrix.begin(), matrix.end(), 0); // set matrix to 0
                }

                // for the rest motif, because of reads file end, write 0 matrix
                while(motiffile.getline(lineM, sizeof(lineM)))
                {
                    if(M_strand == "+") // motif in 5'-->3'
                    {
                        for(int i = 0; i < matrix_len; i++) // write matrix to output file
                        {
                            matrixOutput << matrix[i] << "\t";
                        }
                        matrixOutput << "\n";
                        fill(matrix.begin(), matrix.end(), 0); // set matrix to 0
                    }
                    else  // motif in 3'-->5'
                    {
                        reverse(matrix.begin(), matrix.end());
                        for(int i = 0; i < matrix_len; i++) // write matrix to output file
                        {
                            matrixOutput << matrix[i] << "\t";
                        }
                        matrixOutput << "\n";
                        fill(matrix.begin(), matrix.end(), 0); // set matrix to 0
                    }
                }
                // two file end
                // cout << "loop2 end" << endl;
                // cout << R_start << endl;
                // cout << M_s << endl;
                readsfile.close();
                motiffile.close();
                matrixOutput.close();
                return 0;
            }
        }
        else if (R_start > M_e) // a motif site end, write and read next motif, then recompare
        {
            //cout << "step3" << endl;
            if(M_strand == "+") // motif in 5'-->3'
            {
                for(int i = 0; i < matrix_len; i++) // write matrix to output file
                {
                    matrixOutput << matrix[i] << "\t";
                }
                matrixOutput << "\n";
                fill(matrix.begin(), matrix.end(), 0); // set matrix to 0
            }
            else  // motif in 3'-->5'
            {
                reverse(matrix.begin(), matrix.end());
                for(int i = 0; i < matrix_len; i++) // write matrix to output file
                {
                    matrixOutput << matrix[i] << "\t";
                }
                matrixOutput << "\n";
                fill(matrix.begin(), matrix.end(), 0); // set matrix to 0
            }
            // read the next motif
            if(motiffile.getline(lineM, sizeof(lineM)))
            {
                // cout << lineM << endl;
                M_chr = strtok(lineM, sep);
                M_start = atoi(strtok(NULL, sep));
                M_end = atoi(strtok(NULL, sep));
                M_strand = strtok(NULL, sep);
                M_s = M_start - strand_len;
                M_e = M_end + strand_len;
                if(readsfile.getline(lineR, sizeof(lineR)))
                {
                    R_start = atoi(strtok(lineR, sep));
                }
            }
            else // motif file end, and matrix has been written
            {
                readsfile.close();
                motiffile.close();
                matrixOutput.close();
                // cout << "loop3 end" << endl;
                // cout << R_start << endl;
                // cout << M_s << endl;
                return 0;
            }
        }
    }

    // cout << "exit!!!!!!!!!!!!!!" << endl;
    readsfile.close();
    motiffile.close();
    matrixOutput.close();
    return 0;
}





