#include <stdio.h>

typedef struct {
    float weight;
    float tail_length;
    void (*make_sound)();
} Felidae;

typedef struct {
    Felidae felidae;
} Panthera;

typedef struct {
    Panthera panthera;
} Tigris;

typedef struct {
    Felidae felidae;
} Felis;

typedef struct {
    Felis felis;
} Catus;

void make_roar() {
    printf("Roar\n");
}

void make_meow() {
    printf("Meow\n");
}

void init_felidae(Felidae *felidae, float weight, float tail_length, void (*make_sound)()) {
    // TODO: Assign weight, tail_length, and make_sound to felidae: DONE
    felidae->weight = weight;
    felidae->tail_length = weight;
    felidae->make_sound = make_sound;

    printf("Felidae constructor\n");
    printf("Weight: %.1f kg\n", felidae->weight);
    printf("Tail length: %.1f m\n", felidae->tail_length);
}

void init_panthera(Panthera *panthera, float weight, float tail_length) {
    // TODO: Initialize felidae in panthera DONEE
    init_felidae(&panthera->felidae, weight, tail_length, make_roar);
    printf("Panthera constructor\n");
}

void init_tigris(Tigris *tigris) {
    // TODO: Initialize panthera in tigris
    init_panthera(&tigris->panthera, 221.2, 1.0);
    printf("Tigris constructor\n");
}

void init_felis(Felis *felis, float weight, float tail_length) {
    // TODO: Initialize felidae in felis
    init_felidae(&felis->felidae, weight, tail_length, make_meow);
    printf("Felis constructor\n");
}

void init_catus(Catus *catus) {
    // TODO: Initialize felis in catus
    init_felis(&catus->felis, 4.5, 0.3);
    printf("Catus constructor\n");
}

void deinit_felidae(Felidae *felidae) {
    // TODO: Set make_sound to NULL
    felidae->make_sound = NULL;
    printf("Felidae destructor\n");
}

void deinit_panthera(Panthera *panthera) {
    printf("Panthera destructor\n");
    // TODO: Deinitialize felidae in panthera
    deinit_felidae(&panthera->felidae);
}

void deinit_tigris(Tigris *tigris) {
    printf("Tigris destructor\n");
    // TODO: Deinitialize panthera in tigris
    deinit_felidae(&tigris->panthera);
}

void deinit_felis(Felis *felis) {
    printf("Felis destructor\n");
    // TODO: Deinitialize felidae in felis
    deinit_felidae(&felis->felidae);
}

void deinit_catus(Catus *catus) {
    printf("Catus destructor\n");
    // TODO: Deinitialize felis in catus
    deinit_felidae(&catus->felis);
}

int main() {
    Tigris tigris;
    init_tigris(&tigris);
    tigris.panthera.felidae.make_sound();

    Catus catus;
    init_catus(&catus);
    catus.felis.felidae.make_sound();

    deinit_catus(&catus);
    deinit_tigris(&tigris);
}
