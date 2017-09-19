struct current_cd_st{
    int artist_id;
    int cd_id;
    char artist_name[100];
    char title[100];
    char catalogue[100];
};

struct current_tracks_st{
    int cd_id;
    char track[20][100];
};

#define MAX_CD_RESULT 10
struct cd_search_st{
    int cd_id[MAX_CD_RESULT];
};

int database_start(char *name, char *password);
void database_end();

//Functions for adding a CD
int add_cd(char *artist, char *title, char *catalogue, int *cd_id);
int add_tracks(struct current_tracks_st *tracks);

//Functions for finding and retrieving a CD
int find_cds(char *search_str, struct cd_search_st *result);
int get_cd(int cd_id, struct current_cd_st *dest);
int get_cd_tracks(int cd_id, struct current_tracks_st *dest);

//Function for deleting items
int delete_cd(int cd_id);
int delete_tracks(int cd_id);

//Function for count the database(CD, tracks, artist)
int database_count(int (*data)[3]);

//Function for find a cd'id with the catalogue
int find_cd_with_catalogue(char *catalogue);

