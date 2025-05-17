# Burrows-Wheeler-transform- Run Lenght Encoding- HPC
Parallel Implementation of the Burrows-Wheeler Transform for the Compression of Text Files via OpenMP.

# Contact Information
jalg030129@gmail.com 

# Affiliation
This is a project of the High Performace Computing (HPC) course at Escuela Nacional de Estudios Superiores Unidad Morela (ENES - UNAM).This project is carried out with the intention of accomplish the parallelization of the BWT algorithm as part of the requirements of the course in the Bachelor's Degree for Technologies for Information in Sciences, but also to provide a functional tool for compression of large text files as quick as posible.

# Introduction
Compressing large text files is a challenge in today's standards, it happens all the time, sometimes in real time like in some database software around the globe, so the knowledge of the basics of these algorithms is critical. On this case, the implementation of the Burrows Wheeler Transform using OpenMP allows to use the resources in our machine to complete the task. Later to compress it with Run Lenght encoding (RLE) for practical purposes and again decode it so we can check that there's no data loss.

# Justification 
Parallelizing the BWT can significantly improve performance, the algorithms that compress the data, such as RLE, Lempel-Ziv-Welch (LZW), Huffman coding,etc. They all share things in common wich is the oranization of the informatino to then start compressing it in some order. By using tools to use more cores to this task enables its use in applications that handle large volumes of data, to organize it, and lend a hand to the chain of processes involved

# General Objective
Parallelize the BWT with Apply Move-to-Front and Huffman Encoding to compress text

#Particular Objectives:

* Compress a .txt file and verify that the size with compression is smaller than the original.
* Once compressed, decompress the file and see that it hasn't lost data.


# Methodology
        BWT Algorithm Analysis: Study its operation and bottlenecks.

        Parallelization: Use OpenMP to parallelize the generation and sorting of rotations.

        Integration with Compression Techniques: Apply Move-to-Front and Huffman Encoding.

        Testing and Evaluation: Compare performance with existing implementations."

# Tools and Technologies:

    Language: C.

    Libraries: OpenMP for parallelization.

    Environment: Linux.
# How To execute it without installation:
         you should start by cloning this repository: 
         
