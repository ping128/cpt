///////////////////////////////////////////////////////////////////////////////
//  Earth Coordinates
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>

using namespace std;

void latitude_longitude_to_xyz(double R, double lat, double lon, double &x, double &y, double &z) {
    x = -R * cos(lat) * cos(lon);
    y = R * sin(lat);
    z = R * cos(lat) * sin(lon);
}

// Return the distance between the two given points
// R = Earth's radius
double cal_dist(double R, double lat1, double long1, double lat2, double long2) {
    double xi, yi, zi;
    latitude_longitude_to_xyz(R, lat1 * M_PI / 180.0, long1 * M_PI / 180.0, xi, yi, zi);
    double xj, yj, zj;
    latitude_longitude_to_xyz(R, lat2 * M_PI / 180.0, long2 * M_PI / 180.0, xj, yj, zj);

    double dot = xi * xj + yi * yj + zi * zj;
    double dist_i = sqrt (xi * xi + yi * yi + zi * zi);
    double dist_j = sqrt (xj * xj + yj * yj + zj * zj);
    double angle = acos(dot / dist_i / dist_j);
    return min(R * angle, R * (2 * M_PI - angle));
}
