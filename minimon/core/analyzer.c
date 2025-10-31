#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// Süreç bilgilerini tutmak için yapı
typedef struct {
    int pid;
    char name[256];
    float cpu;  
} ProcessInfo;

#define MAX_PROCESSES 1024

// Bir dizenin sadece sayılardan oluşup oluşmadığını kontrol eder
int is_number(const char* s) {
    while (*s) {
        if (!isdigit(*s)) return 0;
        s++;
    }
    return 1;
}

// En çok CPU kullanan süreçleri  sıralı biçimde listeler
void list_top_processes_from_proc(int count) {
    DIR* dir = opendir("/proc");  // /proc dizinini aç
    if (!dir) {
        perror("proc açılamadı");
        return;
    }

    struct dirent* entry;
    ProcessInfo processes[MAX_PROCESSES];
    int idx = 0;

    // /proc içindeki tüm PID klasörlerini dolaş
    while ((entry = readdir(dir)) != NULL && idx < MAX_PROCESSES) {
        if (!is_number(entry->d_name)) continue;  // sadece PID olan klasörleri al

        // /proc/[pid]/stat yolunu oluştur
        char stat_path[300];
        snprintf(stat_path, sizeof(stat_path), "/proc/%s/stat", entry->d_name);

        FILE* stat_fp = fopen(stat_path, "r");
        if (!stat_fp) continue;

        // PID, komut adı ve process state bilgilerini oku
        int pid;
        char comm[300];
        char state;
        unsigned long utime, stime;

        fscanf(stat_fp, "%d %s %c", &pid, comm, &state);
        for (int i = 0; i < 10; i++) fscanf(stat_fp, "%*s"); // gereksiz alanları atla
        fscanf(stat_fp, "%lu %lu", &utime, &stime);          // kullanıcı ve sistem süresi
        fclose(stat_fp);

        // CPU kullanımı yaklaşık olarak hesaplanıyor (gerçek oran değil)
        float cpu_fake = (utime + stime) / 1000.0;

        // süreç bilgilerini kaydet
        processes[idx].pid = pid;
        strncpy(processes[idx].name, comm, 255);
        processes[idx].cpu = cpu_fake;
        idx++;
    }

    closedir(dir);

    // Bubble sort ile CPU değerine göre azalan sıralama
    for (int i = 0; i < idx - 1; i++) {
        for (int j = 0; j < idx - i - 1; j++) {
            if (processes[j].cpu < processes[j + 1].cpu) {
                ProcessInfo tmp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = tmp;
            }
        }
    }

    // İlk N süreci ekrana yazdır
    printf("En çok CPU kullanan %d süreç:\n", count);
    for (int i = 0; i < count && i < idx; i++) {
        printf("%d. %s (PID: %d) - %%%.1f\n", i + 1, processes[i].name, processes[i].pid, processes[i].cpu);
    }
}

