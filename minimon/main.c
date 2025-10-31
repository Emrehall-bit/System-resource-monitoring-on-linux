#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// main.c - Kullanıcıdan komut satırı argümanlarını alarak ilgili işlemleri çalıştıran uygulama giriş noktasıdır.



// diğer dosyada tanımlanan Fonksiyonların prototipleri
void show_cpu_info();
void show_mem_info();
void show_top_processes(int count);
void show_disk_info();
void show_network_info();
void write_resource_report();
void run_cli();

//Programın ana fonksiyonu
int main(int argc, char *argv[])
{
    // Argument yoksa kullanım bilgisi ver
    if (argc < 2)
    {
        printf("Kullanım: %s <komut>\n", argv[0]);
        printf("Komutlar: cpu | mem | top <n> | disk | net | report | cli\n");
        return 1;
    }

    // Komut karşılaştırmaları
    if (strcmp(argv[1], "cpu") == 0)
    {
        show_cpu_info();  // CPU bilgisi göster
    }
    else if (strcmp(argv[1], "mem") == 0)
    {
        show_mem_info();  // RAM bilgisi göster
    }
    else if (strcmp(argv[1], "top") == 0)
    {
        if (argc < 3)
        {
            printf("top komutu için bir sayı belirtmelisiniz. Örn: %s top 5\n", argv[0]);
            return 1;
        }

        int count = atoi(argv[2]);
        show_top_processes(count);  // En çok CPU kullanan süreçleri gösterir
    }
    else if (strcmp(argv[1], "disk") == 0)
    {
        show_disk_info();  // Disk bilgisi
    }
    else if (strcmp(argv[1], "net") == 0) 
    {
        show_network_info();  // Ağ bilgisi
    }
    else if (strcmp(argv[1], "report") == 0) 
    {
        write_resource_report();  // CSV raporu oluşturur
    }
    else if (strcmp(argv[1], "cli") == 0) 
    {
        run_cli();  // CLI menü arayüzünü başlatır
    }
    else
    {
        printf("Bilinmeyen komut: %s\n", argv[1]);  // Geçersiz komut uyarısı verir
    }

    return 0;
}


