g++ -O3 -o multhread multhread.cc -pthread -std=c++11
g++ -O3 -o shm_writer shm_writer.cc -lrt -std=c++11
g++ -O3 -o shm_reader shm_reader.cc -lrt -std=c++11
