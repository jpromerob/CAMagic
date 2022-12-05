#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;



struct pixel {
    int x;
    int y;
};

struct map {
        int np;
        pixel p[2];
};

void print_lut(int width, int height, map lut[]){
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {  
            printf("(%i, %i): ", x, y);
            printf("%i --> ", lut[x*height+y].np);
            printf("(%i, %i)\t\t", lut[x*height+y].p[0].x, lut[x*height+y].p[0].y);
            printf("(%i, %i)\n", lut[x*height+y].p[1].x, lut[x*height+y].p[1].y);
        }
    }
}

void count_stuff(int width, int height, map lut[]){

    int count_0 = 0;
    int count_1 = 0;
    int count_2 = 0;

    
    for(int idx=0; idx < width*height; idx++){
        switch(lut[idx].np){
            case 0:
                count_0 += 1;
                break;
            case 1:
                count_1 += 1;
                break;
            case 2:
                count_2 += 1;
                break;

        }
    }

    printf("%i with 0 maps\n", count_0);
    printf("%i with 1 maps\n", count_1);
    printf("%i with 2 maps\n", count_2);
}

void get_empty_lut(int width, int height, map lut[]){
    

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {           
            lut[x*height+y].np = 0;
            lut[x*height+y].p[0].x = -1;            
            lut[x*height+y].p[0].y = -1;         
            lut[x*height+y].p[1].x = -1;      
            lut[x*height+y].p[1].y = -1;
        }
    }
}

void classic_load_lut(char fname[], int width, int height, map lut[]){

    get_empty_lut(width, height, lut); 

    FILE *file = fopen(fname, "r");
    
    if(!file)
    {
        printf("Could not open the file\n");
    }

    
    char content[1024];
    int lutix;
    bool done = false;
    while(fgets(content, 1024, file))
    {
        char *val = strtok(content, ",");
        int tok_ix = 0;
        int np = 0;
        while(val && !done )
        {
            switch(tok_ix) {
                case 0:
                    lutix = stoi(val);
                    printf("lutix: %i\t", lutix);
                    break;
                case 1:
                    lut[lutix].p[0].x= stoi(val);
                    if (lut[lutix].p[0].x >= 0){
                        np += 1;
                    }
                    printf("x[0]: %i\t", lut[lutix].p[0].x);
                    break;
                case 2:
                    lut[lutix].p[0].y= stoi(val);
                    lut[lutix].np= np;
                    printf("y[0]: %i\t", lut[lutix].p[0].y);
                    break;
                case 3:
                    lut[lutix].p[1].x= stoi(val);
                    if (lut[lutix].p[1].x >= 0){
                        np += 1;
                    }
                    printf("x[1]: %i\t", lut[lutix].p[1].x);
                    break;
                case 4:
                    lut[lutix].p[1].y= stoi(val);
                    lut[lutix].np= np;
                    printf("y[1]: %i\n", lut[lutix].p[1].y);
                    break;
                case 5:
                    done = true;
                    break;
            }
            tok_ix += 1;

            val = strtok(NULL, ",");
            
        }
    }
    fclose(file);
    
}

void load_lut(char fname[], int width, int height, map lut[]){

    
    get_empty_lut(width, height, lut);   

    // File pointer
    std::ifstream infile(fname);  
    cout << "Doing stuff" << "\n";

    vector<string> row;
    string line, val, temp;  

    while (std::getline(infile, line))
    {
        stringstream s(line);
  
        // read every column data of a row and
        // store it in a string variable, 'val'
        int tok_ix = 0;
        int np = 0;
        int lutix;
        bool done = false;
        while (getline(s, val, ',')) 
        {

            // std::istringstream iss(val);
            switch(tok_ix) {
                case 0:
                    lutix = stoi(val);
                    printf("lutix: %i\t", lutix);
                    break;
                case 1:
                    lut[lutix].p[0].x= stoi(val);
                    if (lut[lutix].p[0].x >= 0){
                        np += 1;
                    }
                    printf("x[0]: %i\t", lut[lutix].p[0].x);
                    break;
                case 2:
                    lut[lutix].p[0].y= stoi(val);
                    lut[lutix].np= np;
                    printf("y[0]: %i\t", lut[lutix].p[0].y);
                    break;
                case 3:
                    lut[lutix].p[1].x= stoi(val);
                    if (lut[lutix].p[1].x >= 0){
                        np += 1;
                    }
                    printf("x[1]: %i\t", lut[lutix].p[1].x);
                    break;
                case 4:
                    lut[lutix].p[1].y= stoi(val);
                    lut[lutix].np= np;
                    printf("y[1]: %i\n", lut[lutix].p[1].y);
                    break;
                case 5:
                    done = true;
                    break;
            }
            tok_ix += 1;
        }
    }
    
    // print_lut(width, height, lut);
    
}

void create_events(int nbev, int width, int height, pixel events[]){
    
    printf("Events at:\n");
    for (int ev_ix = 0; ev_ix < nbev; ev_ix++) {
        events[ev_ix].x = std::rand() % width;
        events[ev_ix].y = std::rand() % height;
        // printf("\t (%i, %i)\n", events[ev_ix].x, events[ev_ix].y);
    }
}

void remap_event(pixel old_xy, int width, int height, map lut[]){

    pixel new_xy;
    for(int pixix = 0; pixix < lut[old_xy.x*height+old_xy.y].np; pixix++){
        new_xy.x = lut[old_xy.x*height+old_xy.y].p[pixix].x;
        new_xy.y = lut[old_xy.x*height+old_xy.y].p[pixix].y;
        printf("%i: (%i, %i) --> (%i, %i)\n", old_xy.x*height+old_xy.y, old_xy.x, old_xy.y, new_xy.x, new_xy.y);
    }

}


int main(int argc, char** argv)
{

    
    int width = stoi(argv[1]);
    int height = stoi(argv[2]);
    
    int mode = stoi(argv[4]);
    
    printf("Size: %i x %i\n", width, height);

    map lut[width*height];

    if (mode == 0){
        classic_load_lut(argv[3], width, height, lut); 
    }
    if (mode == 1){
        load_lut(argv[3], width, height, lut); 
    }

    print_lut(width, height, lut);

    count_stuff(width, height, lut);


    // int nbev = 16;
    // pixel events[nbev];
    // create_events(nbev, width, height, events);

    // for(int evix = 0; evix < nbev; evix++){
    //     remap_event(events[evix], width, height, lut);
    // }

    return 0;
}