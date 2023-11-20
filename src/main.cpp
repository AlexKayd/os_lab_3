#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>

int main(){
// Чтение, запись  имени тестового файла 
    std::cout << "Enter the file name: " << "\n";
    std::string file_name;
    std::cin >> file_name; 

// Открытие файла
    int fd = open(file_name.c_str(), O_RDONLY);
    if (fd == -1){
        perror("Open file error. \n");
        exit(-1);
    }

// Получение информации о файле (его размер)
    struct stat file_info;
    if (fstat(fd, &file_info) == -1){
        close(fd);
        perror("Get file information error. \n");
        exit(-1);
    }

// Отображение файла в память
    char* file_memory = (char*) mmap(NULL, file_info.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (file_memory == MAP_FAILED){
        close(fd);
        perror("Memory file error. \n");
        exit(-1);
    }

// Создание дочернего потока
    pid_t pid = fork();

    if (pid == -1){
        perror("Pid errror. \n");
        exit(-1);
    }

// Работа с дочерним потоком
    if (pid == 0){ 
        const char* child = "child"; 
        std::string size = std::to_string(file_info.st_size);

        execl(child, file_memory, size.c_str(), NULL);
        exit(EXIT_SUCCESS);
    }
    
// Работа с родительским потоком 
    if (pid > 0){
        int end;
        waitpid(pid, &end, 0);

        if (WIFEXITED(end)){
            int exit = WEXITSTATUS(end);
            std::cout << "Child process: " << exit << "\n";
        } 
        if (WIFSIGNALED(end)){
            int signal = WTERMSIG(end);
            std::cout << "Child process ended with a signal: " << signal << "\n";
        }
    }

// Освобождение памяти, закрытие файла
    if (munmap(file_memory, file_info.st_size) == -1){
        perror("Pid errror. \n");
        exit(-1);
    }

    close(fd);
    
    return 0;
}