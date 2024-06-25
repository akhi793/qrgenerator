#include <iostream>
#include <string>
#include <qrencode.h>
#include <png.h>

void savePNG(const char *filename, QRcode *qrcode) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        std::cerr << "Failed to create PNG write struct" << std::endl;
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        std::cerr << "Failed to create PNG info struct" << std::endl;
        png_destroy_write_struct(&png, nullptr);
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png))) {
        std::cerr << "PNG error during setup" << std::endl;
        png_destroy_write_struct(&png, &info);
        exit(EXIT_FAILURE);
    }

    png_init_io(png, fp);
    png_set_IHDR(
        png,
        info,
        qrcode->width,
        qrcode->width,
        8,
        PNG_COLOR_TYPE_GRAY,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    png_write_info(png, info);

    for (int y = 0; y < qrcode->width; y++) {
        png_write_row(png, qrcode->data + y * qrcode->width);
    }

    png_write_end(png, nullptr);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
}

int main() {
    std::string data;
    std::cout << "Enter data to display on QR code:\n";
    std::getline(std::cin, data);

    QRcode *qrcode = QRcode_encodeString(data.c_str(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    if (!qrcode) {
        std::cerr << "Failed to generate QR code" << std::endl;
        exit(EXIT_FAILURE);
    }

    savePNG("gen.png", qrcode);
    QRcode_free(qrcode);

    std::cout << "QR code saved to gen.png" << std::endl;
    return 0;
}
