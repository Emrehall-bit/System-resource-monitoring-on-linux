#include <stdio.h>
#include <stdlib.h>
// reporter.c Dosyası, Resource utilization bilgilerini CSV dosyasına yazarak rapor oluşturan fonksiyonları içerir. (oluşturulan dosya ana dizinde)



// CPU, RAM ve Disk bilgilerini okuyup system_report.csv dosyasına yazar
void write_resource_report() {
    FILE *fp = fopen("system_report.csv", "w");
    if (!fp) {
        perror("Dosya oluşturulamadı");
        return;
    }

    // CSV dosyasına başlık satırı yaz
    fprintf(fp, "Resource,Value\n");

    //CPU BİLGİSİ
    FILE *stat = fopen("/proc/stat", "r");
    if (stat) {
        char line[256];
        fgets(line, sizeof(line), stat);  // ilk satırı oku: cpu ...

        unsigned long user, nice, system, idle;
        sscanf(line, "cpu %lu %lu %lu %lu", &user, &nice, &system, &idle);
        unsigned long total = user + nice + system + idle;
        float usage = 100.0 * (user + system) / total;

        fprintf(fp, "CPU Usage,%.1f%%\n", usage);
        fclose(stat);
    }

    // RAM BİLGİSİ
    FILE *mem = fopen("/proc/meminfo", "r");
    if (mem) {
        unsigned long mem_total = 0, mem_free = 0;
        char line[256];

        // RAM toplam ve boş miktarlarını oku
        while (fgets(line, sizeof(line), mem)) {
            if (sscanf(line, "MemTotal: %lu", &mem_total) == 1) continue;
            if (sscanf(line, "MemFree: %lu", &mem_free) == 1) continue;
        }
        fclose(mem);

        fprintf(fp, "RAM Total,%.1f MB\n", mem_total / 1024.0);
        fprintf(fp, "RAM Free,%.1f MB\n", mem_free / 1024.0);
    }

    //DİSK KULLANIM YÜZDESİ
    // "df" komutu ile kök dizinin kullanım yüzdesini al
    FILE *df = popen("df --output=pcent / | tail -n 1", "r");
    if (df) {
        char percent[16];
        fscanf(df, "%s", percent);
        fprintf(fp, "Disk Used,%s\n", percent);
        pclose(df);
    }

    fclose(fp);
    printf("Rapor oluşturuldu: system_report.csv\n");
}


