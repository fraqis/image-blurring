#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#include <stdio.h>
#include <stdlib.h>

// Fungsi untuk melakukan Blur (Box Blur)
unsigned char* apply_blur(unsigned char *src, int w, int h, int channels, int radius) {
    unsigned char *dst = (unsigned char*)malloc(w * h * channels);
    
    // Looping setiap piksel pada gambar
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            
            // Untuk setiap channel (R, G, B)
            for (int c = 0; c < channels; c++) {
                float sum = 0.0f;
                int count = 0;

                // Ambil area sekitar piksel berdasarkan radius
                for (int ky = -radius; ky <= radius; ky++) {
                    for (int kx = -radius; kx <= radius; kx++) {
                        int nx = x + kx;
                        int ny = y + ky;

                        // Pastikan koordinat tetangga ada di dalam batas gambar
                        if (nx >= 0 && nx < w && ny >= 0 && ny < h) {
                            sum += src[(ny * w + nx) * channels + c];
                            count++;
                        }
                    }
                }
                // Hitung rata-rata (blur)
                dst[(y * w + x) * channels + c] = (unsigned char)(sum / count);
            }
        }
    }
    return dst;
}

int main() {
    const char *input_path = "hacker.png"; // Pastikan nama file benar
    const char *output_path = "output_blur.png";

    int width, height, channels;
    unsigned char *src = stbi_load(input_path, &width, &height, &channels, 0);
    
    if (!src) {
        fprintf(stderr, "Gagal membaca file %s\n", input_path);
        return 1;
    }

    printf("Memproses gambar: %dx%d (%d channel)\n", width, height, channels);

    // Semakin besar radius, semakin blur gambarnya
    int blur_radius = 5; 
    unsigned char *blurred = apply_blur(src, width, height, channels, blur_radius);

    if (!stbi_write_png(output_path, width, height, channels, blurred, width * channels)) {
        fprintf(stderr, "Gagal menyimpan file %s\n", output_path);
    } else {
        printf("Gambar blur berhasil disimpan ke: %s\n", output_path);
    }

    stbi_image_free(src);
    free(blurred);

    return 0;
}