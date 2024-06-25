#include <iostream>
#include <string>
#include <qrcodegen.hpp>
#include <fstream>

using namespace std;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

void generateQRCode(const string &data, const string &filename) {
    // Create the QR code
    const QrCode qr = QrCode::encodeText(data.c_str(), QrCode::Ecc::LOW);

    // Open
