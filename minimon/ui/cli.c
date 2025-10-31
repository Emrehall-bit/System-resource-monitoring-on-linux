#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Ödevde belirtilen CLI,Kullanıcıdan giriş alarak ilgili işlemleri menü şeklinde çalıştıran CLI arayüzünü içerir

//DERLEME KOMUTU: gcc main.c core/collector.c core/analyzer.c core/reporter.c ui/cli.c utils/config.c -o minimon

//ÇALIŞTIRMA KOMUTU: ./minimon cli


void show_cpu_info();
void show_mem_info();
void show_top_processes(int count);
void show_disk_info();
void show_network_info();
void write_resource_report();
void print_banner();  


void run_cli() {
    int secim;
    int sayi;
    print_banner();

    while (1) {
        printf("\n=== MiniMon Menü ===\n");
        printf("1. CPU Bilgisi Göster\n");
        printf("2. RAM Bilgisi Göster\n");
        printf("3. En Çok CPU Kullanan Süreçler\n");
        printf("4. Disk Kullanımı Göster\n");
        printf("5. Ağ Bilgisi Göster\n");
        printf("6. Sistem Raporu Oluştur\n");
        printf("0. Çıkış\n");
        printf("Seçiminiz: ");
        scanf("%d", &secim);

        switch (secim) {
            case 1:
                show_cpu_info();
                break;
            case 2:
                show_mem_info();
                break;
            case 3:
                printf("Kaç süreç listelensin? ");
                scanf("%d", &sayi);
                show_top_processes(sayi);
                break;
            case 4:
                show_disk_info();
                break;
            case 5:
                show_network_info();
                break;
            case 6:
                write_resource_report();
                break;
            case 0:
                printf("Çıkılıyor...\n");
                return;
            default:
                printf("Geçersiz seçim!\n");
        }
    }
}

