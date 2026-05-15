#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define NUM_FRAMES 100
#define PAGE_SIZE 256
#define MAX_VIRTUAL_ADDRESS 0xFFFF

// estados de los frames
typedef enum {
    FREE = 0,
    OCCUPIED = 1
} FrameState;

// Entry para la tabla de paginas
typedef struct {
    bool valid;
    int pfn;
} PTE;

// Memoria fisica
FrameState physical_memory[NUM_FRAMES];

// Tabla de paginas
PTE page_table[256];

// Inicializa la memoria fisica
void init_ram(int seed, int num_virtual_pages) {
    srand(seed);
    int required_free = (num_virtual_pages > 10) ? num_virtual_pages : 10;
    int iterations = 0;
    
    while (true) {
        iterations++;
        if (iterations > 1000) {
            fprintf(stderr, "Fatal error: Could not satisfy RAM randomization constraints after 1000 iterations.\n");
            exit(1);
        }

        // limpia la memoria
        for (int i = 0; i < NUM_FRAMES; i++) {
            physical_memory[i] = FREE;
        }
        
        int occupied_count = (rand() % 51) + 10;
        
        // distribuye los frames ocupados aleatoriamente
        int assigned = 0;
        while (assigned < occupied_count) {
            int frame_idx = rand() % NUM_FRAMES;
            if (physical_memory[frame_idx] == FREE) {
                physical_memory[frame_idx] = OCCUPIED;
                assigned++;
            }
        }
        
        // cuenta los frames libres y verifica la condicion
        int free_count = NUM_FRAMES - occupied_count;
        if (free_count >= required_free) {
            break; // valid randomization found
        }
    }
}

// imprime el mapa de la memoria fisica
void print_ram_map(int seed) {
    int free_count = 0;
    int occupied_count = 0;
    
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (physical_memory[i] == FREE) {
            free_count++;
        } else {
            occupied_count++;
        }
    }
    
    printf("PHYSICAL RAM (%d frames) after random init (seed=%d):\n", NUM_FRAMES, seed);
    printf("FREE=%d OCCUPIED=%d\n", free_count, occupied_count);
    
    for (int i = 0; i < NUM_FRAMES; i++) {
        printf("%d:%c ", i, (physical_memory[i] == FREE) ? 'F' : 'X');
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

// asigna un frame libre
int allocate_frame() {
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (physical_memory[i] == FREE) {
            physical_memory[i] = OCCUPIED;
            return i;
        }
    }
    return -1; // Allocation failed (pedillos)
}

// carga el proceso asignando frames para cada pagina virtual.
bool load_process(int num_virtual_pages) {
    // inicializa la tabla de paginas a invalida
    for (int i = 0; i < 256; i++) {
        page_table[i].valid = false;
        page_table[i].pfn = -1;
    }
    
    printf("Load process: V=%d -> VPN 0..%d mapped to PFNs [ ", num_virtual_pages, num_virtual_pages - 1);
    for (int i = 0; i < num_virtual_pages; i++) {
        int frame = allocate_frame();
        if (frame == -1) {
            printf("]\nError: Failed to allocate frame for VPN %d. Aborting load.\n", i);
            return false;
        }
        page_table[i].valid = true;
        page_table[i].pfn = frame;
        printf("%d ", frame);
    }
    printf("]\n");
    return true;
}

// traduce una direccion virtual e imprime el resultado.
void translate_address(unsigned long int va, int num_virtual_pages) {
    if (va > MAX_VIRTUAL_ADDRESS) {
        printf("VA=%lu\t\t\tERROR=VA_OUT_OF_RANGE\n", va);
        return;
    }
    
    unsigned int offset = va & 0xFF;
    unsigned int vpn = (va >> 8) & 0xFF;
    
    if (vpn >= (unsigned int)num_virtual_pages) {
        printf("VA=%lu (0x%04lX)\tERROR=VPN_OUT_OF_RANGE (vpn=%u, V=%d)\n", va, va, vpn, num_virtual_pages);
        return;
    }
    
    if (!page_table[vpn].valid) {
        printf("VA=0x%04lX (%lu)\tVPN=0x%02X OFF=0x%02X ERROR=PAGE_NOT_MAPPED\n", va, va, vpn, offset);
        return;
    }
    
    unsigned int pfn = page_table[vpn].pfn;
    unsigned int pa = (pfn * PAGE_SIZE) + offset;
    
    printf("VA=0x%04lX (%lu)\tVPN=0x%02X OFF=0x%02X PFN=%u PA=0x%04X (%u)\n", 
           va, va, vpn, offset, pfn, pa, pa);
}

// lee las direcciones del archivo y las procesa
void process_addresses(const char *filename, int num_virtual_pages) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;
        
        // parsea la direccion virtual
        char *endptr;
        unsigned long int va = strtoul(line, &endptr, 0);
        
        if (endptr == line) {
            fprintf(stderr, "Error parsing line: %s\n", line);
            continue;
        }
        
        translate_address(va, num_virtual_pages);
    }
    
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <NUM_VIRTUAL_PAGES> <address_file> [seed]\n", argv[0]);
        return 1;
    }
    
    int num_virtual_pages = atoi(argv[1]);
    if (num_virtual_pages < 1 || num_virtual_pages > 256) {
        fprintf(stderr, "Error: NUM_VIRTUAL_PAGES must be between 1 and 256.\n");
        return 1;
    }
    
    const char *address_file = argv[2];
    
    int seed;
    if (argc >= 4) {
        seed = atoi(argv[3]);
    } else {
        seed = time(NULL);
    }
    
    init_ram(seed, num_virtual_pages);
    print_ram_map(seed);
    
    if (load_process(num_virtual_pages)) {
        process_addresses(address_file, num_virtual_pages);
    }
    
    return 0;
}
