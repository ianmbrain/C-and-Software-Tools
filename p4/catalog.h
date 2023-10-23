
#define PARK_NAME_LENGTH 40

#define COUNTY_NAME_LENGTH 12

struct ParkStruct {
    int id;
    char name[ PARK_NAME_LENGTH + 1 ];
    double lat;
    double lon;
    int numCounties;
    char counties[ 5 ][ COUNTY_NAME_LENGTH + 1 ];
};

typedef struct ParkStruct Park;
