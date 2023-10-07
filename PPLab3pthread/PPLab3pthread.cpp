// Задание pthread.
//
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <pthread.h> 

const int imageSize = 1900; //размер изображения в байтах
const int coefficient = 1;
const int maxThreads = 8;
const int threadStep = imageSize / maxThreads;
unsigned char f[1900];

typedef struct thread_parameters { //параметры функции для потока
    int start;
    int end;
};
void* change_bytes(void* args) {
    thread_parameters* params = (thread_parameters*)args;
    for (int i = params->start; i > params->end; i -= threadStep) {
        f[i] += (i * coefficient) & 255;
    }
    pthread_exit(NULL);
    return 0;
}
HANDLE handles[maxThreads];
DWORD thId[maxThreads];
thread_parameters args[maxThreads];
int main()
{
    FILE* original_file;
    FILE* new_file;
    int tmp;

    original_file = fopen("C://Users//707//Downloads//corrupted_binary.jpg", "rb");
    fread(f, sizeof(f), 1, original_file);

    pthread_t thread[maxThreads];

    for (int i = 1; i < maxThreads + 1; i++) { //идем по потокам

        args[i - 1].start = imageSize - i;
        args[i - 1].end = 0;

        //первый поток расшифровывает от N-1, N-1-N/B, второй N-2, N-2-N/B, N-2-2*N/B...
        //N - размер файла, B = max_threads
         
        tmp = pthread_create(&thread[i - 1], NULL, change_bytes, (void*) &args[i - 1]);

        if (tmp != 0)
        {
            std::cout << "Creating thread " << i << " failed!" << std::endl;
            return 1;
        }
    }
    for (int i = 0; i < maxThreads; i++)
    {
        tmp = pthread_join(thread[i], NULL);
        if (tmp != 0)
        {
            return 1;
        }
    }
    

    new_file = fopen("C://Users//707//Downloads//changed_binary_pthread.jpg", "wb");
    fwrite(f, sizeof(f), 1, new_file);

    fclose(original_file);
    fclose(new_file);
}