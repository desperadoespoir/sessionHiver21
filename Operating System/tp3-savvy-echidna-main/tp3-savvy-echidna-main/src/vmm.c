#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "conf.h"
#include "common.h"
#include "vmm.h"
#include "tlb.h"
#include "pt.h"
#include "pm.h"

static unsigned int read_count = 0;
static unsigned int write_count = 0;
static FILE* vmm_log;

// Frame access log counter for Least Recently Used (LRU) algorithm
static int frame_access_log[NUM_FRAMES];
static unsigned int frames_in_use_count = 0;

void vmm_init (FILE *log)
{
    // Initialise le fichier de journal.
    vmm_log = log;
}

// NE PAS MODIFIER CETTE FONCTION
static void vmm_log_command (FILE *out, const char *command,
                             unsigned int laddress, /* Logical address. */
                             unsigned int page,
                             unsigned int frame,
                             unsigned int offset,
                             unsigned int paddress, /* Physical address.  */
                             char c) /* Caractère lu ou écrit.  */
{
    if (out)
        fprintf (out, "%s[%c]@%05d: p=%d, o=%d, f=%d pa=%d\n", command, c, laddress,
                 page, offset, frame, paddress);
}

unsigned int get_available_frame(void) {

    // If physical memory is full
    if (frames_in_use_count >= NUM_FRAMES) {

        int min = (int) frame_access_log[0];
        int least_recently_used = 0;

        // Find Least Recently Used frame
        for (int i = 1; i < NUM_FRAMES; i++) {
            if (frame_access_log[i] < min) {
                min = (int) frame_access_log[i];
                least_recently_used = i;
            }
        }

        frame_access_log[least_recently_used] = 0;

        return least_recently_used;
    }
    return frames_in_use_count++;
}

void translate_logical_address(unsigned int logical_address, unsigned int *page_number,
                               unsigned int *frame_number, unsigned int *offset, unsigned int *physical_address, bool read) {

    // Determiner le numero de la page ainsi que le decalage a partir de l'addresse logique 'laddress'
    *page_number = logical_address / PAGE_FRAME_SIZE;
    *offset = logical_address % PAGE_FRAME_SIZE;

    // Chercher le frame correspondant dans le TLB
    *frame_number = tlb_lookup(*page_number, !read);

    // Si un numéro de page n’est pas dans le TLB:
    if ((int)*frame_number < 0) {

        // Chercher le frame correspondant dans le tableau de pages
        *frame_number = pt_lookup(*page_number);

        // Si un numéro de page n’est pas dans le tableau de pages (pt):
        if ((int)*frame_number < 0) {

            // un page-fault est produit.
            // il faut trouver une frame libre dans la mémoire physique
            *frame_number = get_available_frame();

            if(frames_in_use_count >= NUM_FRAMES){
                unsigned int lru_page = pt_inverse_lookup(*frame_number);
                if(!pt_readonly_p(lru_page)){
                    pm_backup_page(*frame_number,lru_page);
                }

                pt_unset_entry(lru_page);
            }

            // Charge la page demandée du backing store
            pm_download_page(*page_number, *frame_number);

            // Modifie l'entrée de `page_number` dans la page table pour qu'elle pointe vers `frame_number`.
            pt_set_entry(*page_number, *frame_number);
        }

        // Placer le résultat dans le TLB
        tlb_add_entry(*page_number, *frame_number, read);
        if(!read){
            pt_set_readonly(*page_number, false);
        }
    }

    // Determiner l'addresse physique
    *physical_address = *frame_number * PAGE_FRAME_SIZE + *offset;

    // counter for Least Recently Used (LRU) algorithm
    // For each physical memory access log, if frame is not in use decrement value, otherwise increment.
    // The frame with minimal value should be replaced as least recently used when memory is full.
    for (int fn = 0; fn < frames_in_use_count; fn++) {
        if (fn != *frame_number)
            frame_access_log[fn]--;
        else
            frame_access_log[fn]++;
    }
}

/* Effectue une lecture à l'adresse logique `laddress`.  */
char vmm_read (unsigned int laddress)
{
    char c = '!';
    read_count++;

    unsigned int page_number, frame_number, offset, physical_address;

    translate_logical_address(laddress, &page_number, &frame_number, &offset, &physical_address,true);

    // Lire le character a l'addresse physique donnee
    c = pm_read(physical_address);

    // Imprimer les informations
    vmm_log_command (stdout, "READING", laddress, page_number, frame_number, offset, physical_address, c);

    return c;
}

/* Effectue une écriture à l'adresse logique `laddress`.  */
void vmm_write (unsigned int laddress, char c)
{
    write_count++;

    unsigned int page_number, frame_number, offset, physical_address;

    translate_logical_address(laddress, &page_number, &frame_number, &offset, &physical_address,false);

    // Ecrire le character dans mémoire physique a l'addresse donnee
    pm_write(physical_address, c);

    // Imprimer les informations
    vmm_log_command (stdout, "WRITING", laddress, page_number, frame_number, offset, physical_address, c);
}


// NE PAS MODIFIER CETTE FONCTION
void vmm_clean (void)
{
    fprintf (stdout, "VM reads : %4u\n", read_count);
    fprintf (stdout, "VM writes: %4u\n", write_count);
}
