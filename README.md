# Burrows-Wheeler-transform-HPC
Parallel Implementation of the Burrows-Wheeler Transform for Efficient Compression of Large Text Files.

# Contact Information
jalg030129@gmail.com 

# Affiliation
This is a project of the High Performace Computing (HPC) course at Escuela Nacional de Estudios Superiores Unidad Morela (ENES - UNAM).This project is carried out with the intention of accomplish the parallelization of the BWT algorithm as part of the requirements of the course in the Bachelor's Degree for Technologies for Information in Sciences, but also to provide a functional tool for compression of large text files as quick as posible.

# Introduction
Compressing large text files is a challenge in applications such as data storage (DB search engines) and information transmission (such as message apps). The BWT is a key algorithm in compressors like bzip2, but its sequential implementation can be slow for large files. And the compression of algorithms like Run-Lenght Encoding is harder to rebuild to get the original files.

# Justification 
Parallelizing the BWT can significantly improve performance, making use of MPI and Open MP for matrices. That enables its use in applications that handle large volumes of data, providing a faster and safer tool to compress data.

# Methodology
        BWT Algorithm Analysis: Study its operation and bottlenecks.

        Parallelization: Use OpenMP to parallelize the generation and sorting of rotations.

        Integration with Compression Techniques: Apply Move-to-Front and Huffman Encoding.

        Testing and Evaluation: Compare performance with existing implementations."

Tools and Technologies:

    Language: C.

    Libraries: OpenMP for parallelization.

    Environment: Linux.
