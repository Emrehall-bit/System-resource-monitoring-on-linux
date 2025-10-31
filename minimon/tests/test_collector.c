#include <stdio.h>

// Ana minimon dosyasını komple çalıştırmadan fonksiyonları test etmek için test dosyası
void show_cpu_info();
void show_mem_info();
void show_top_processes(int count);
void show_disk_info();
void show_network_info();

int main() {
    printf("=== MiniMon Test Başlatıldı ===\n\n");

    printf("[TEST] CPU Bilgisi:\n");
    show_cpu_info();

    printf("\n[TEST] RAM Bilgisi:\n");
    show_mem_info();

    printf("\n[TEST] En çok CPU kullanan süreçler:\n");
    show_top_processes(3);

    printf("\n[TEST] Disk Bilgisi:\n");
    show_disk_info();

    printf("\n[TEST] Ağ Bilgisi:\n");
    show_network_info();

    printf("\n=== MiniMon Test Bitti ===\n");
    return 0;
}

