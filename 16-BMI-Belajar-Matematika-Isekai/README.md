# 16-BMI Belajar Matematika ISEKAI

Basically tinggal searching google instruksi-nya biar tw dia ngapain, with a ~~huge~~ bit help of ChatGPT.

Sauce:
- https://chatgpt.com/share/0464b348-d274-4280-bd98-6d086797ca5d

### Main Workflow
1. Copast ke VSCode
2. Separate Compare function and Main function
3. Analayze from main function, look for each instruction in the documentation
4. use chatgpt to verify and fix stuff
5. use chatgpt to reverse engineer also with https://godbolt.org/

## PROGRAM OUTPUT

```bash
# program.o
330 11786
249 12413
11786.000000 12413.000000
```

```bash
# second-program.o
330 330
249 249
11786.000000 12413.000000
```

## Writeup
- `push {fp, lr}`

Kegunaan: Instruksi ini menyimpan register fp (frame pointer) dan lr (link register) ke stack. Ini dilakukan untuk menjaga nilai-nilai ini sebelum memulai eksekusi fungsi, sehingga dapat dipulihkan nanti.

- `add fp, sp, #4`

Kegunaan: Instruksi ini mengatur fp (frame pointer) ke sp + 4. Frame pointer digunakan untuk melacak awal frame stack saat fungsi dieksekusi.

- `sub sp, sp, #40`

Kegunaan: Mengurangi nilai stack pointer (sp) sebesar 40 byte untuk menyediakan ruang bagi variabel lokal yang akan digunakan dalam fungsi.

- `str r0, [fp, #-40]`

Kegunaan: Menyimpan nilai register r0 (yang berisi argumen pertama fungsi) ke alamat memori yang berada 40 byte di bawah frame pointer (fp).

- `str r1, [fp, #-44]`

Kegunaan: Menyimpan nilai register r1 (yang berisi argumen kedua fungsi) ke alamat memori yang berada 44 byte di bawah frame pointer (fp).

- `ldr r3, [fp, #-40]`

Kegunaan: Memuat nilai yang tersimpan di memori pada alamat fp - 40 (nilai r0 sebelumnya) ke register r3.

- `lsl r3, r3, #3`

Kegunaan: Melakukan operasi left shift logical (lsl) pada register r3 sebanyak 3 bit, yang setara dengan mengalikan nilai r3 dengan 8.

- `add r3, r3, #25`

Kegunaan: Menambahkan nilai 25 ke register r3.

- `lsr r2, r3, #31`

Kegunaan: Melakukan operasi right shift logical (lsr) pada register r3 sebanyak 31 bit, hasilnya disimpan di register r2.

- `add r3, r2, r3`

Kegunaan: Menambahkan nilai dalam register r2 dengan r3 dan hasilnya disimpan kembali di r3.
- `asr r3, r3, #1`

Kegunaan: Melakukan operasi arithmetic shift right (asr) pada register r3 sebanyak 1 bit, setara dengan membagi nilai r3 dengan 2 sambil mempertahankan tanda bit.

- `str r3, [fp, #-8]`

Kegunaan: Menyimpan nilai dalam r3 ke memori pada alamat fp - 8.

- `ldr r3, [fp, #-44]`

Kegunaan: Memuat nilai yang tersimpan di memori pada alamat fp - 44 (nilai r1 sebelumnya) ke register r3.

- `ldr r2, [fp, #-8]`

Kegunaan: Memuat nilai yang tersimpan di memori pada alamat fp - 8 (hasil operasi sebelumnya) ke register r2.

- `mul r3, r2, r3`

Kegunaan: Melakukan operasi perkalian (mul) antara nilai dalam register r2 dan r3, hasilnya disimpan di r3.

- `str r3, [fp, #-12]`

Kegunaan: Menyimpan hasil perkalian dalam r3 ke memori pada alamat fp - 12.

- `ldr r2, [fp, #-44]`

Kegunaan: Memuat kembali nilai yang tersimpan di memori pada alamat fp - 44 (nilai r1).

- `ldr r3, [fp, #-8]`

Kegunaan: Memuat kembali nilai yang tersimpan di memori pada alamat fp - 8 (hasil operasi sebelumnya).

- `add r3, r2, r3`

Kegunaan: Menambahkan nilai dalam r2 dan r3, hasilnya disimpan di r3.

- `str r3, [fp, #-16]`

Kegunaan: Menyimpan hasil penambahan dalam r3 ke memori pada alamat fp - 16.

