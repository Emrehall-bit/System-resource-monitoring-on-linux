#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// collector.c dosyası Sistem resource utilization bilgilerini toplayan fonksiyonları içeriyor.




// analyzer.c'den geldi
void list_top_processes_from_proc(int count);

// /proc/stat üzerinden CPU kullanım oranlarını gösterir
void show_cpu_info() {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) {
        perror("Dosya açılamadı");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), fp); // ilk satırı oku

    unsigned long user, nice, system, idle;
    sscanf(line, "cpu %lu %lu %lu %lu", &user, &nice, &system, &idle);

    // Toplam zaman ve oran hesaplama
    unsigned long total = user + nice + system + idle;
    float usage = 100.0 * (user + system) / total;
    float idle_p = 100.0 * idle / total;

    printf("CPU Kullanımı: %.1f%% (Kullanıcı: %.1f%%, Sistem: %.1f%%, Boşta: %.1f%%)\n",
           usage, 100.0 * user / total, 100.0 * system / total, idle_p);

    fclose(fp);
}

// /proc/meminfo üzerinden RAM ve swap bilgilerini gösterir
void show_mem_info() {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        perror("Dosya açılamadı");
        return;
    }

    char line[256];
    unsigned long mem_total = 0, mem_free = 0;
    unsigned long swap_total = 0, swap_free = 0;

    // İlgili alanları sırayla bul ve değerleri al
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "MemTotal: %lu", &mem_total) == 1) continue;
        if (sscanf(line, "MemFree: %lu", &mem_free) == 1) continue;
        if (sscanf(line, "SwapTotal: %lu", &swap_total) == 1) continue;
        if (sscanf(line, "SwapFree: %lu", &swap_free) == 1) continue;
    }

    fclose(fp);

    // Değerleri MB cinsinden yazdır
    printf("Toplam RAM: %.1f MB\n", mem_total / 1024.0);
    printf("Boş RAM: %.1f MB\n", mem_free / 1024.0);
    printf("Swap: %.1f MB / %.1f MB\n", (swap_total - swap_free) / 1024.0, swap_total / 1024.0);
}

// En çok CPU kullanan süreçleri gösterir (analyzer.c içinde detaylı analiz mevcut)
void show_top_processes(int count) {
    list_top_processes_from_proc(count);  // analyzer.c fonksiyonunu çağır
}

// Disk boyutu, kullanım oranı ve I/O istatistiklerini gösterir
void show_disk_info() {
    printf("Disk Kullanımı:\n");

    // df komutu ile bölümleri ve kullanım oranlarını göster
    FILE *fp = popen("df -h --output=source,size,used,avail,pcent,target | grep '^/dev/'", "r");
    if (fp == NULL) {
        perror("df komutu çalıştırılamadı");
        return;
    }

    char line[300];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    pclose(fp);

    // /proc/diskstats üzerinden ilk fiziksel diskin I/O bilgilerini al
    printf("\nDisk I/O İstatistikleri (ilk fiziksel disk):\n");

    FILE *fp2 = fopen("/proc/diskstats", "r");
    if (fp2 == NULL) {
        perror("/proc/diskstats açılamadı");
        return;
    }

    char dev[32];
    unsigned long read_ops, write_ops;

    // İlk fiziksel diski bulana kadar satırları tara
    while (fgets(line, sizeof(line), fp2)) {
        if (sscanf(line, "%*d %*d %s %lu %*u %*u %*u %lu", dev, &read_ops, &write_ops) == 3) {
            // Sanal aygıtları (loop, ram) atla
            if (strncmp(dev, "loop", 4) != 0 && strncmp(dev, "ram", 3) != 0) {
                printf("Disk: %s | Okuma işlemi: %lu | Yazma işlemi: %lu\n", dev, read_ops, write_ops);
                break;
            }
        }
    }

    fclose(fp2);
}

// Ağ arayüzlerinden veri aktarım miktarlarını ve bağlantı sayılarını gösterir
void show_network_info() {
    printf("Ağ Arayüzleri Veri Aktarımı:\n");

    FILE *fp = fopen("/proc/net/dev", "r");
    if (!fp) {
        perror("/proc/net/dev açılamadı");
        return;
    }

    char line[512];
    int line_count = 0;

    // Arayüz satırlarını oku
    while (fgets(line, sizeof(line), fp)) {
        line_count++;
        if (line_count <= 2) continue; // başlık satırlarını atla

        char iface[32];
        unsigned long rx_bytes, tx_bytes;

        // Gelen ve giden byte bilgilerini al
        sscanf(line, " %[^:]: %lu %*u %*u %*u %*u %*u %*u %*u %lu",
               iface, &rx_bytes, &tx_bytes);

        printf("Arayüz: %s | Alınan: %.2f MB | Gönderilen: %.2f MB\n",
               iface, rx_bytes / 1024.0 / 1024.0, tx_bytes / 1024.0 / 1024.0);
    }

    fclose(fp);

    // Aktif bağlantı sayısını bulmak için ss komutunu kullan
    printf("\nAktif Ağ Bağlantı Sayısı:\n");

    FILE *fp2 = popen("ss -tun | tail -n +2 | wc -l", "r");
    if (!fp2) {
        perror("ss komutu çalıştırılamadı");
        return;
    }

    int conn_count = 0;
    fscanf(fp2, "%d", &conn_count);
    pclose(fp2);

    printf("Aktif bağlantı sayısı: %d\n", conn_count);
}



