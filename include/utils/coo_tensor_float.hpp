#pragma once
/**
 * @file        coo_tensor.hpp
 * 
 * @brief       Functions for saving an array in COO format.
 * 
 * @author      Filippo Maggioli\n
 *              (maggioli@di.uniroma1.it, maggioli.filippo@gmail.com)\n
 *              Sapienza, University of Rome - Department of Computer Science
 * 
 * @date        2023-08-21
 */




#include <cmath>
#include <fstream>
#include <filesystem>


// namespace fx3d
// {

inline bool compress_array(const float* buffer, 
                    int buflen,
                    int* idxs,
                    float* vals,
                    int& nnz,
                    float zero_thresh = 1e-6f)
{
    if (idxs == nullptr || vals == nullptr || buffer == nullptr)
        return false;
    if (buflen < 0)
        return false;

    nnz = 0;
    for (int i = 0; i < buflen; ++i)
    {
        if (std::abs(buffer[i]) <= zero_thresh)
            continue;

        idxs[nnz] = i;
        vals[nnz] = buffer[i];
        nnz += 1;
    }

    return true;
}

inline bool compress_array(const float* buffer,
                    int buf_x, int buf_y,
                    int* idxs,
                    float* vals,
                    int& nnz,
                    float zero_thresh = 1e-6f)
{
    return compress_array(buffer, buf_x * buf_y, idxs, vals, nnz, zero_thresh);
}

inline bool compress_array(const float* buffer,
                    int buf_x, int buf_y, int buf_z,
                    int* idxs,
                    float* vals,
                    int& nnz,
                    float zero_thresh = 1e-6f)
{
    return compress_array(buffer, buf_x * buf_y * buf_z, idxs, vals, nnz, zero_thresh);
}

inline bool decompress_array(float* buffer,
                      int buflen,
                      const int* idxs,
                      const float* vals,
                      int nnz)
{
    if (idxs == nullptr || vals == nullptr || buffer == nullptr)
        return false;
    if (buflen < 0)
        return false;
    if (nnz < 0)
        return false;

    buffer = (float*)std::memset(buffer, 0, buflen * sizeof(float));
    if (buffer == nullptr)
        return false;

    for (int i = 0; i < nnz; ++i)
        buffer[idxs[i]] = vals[i];

    return true;
}


inline float* float_alloc(size_t numel)
{
    return (float*)malloc(numel * sizeof(float));
}

inline float* float_alloc(size_t num_x, size_t num_y)
{
    return float_alloc(num_x * num_y);
}

inline float* float_alloc(size_t num_x, size_t num_y, size_t num_z)
{
    return float_alloc(num_x * num_y * num_z);
}


inline bool write_coo_tensor(std::ofstream& outstream,
                      const int nx, const int ny, const int nz,
                      const int* idxs,
                      const float* vals,
                      int nnz)
{
    if (!outstream.is_open())
        return false;

    outstream.write((const char*)&nx, sizeof(int));
    outstream.write((const char*)&ny, sizeof(int));
    outstream.write((const char*)&nz, sizeof(int));
    outstream.write((const char*)&nnz, sizeof(int));
    outstream.write((const char*)idxs, nnz * sizeof(int));
    outstream.write((const char*)vals, nnz * sizeof(float));

    return true;
}

inline bool write_coo_tensor(const std::string& filename,
                      const int nx, const int ny, const int nz,
                      const int* idxs,
                      const float* vals,
                      int nnz)
{
    std::ofstream outstream;
    outstream.open(filename, std::ios::binary);
    bool res = write_coo_tensor(outstream, nx, ny, nz, idxs, vals, nnz);
    outstream.close();
    return res;
}

inline bool write_coo_tensor(const std::string& dirname,
                      const std::string& filename,
                      const int nx, const int ny, const int nz,
                      const int* idxs,
                      const float* vals,
                      int nnz)
{
    std::filesystem::path dpath(dirname);
    std::filesystem::path fpath(filename);
    return write_coo_tensor((dpath / fpath).string(), nx, ny, nz, idxs, vals, nnz);
}

inline bool read_coo_tensor(std::ifstream& instream,
                     int** idxs,
                     float** vals,
                     int& nnz)
{
    if (!instream.is_open())
        return false;

    int nxnynz[3];
    instream.read((char*)nxnynz, 3 * sizeof(int));
    instream.read((char*)&nnz, sizeof(int));

    if (*vals == nullptr)
        *vals = float_alloc(nnz);
    if (*idxs == nullptr)
        *idxs = (int*)std::malloc(nnz * sizeof(int));
    if (*vals == nullptr || *idxs == nullptr)
        return false;
    
    instream.read((char*)*idxs, nnz * sizeof(int));
    instream.read((char*)*vals, nnz * sizeof(float));

    return true;
}

inline bool read_coo_tensor(const std::string& filename,
                     int** idxs,
                     float** vals,
                     int& nnz)
{
    std::ifstream instream;
    instream.open(filename, std::ios::binary);
    bool res = read_coo_tensor(instream, idxs, vals, nnz);
    instream.close();
    return res;
}

inline bool read_coo_tensor(const std::string& dirname,
                     const std::string& filename,
                     int** idxs,
                     float** vals,
                     int& nnz)
{
    std::filesystem::path dpath(dirname);
    std::filesystem::path fpath(filename);
    return read_coo_tensor((dpath / fpath).string(), idxs, vals, nnz);
}

    
// } // namespace fx3d