- `ldr r3, [fp, #-12]`

Kegunaan: Memuat nilai yang tersimpan di memori pada alamat fp - 12 (hasil perkalian).

- `and r3, r3, #3`

Kegunaan: Melakukan operasi AND bitwise antara nilai dalam r3 dengan konstanta 3, hasilnya disimpan di r3.

- `ldr r2, [fp, #-16]`

Kegunaan: Memuat nilai yang tersimpan di memori pada alamat fp - 16.

- `lsl r3, r2, r3`

Kegunaan: Melakukan operasi left shift logical (lsl) pada r2 sebanyak nilai yang tersimpan dalam r3, hasilnya disimpan di r3.

- `str r3, [fp, #-20]`

Kegunaan: Menyimpan hasil left shift dalam r3 ke memori pada alamat fp - 20.

- `ldr r2, [fp, #-20]`

Kegunaan: Memuat nilai yang tersimpan di memori pada alamat fp - 20.

- `ldr r3, [fp, #-16]`

Kegunaan: Memuat nilai yang tersimpan di memori pada alamat fp - 16.

- `orr r3, r2, r3`

Kegunaan: Melakukan operasi OR bitwise antara nilai dalam r2 dan r3, hasilnya disimpan di r3.

- `ldr r2, [fp, #-12]`

Kegunaan: Memuat nilai yang tersimpan di memori pada alamat fp - 12.
- `eor r3, r3, r2`

Kegunaan: Melakukan operasi XOR bitwise antara nilai dalam r3 dan r2, hasilnya disimpan di r3.

- `str r3, [fp, #-24]`

Kegunaan: Menyimpan hasil XOR dalam r3 ke memori pada alamat fp - 24.

- `ldr r2, [fp, #-20]`

Kegunaan: Memuat kembali nilai yang tersimpan di memori pada alamat fp - 20.

- `ldr r1, [fp, #-16]`

Kegunaan: Memuat kembali nilai yang tersimpan di memori pada alamat fp - 16.

- `ldr r0, .L3`

Kegunaan: Memuat alamat string format ("%d %d\n") ke dalam register r0.

- `bl printf`

Kegunaan: Memanggil fungsi printf untuk mencetak nilai yang telah disiapkan di r2 dan r1.

- `str r0, [fp, #-28]`

Kegunaan: Menyimpan hasil yang dikembalikan oleh printf ke memori pada alamat fp - 28.

- `ldr r2, [fp, #-28]`

Kegunaan: Memuat kembali nilai yang tersimpan di memori pada alamat fp - 28.

- `ldr r1, [fp, #-24]`

Kegunaan: Memuat kembali nilai yang tersimpan di memori pada alamat fp - 24.

- `ldr r0, .L3`

Kegunaan: Memuat alamat string format ("%d %d\n") ke dalam register r0.

- `bl printf`

Kegunaan: Memanggil fungsi printf untuk mencetak hasil kedua.

- `str r0, [fp, #-32]`

Kegunaan: Menyimpan hasil yang dikembalikan oleh printf ke memori pada alamat fp - 32.

- `ldr r3, [fp, #-28]`

Kegunaan: Memuat nilai yang tersimpan di memori pada alamat fp - 28.

- `ldr r2, [fp, #-32]`

Kegunaan: Memuat nilai yang tersimpan di memori pada alamat fp - 32.

- `mul r3, r2, r3`

Kegunaan: Melakukan operasi perkalian antara r2 dan r3.

- `mov r0, r3`

Kegunaan: Memindahkan hasil perkalian ke r0 untuk mengembalikan hasil dari fungsi compute.

- `sub sp, fp, #4`

Kegunaan: Mengatur ulang stack pointer (sp) ke posisi sebelumnya sebelum frame pointer.

- `pop {fp, lr}`

Kegunaan: Mengambil kembali nilai fp dan lr dari stack, mengembalikan stack pointer ke posisi sebelum fungsi dipanggil.

- `bx lr`

Kegunaan: Mengembalikan kontrol ke fungsi pemanggil dengan melompat ke alamat yang disimpan di lr.

Fungsi Pendukung dalam main
- `bl __aeabi_i2f` dan `bl __aeabi_f2d`
digunakan untuk konversi tipe data dari integer ke floating-point dan dari floating-point ke double precision.

- `bl __aeabi_fadd` digunakan untuk menambahkan dua nilai floating-point.