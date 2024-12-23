#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct {
    int id;
    char nama[50];
    char jenisKelamin[10];
    int lamaTahunBekerja; 
} Pegawai;


void tambahPegawai(Pegawai *pegawai, int *count);
void tampilkanPegawai(Pegawai *pegawai, int count);
void hapusPegawai(Pegawai *pegawai, int *count, int key);
void simpanKeDatabase(Pegawai *pegawai, int count, const char *filename);
void mergeSort(Pegawai *pegawai, int left, int right);
void merge(Pegawai *pegawai, int left, int mid, int right);
int jumpSearch(Pegawai *pegawai, int count, int key);

int main() {
    int pilihan, count = 0, cariID, hasilCari;
    Pegawai pegawai[100];
    char filename[] = "database_pegawai.txt";

    
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        while (fscanf(file, "%d,%49[^,],%9[^,],%d\n", 
                      &pegawai[count].id, 
                      pegawai[count].nama, 
                      pegawai[count].jenisKelamin, 
                      &pegawai[count].lamaTahunBekerja) != EOF) {
            count++;
        }
        fclose(file);
        printf("Data berhasil dimuat dari file %s\n", filename);
    }

    do {
        printf("\n===========================================================\n");
        printf("|                 Menu Manajemen Data Pegawai              |\n");
        printf("===========================================================\n");
        printf("| 1. Tambah Data Pegawai                                   |\n");
        printf("| 2. Tampilkan Data Pegawai                                |\n");
        printf("| 3. Cari Data Pegawai berdasarkan ID                      |\n");
        printf("| 4. Hapus Data Pegawai                                    |\n");
        printf("| 5. Keluar                                                |\n");
        printf("===========================================================\n");
        printf("Pilihan Anda: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                tambahPegawai(pegawai, &count);
                break;
            case 2:
                tampilkanPegawai(pegawai, count);
                break;
            case 3:
                printf("Masukkan ID Pegawai yang dicari: ");
                scanf("%d", &cariID);
                hasilCari = jumpSearch(pegawai, count, cariID);
                if (hasilCari != -1) {
                    printf("Data Ditemukan:\n");
                    printf("|  ID  |     Nama      | Jenis Kelamin | Lama Tahun Bekerja |\n");
                    printf("| %4d | %-13s | %-12s | %19d |\n",
                           pegawai[hasilCari].id, pegawai[hasilCari].nama,
                           pegawai[hasilCari].jenisKelamin, pegawai[hasilCari].lamaTahunBekerja);
                } else {
                    printf("Data tidak ditemukan!\n");
                }
                break;
            case 4:
                printf("Masukkan ID Pegawai yang ingin dihapus: ");
                scanf("%d", &cariID);
                hapusPegawai(pegawai, &count, cariID);
                break;
            case 5:
                printf("Menyimpan data ke database...\n");
                simpanKeDatabase(pegawai, count, filename);
                printf("Data berhasil disimpan. Keluar dari program.\n");
                break;
            default:
                printf("Pilihan tidak valid!\n");
        }
    } while (pilihan != 5);

    return 0;
}

void tambahPegawai(Pegawai *pegawai, int *count) {
    printf("\nMasukkan ID Pegawai: ");
    scanf("%d", &pegawai[*count].id);
    printf("Masukkan Nama Pegawai: ");
    scanf(" %[^\n]s", pegawai[*count].nama);
    printf("Masukkan Jenis Kelamin (L/P): ");
    scanf(" %s", pegawai[*count].jenisKelamin);
    printf("Masukkan Lama Tahun Bekerja: ");
    scanf("%d", &pegawai[*count].lamaTahunBekerja);

    (*count)++;
    mergeSort(pegawai, 0, *count - 1); 
    printf("Data pegawai berhasil ditambahkan!\n");
}

void tampilkanPegawai(Pegawai *pegawai, int count) {
    printf("\n=====================================================================\n");
    printf("|  ID  |     Nama          | Jenis Kelamin | Lama Tahun Bekerja    |\n");
    printf("=====================================================================\n");
    for (int i = 0; i < count; i++) {
        printf("| %4d | %-17s | %-12s | %19d |\n",
               pegawai[i].id, pegawai[i].nama, pegawai[i].jenisKelamin, pegawai[i].lamaTahunBekerja);
    }
    printf("=====================================================================\n");
}

void hapusPegawai(Pegawai *pegawai, int *count, int key) {
    int index = jumpSearch(pegawai, *count, key);
    if (index == -1) {
        printf("Data dengan ID %d tidak ditemukan!\n", key);
        return;
    }
    for (int i = index; i < *count - 1; i++) {
        pegawai[i] = pegawai[i + 1];
    }
    (*count)--;
    printf("Data pegawai dengan ID %d berhasil dihapus!\n", key);
}

void simpanKeDatabase(Pegawai *pegawai, int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Gagal menyimpan ke database!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%s,%d\n", pegawai[i].id, pegawai[i].nama,
                pegawai[i].jenisKelamin, pegawai[i].lamaTahunBekerja);
    }
    fclose(file);
    printf("Data berhasil disimpan ke file %s\n", filename);
}

void mergeSort(Pegawai *pegawai, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(pegawai, left, mid);
        mergeSort(pegawai, mid + 1, right);
        merge(pegawai, left, mid, right);
    }
}

void merge(Pegawai *pegawai, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Pegawai L[n1], R[n2];

    for (int i = 0; i < n1; i++) L[i] = pegawai[left + i];
    for (int i = 0; i < n2; i++) R[i] = pegawai[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].id <= R[j].id) {
            pegawai[k++] = L[i++];
        } else {
            pegawai[k++] = R[j++];
        }
    }

    while (i < n1) pegawai[k++] = L[i++];
    while (j < n2) pegawai[k++] = R[j++];
}

int jumpSearch(Pegawai *pegawai, int count, int key) {
    int step = sqrt(count);
    int prev = 0;

    while (pegawai[step - 1].id < key && step < count) {
        prev = step;
        step += sqrt(count);
        if (prev >= count) return -1;
    }

    for (int i = prev; i < step && i < count; i++) {
        if (pegawai[i].id == key) return i;
    }
    return -1;
}



