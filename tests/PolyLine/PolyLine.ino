// lat = coordinatesForArea[i][0]
// lng = coordinatesForArea[i][1]
// cntr is the count of points

#include <Arduino.h>


static const double s_presision   = 100000.0;
static const int    s_chunkSize   = 5;
static const int    s_asciiOffset = 63;
static const int    s_5bitMask    = 0x1f; // 0b11111 = 31
static const int    s_6bitMask    = 0x20; // 0b100000 = 32

using Point = std::tuple<double, double>;
using Polyline = std::vector<Point>;
/**
 * @copyright Vahan Aghajanyan <vahancho@gmail.com>
 * @param coords
 * @param i
 * @return
 */
double PolylineDecode(const std::string &coords, size_t &i)
{
    assert(i < coords.size());

    int32_t result = 0;
    int shift = 0;
    char c = 0;
    do {
        c = coords.at(i++);
        c -= s_asciiOffset;      // (10)
        result |= (c & s_5bitMask) << shift;
        shift += s_chunkSize;    // (7)
    } while (c >= s_6bitMask);

    if (result & 1) {
        result = ~result;        // (5)
    }
    result >>= 1;                // (4)

    // Convert to decimal value.
    return result / s_presision; // (2)
}
/**
 * @copyright Vahan Aghajanyan <vahancho@gmail.com>
 * @param coords
 * @return
 */
Polyline PolylineDecode(const std::string &coords)
{
    Polyline polyline;

    size_t i = 0;
    while (i < coords.size())
    {
        auto lat = decode(coords, i);
        auto lon = decode(coords, i);

        if (!polyline.empty()) {
            const auto &prevPoint = polyline.back();
            lat += std::get<0>(prevPoint);
            lon += std::get<1>(prevPoint);
        }
        polyline.emplace_back(lat, lon);
    }

    return polyline;
}

/**
 *
 */
void drawPoly() {
    float newPoints[100][2];
    float maxLat = 0;
    float maxLng = 0;
    float minLat = 180;
    float minLng = 180;

    for (int i = 0; i < cntr; i++) {
        maxLat = max(maxLat, coordinatesForArea[i][0]);
        maxLng = max(maxLng, coordinatesForArea[i][1]);
        minLat = min(minLat, coordinatesForArea[i][0]);
        minLng = min(minLng, coordinatesForArea[i][1]);
    }

    float ySize = maxLat - minLat;
    float xSize = maxLng - minLng;

    float scaleFactor = ySize / 300;

    float maxY = 0;
    float maxX = 0;

    for (int i = 0; i < cntr; i++) {
        newPoints[i][1] = (coordinatesForArea[i][1] - minLng) / scaleFactor;
        newPoints[i][0] = (coordinatesForArea[i][0] - minLat) / scaleFactor;
        maxX = max(maxX, newPoints[i][0]);
        maxY = max(maxY, newPoints[i][1]);
    }


    float newScaleFactor = maxY > maxX ? maxY / 300 : maxX / 300;

    float newMaxX = 0;

    for (int i = 0; i < cntr; i++) {
        newPoints[i][1] = newPoints[i][1] / newScaleFactor;
        newPoints[i][0] = newPoints[i][0] / newScaleFactor;

        newMaxX = max(newMaxX, newPoints[i][0]);
    }

    float newOffsetX = (320 - newMaxX) / 2;
    float newOffsetY = 100;

    for (int i = 0; i < cntr; i++) {
        newPoints[i][1] = newPoints[i][1] + newOffsetY;
        newPoints[i][0] = newPoints[i][0] + newOffsetX;
    }

    for (int i = 0; i < cntr - 1; i++) {
        myGLCD.drawLine(newPoints[i][0], newPoints[i][1], newPoints[i + 1][0], newPoints[i + 1][1]);
    }

    myGLCD.drawLine(newPoints[0][0], newPoints[0][1], newPoints[cntr - 1][0], newPoints[cntr - 1][1]);
}

void setup() {

}


void loop() {

}
